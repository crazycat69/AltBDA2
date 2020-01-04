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
	case PURE_BDA:
	default:
		m_strBDAExt = "Generic BDA";
	}

	m_strBDAExt.Append(" used");

	switch(m_ConfParams.S2Pilot)
	{
		case PILOT_OFF:
			m_iPilot=1;
			break;
		case PILOT_ON:
			m_iPilot=2;
			break;
		case PILOT_NOT_SET:
		default:
			m_iPilot=0;
	}

	switch(m_ConfParams.S2RollOff)
	{
	case ROLLOFF_20:
		m_iRollOff=1;
		break;
	case ROLLOFF_25:
		m_iRollOff=2;
		break;
	case ROLLOFF_35:
		m_iRollOff=3;
		break;
	case ROLLOFF_NOT_SET:
	default:
		m_iRollOff=0;
	}

	m_ctrlMIS.InsertString(0,"OFF");
	for (int i=0; i<256; i++)
	{
		CString str;
		str.Format("%i",i);
		m_ctrlMIS.InsertString(i+1,str);
	}
	m_ctrlMIS.SetCurSel(m_ConfParams.MIS+1);

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
	DDX_CBIndex(pDX, IDC_COMBO_ROLLOFF, m_iRollOff);
	DDX_CBIndex(pDX, IDC_COMBO_PILOT, m_iPilot);
	DDX_Control(pDX, IDC_COMBO_MIS, m_ctrlMIS);
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
	switch(m_iPilot)
	{
	case 1:
		m_ConfParams.S2Pilot=PILOT_OFF;
		break;
	case 2:
		m_ConfParams.S2Pilot=PILOT_ON;
		break;
	default:
		m_ConfParams.S2Pilot=PILOT_NOT_SET;
	}
	switch(m_iRollOff)
	{
	case 1:
		m_ConfParams.S2RollOff = ROLLOFF_20;
		break;
	case 2:
		m_ConfParams.S2RollOff = ROLLOFF_25;
		break;
	case 3:
		m_ConfParams.S2RollOff = ROLLOFF_35;
		break;
	default:
		m_ConfParams.S2RollOff = ROLLOFF_NOT_SET;
	}
	m_ConfParams.MIS = m_ctrlMIS.GetCurSel()-1;
	m_ConfParams.RelockTimeout = m_ctrlRelockTime.GetCurSel();

	*m_pConfParams = m_ConfParams; 

	CDialog::OnOK();
}
