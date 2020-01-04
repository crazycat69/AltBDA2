#include "stdafx.h"

#include "Configuration.h"

#include "ConfDialog.h"

CConfiguration::CConfiguration()
{
	message_callback = NULL;
	conf_params.VendorSpecific = VENDOR_SPECIFIC(PURE_BDA);
}

CConfiguration::~CConfiguration()
{
}

void CConfiguration::MessageCallback(MSG_CB_PROC callback)
{
	message_callback = callback;
}

void CConfiguration::ReportMessage(char *text)
{
	if(message_callback)
	{
		struct MESSAGE_CALLBACK_DATA d;

		d.h = 0x0;
		d.message = text;

		message_callback(MESSAGE_CALLBACK_ID2, (char *)&d);
	}
}

void CConfiguration::Configure(HINSTANCE hInstance)
{
	if(!ReadConfigurationFile())
	{
		DebugLog("BDA2: DLLMain: Failed getting configuration file");
		if(!CreateConfigurationFile())
			DebugLog("BDA2: DLLMain: Failed creating default configuration file");
		else
			DebugLog("BDA2: DLLMain: Created default configuration file");
	}
}

void CConfiguration::ConfCaps()
{
	switch(conf_params.VendorSpecific)
	{
	case TT_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8VSB;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_COMMITED | DISEQC_RAW;
		ReportMessage("Technotrend BDA API used !");
		break;
	case TH_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8VSB;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_COMMITED | DISEQC_RAW;
		ReportMessage("Twinhan BDA extension used !");
		break;
	case DW_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_COMMITED | DISEQC_RAW;
		ReportMessage("DvbWorld BDA extension used !");
		break;
	case HAUP_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Hauppauge BDA extension used !");
		break;
	case CXT_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Conexant BDA extension used !");
		break;
	case BST_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Bestunar BDA extension used !");
		break;
	case TBS_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Turbosight-Conexant BDA extension used !");
		break;
	case TBS_NXP_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Turbosight-NXP BDA extension used !");
		break;
	case QBOX_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_NBC_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Turbosight-QBOX BDA extension used !");
		break;
	case TV_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("TeVii BDA extension used !");
		break;
	case OMC_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_RAW;
		ReportMessage("Omicom BDA extension used !");
		break;
	case COMPRO_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Compro BDA extension used !");
		break;
	case ANYSEE_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_RAW;
		ReportMessage("AnySee BDA extension used !");
		break;
	case GNP_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_RAW;
		ReportMessage("Genpix 3dparty BDA extension used !");
		break;
	case GENPIX_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_RAW;
		ReportMessage("Genpix BDA extension used !");
		break;
	case NETUP_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_RAW;
		ReportMessage("Netup BDA extension used !");
		break;
	case CRAZY_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_RAW;
		ReportMessage("Crazy BDA extension used !");
		break;
	case MS_BDA:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_TONEBURST | DISEQC_COMMITED | DISEQC_RAW;
		ReportMessage("Microsoft BDA extension used !");
		break;
	case PURE_BDA:
	default:
		conf_params.ConfMod8PSK = BDA_MOD_8PSK;
		conf_params.ConfDiSEqC = DISEQC_COMMITED;
		ReportMessage("Generic BDA used + DiseqC 1.0 (via InputRange)!");
	}
}

BOOLEAN CConfiguration::ReadConfigurationFile()
{
	DWORD junk;
	int fv_len = GetFileVersionInfoSizeA(AfxGetApp()->m_pszExeName, &junk);
	if(fv_len)
	{
		BYTE *fv_data = new BYTE[fv_len];
		BYTE *ptr;
		UINT txt_len;

		GetFileVersionInfoA(AfxGetApp()->m_pszExeName, junk, fv_len, (void *)fv_data);
		if(VerQueryValueA(fv_data,"\\",(void **)&ptr,&txt_len))
		{
			VS_FIXEDFILEINFO *vs = (VS_FIXEDFILEINFO *)ptr;
			sprintf(conf_params.ConfVer, "v%d.%d.%d.%d",
				HIWORD(vs->dwFileVersionMS),
				LOWORD(vs->dwFileVersionMS),
				HIWORD(vs->dwFileVersionLS),
				LOWORD(vs->dwFileVersionLS));
		}
		free(fv_data);
	}
	else
		sprintf(conf_params.ConfVer,"Unknown");

	CString str = AfxGetApp()->GetProfileString("Common", "BDA_TYPE", "NOT_SET");

	if ( !str.CompareNoCase("MS") || !str.CompareNoCase("MICROSOFT") || !str.CompareNoCase("WIN7") )
		conf_params.VendorSpecific = MS_BDA;
	if ( !str.CompareNoCase("TV") || !str.CompareNoCase("TEVII") )
		conf_params.VendorSpecific = TV_BDA;

	str = AfxGetApp()->GetProfileString("Dev_Bda2Driver DVBS", "S2_ROLLOFF", "NOT_SET");

	DebugLog("BDA2: ReadConfigurationFile: S2_ROLLOFF = %s (default is NOT_SET)", str);
	if(!str.CompareNoCase("20"))
		conf_params.S2RollOff = ROLLOFF_20;
	else
	if(!str.CompareNoCase("25"))
		conf_params.S2RollOff = ROLLOFF_25;
	else
	if(!str.CompareNoCase("35"))
		conf_params.S2RollOff = ROLLOFF_35;
	else
		conf_params.S2RollOff = ROLLOFF_NOT_SET; // default

	str = AfxGetApp()->GetProfileString("Dev_Bda2Driver DVBS", "S2_PILOT", "NOT_SET");

	DebugLog("BDA2: ReadConfigurationFile: S2_PILOT = %s (default is NOT_SET)", str);
	if(!str.CompareNoCase("ON"))
		conf_params.S2Pilot = PILOT_ON;
	else
	if(!str.CompareNoCase("OFF"))
		conf_params.S2Pilot = PILOT_OFF;
	else
		conf_params.S2Pilot = PILOT_NOT_SET; // default

	conf_params.RelockTimeout = AfxGetApp()->GetProfileInt("Common","RelockTime",0);
	if (conf_params.RelockTimeout > MAX_TIMEOUT) conf_params.RelockTimeout = MAX_TIMEOUT;
	DebugLog("BDA2: ReadConfigurationFile: RelockTime = %i sec (default is 0, disabled)", conf_params.RelockTimeout);

	conf_params.MIS = AfxGetApp()->GetProfileInt("Common","MIS",-1);

	HANDLE hCfg;
	hCfg = CreateFile(AfxGetApp()->m_pszProfileName, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
	CloseHandle(hCfg);
	return hCfg != INVALID_HANDLE_VALUE;
}


BOOLEAN CConfiguration::CreateConfigurationFile()
{
	FILE *fp;

	fp = fopen(AfxGetApp()->m_pszProfileName, "w");
	if(fp)
	{
		fputs("; Dev_Bda2Driver.int configuration parameters\n\n",fp);
		fputs("[Common]\n\n",fp);
		fputs(";Preffered BDA extension\n",fp);
		fputs(";   NOT_SET (default)\n",fp);
		fputs(";   MS - Microsoft Win7 BDA extension\n",fp);
		fputs(";   TV - TeVii BDA API\n",fp);
		fputs("BDA_TYPE = NOT_SET\n\n",fp);
		fprintf(fp,";Relock after timeout in sec (1-%d)\n",MAX_TIMEOUT);
		fputs("RelockTime = 0\n\n",fp);
		fputs("[Dev_Bda2Driver DVBS]\n\n",fp);
		fputs(";S2 Roll Off\n",fp);
		fputs(";   NOT_SET (default)\n",fp);
		fputs(";   20 - 0.20\n",fp);
		fputs(";   25 - 0.25\n",fp);
		fputs(";   35 - 0.35\n",fp);
		fputs("S2_ROLLOFF = NOT_SET\n\n",fp);
		fputs(";S2 Pilot\n",fp);
		fputs(";   NOT_SET (default)\n",fp);
		fputs(";   ON\n",fp);
		fputs(";   OFF\n",fp);
		fputs("S2_PILOT = NOT_SET\n\n",fp);
		fputs("[Dev_Bda2Driver DVBT]\n\n",fp);
		fputs("[Dev_Bda2Driver DVBC]\n\n",fp);
		fclose(fp);
		return TRUE;
	}
	return FALSE;
}

BOOLEAN CConfiguration::DoConfigurationDialog()
{
	ConfDialog CfgDlg(CWnd::FindWindow("TFormDvbMain",NULL),&conf_params);
	if (CfgDlg.DoModal()==IDOK)
	{
		char *ConfStr;
		switch(conf_params.S2Pilot)
		{
		case PILOT_ON:
			ConfStr="ON";
			break;
		case PILOT_OFF:
			ConfStr="OFF";
			break;
		case PILOT_NOT_SET:
		default:
			ConfStr="NOT_SET";
		}
		AfxGetApp()->WriteProfileString("Dev_Bda2Driver DVBS", "S2_PILOT", ConfStr);

		switch(conf_params.S2RollOff)
		{
		case ROLLOFF_20:
			ConfStr="20";
			break;
		case ROLLOFF_25:
			ConfStr="25";
			break;
		case ROLLOFF_35:
			ConfStr="35";
			break;
		case ROLLOFF_NOT_SET:
		default:
			ConfStr="NOT_SET";
		}
		AfxGetApp()->WriteProfileString("Dev_Bda2Driver DVBS", "S2_ROLLOFF", ConfStr);

		AfxGetApp()->WriteProfileInt("Common","RelockTime",conf_params.RelockTimeout);

		AfxGetApp()->WriteProfileInt("Common","MIS",conf_params.MIS);
	}
	return TRUE;
}
