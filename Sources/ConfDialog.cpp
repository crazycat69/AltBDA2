#include "stdafx.h"
// ConfDialog.cpp : implementation file
//

#include "ConfDialog.h"

// ConfDialog dialog

ConfDialog::ConfDialog(CWnd* pParent, struct CONF_PARAMS *pConfParams)
	: CDialog(IDD, pParent)
	, m_pConfParams (pConfParams)
	, m_ConfParams (*pConfParams)
{
}

ConfDialog::~ConfDialog()
{
}

BOOL ConfDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	switch(m_ConfParams.VendorSpecific)
	{
	case TT_BDA:
		m_strBDAExt = "Technotrend BDA";
		break;
	case TH_BDA:
		m_strBDAExt = "Twinhan BDA";
		break;
	case DW_BDA:
		m_strBDAExt = "DvbWolrld BDA";
		break;
	case HAUP_BDA:
		m_strBDAExt = "Hauppauge BDA";
		break;
	case CXT_BDA:
		m_strBDAExt = "Conexant BDA";
		break;
	case TBS_BDA:
		m_strBDAExt = "Turbosight-Conexant BDA";
		break;
	case TBS_NXP_BDA:
		m_strBDAExt = "Turbosight-NXP BDA";
		break;
	case QBOX_BDA:
		m_strBDAExt = "Turbosight-QBOX BDA";
		break;
	case TV_BDA:
		m_strBDAExt = "TeVii BDA";
		break;
	case OMC_BDA:
		m_strBDAExt = "Omicom BDA";
		break;
	case COMPRO_BDA:
		m_strBDAExt = "Compro BDA";
		break;
	case ANYSEE_BDA:
		m_strBDAExt = "AnySee BDA";
		break;
	case GNP_BDA:
		m_strBDAExt = "Genpix BDA";
		break;
	case GENPIX_BDA:
		m_strBDAExt = "Genpix advanced BDA";
		break;
	case NETUP_BDA:
		m_strBDAExt = "Netup BDA";
		break;
	case CRAZY_BDA:
		m_strBDAExt = "Crazy BDA";
		break;
	case MS_BDA:
		m_strBDAExt = "Microsoft BDA";
		break;
	case AMT_BDA:
		m_strBDAExt = "Astrometa BDA";
		break;
	case PURE_BDA:
	default:
		m_strBDAExt = "Generic BDA";
	}

	m_strBDAExt.Append(" used");

	m_ctrlRelockTime.InsertString(0,"OFF");
	for (int i=1; i<=MAX_TIMEOUT; i++)
	{
		CString str;
		str.Format("%i",i);
		m_ctrlRelockTime.InsertString(i,str);
	}
	m_ctrlRelockTime.SetCurSel(m_ConfParams.RelockTimeout);

	UpdateData(FALSE);

	return TRUE;
}

void ConfDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_BDAEXT, m_strBDAExt);
	DDX_Control(pDX, IDC_COMBO_RELOCKTIME, m_ctrlRelockTime);
}

BEGIN_MESSAGE_MAP(ConfDialog, CDialog)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// ConfDialog message handlers

void ConfDialog::OnClose()
{
	CDialog::OnClose();
}

void ConfDialog::OnBnClickedOk()
{
	UpdateData();
	m_ConfParams.RelockTimeout = m_ctrlRelockTime.GetCurSel();

	*m_pConfParams = m_ConfParams; 

	CDialog::OnOK();
}
