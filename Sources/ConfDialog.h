#ifndef CONFDIALOG_H
#define CONFDIALOG_H

#include "resource.h"
#include "Configuration.h"
#include "afxwin.h"

// CSubDialog dialog
class ConfDialog : public CDialog
{
	CONF_PARAMS *m_pConfParams;
	CONF_PARAMS m_ConfParams;
public:
	ConfDialog(CWnd* pParent, struct CONF_PARAMS *pConfParams );   // standard constructor
	virtual ~ConfDialog();

// Dialog Data
	enum { IDD = IDD_CONFIGURATION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	CString m_strBDAExt;
	int m_iRollOff;
	int m_iPilot;
	CComboBox m_ctrlRelockTime;
	CComboBox m_ctrlMIS;
};

#endif //CONFDIALOG_H
