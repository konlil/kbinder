// mfc1.h : main header file for the MFC1 application
//

#if !defined(AFX_MFC1_H__A987D33A_527B_4395_8545_975503F73BAB__INCLUDED_)
#define AFX_MFC1_H__A987D33A_527B_4395_8545_975503F73BAB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMfc1App:
// See mfc1.cpp for the implementation of this class
//

class CMfc1App : public CWinApp
{
public:
	CMfc1App();

	std::string m_szUrl;
	std::string m_szTarget;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMfc1App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMfc1App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MFC1_H__A987D33A_527B_4395_8545_975503F73BAB__INCLUDED_)
