#include "stdafx.h"

#include "BdaGraph.h"
#include "tbsbda.h"
#include "qboxbda.h"

HRESULT CBdaGraph::DVBS_TBS_NXP_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	HRESULT hr;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
											KSPROPERTY_BDA_TBSACCESS, 
											&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_DISEQC;
		CopyMemory(cmd.uc_diseqc_send_message, DiSEqC_Command, len);
		cmd.uc_diseqc_send_message_length = len;

		// make call into driver
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_TBS_NXP_DiSEqC: failed sending KSPROPERTY_BDA_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_TBS_NXP_DiSEqC: sent KSPROPERTY_BDA_TBSACCESS");
		return S_OK;
	}

	TBSDISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
	CopyMemory(diseqc_msg.uc_diseqc_send_message, DiSEqC_Command, len);
	diseqc_msg.uc_diseqc_send_message_length = len;

	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&diseqc_msg, sizeof(diseqc_msg), &diseqc_msg, sizeof(diseqc_msg));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_TBS_NXP_DiSEqC: failed sending TBSDISEQC_MESSAGE_PARAMS (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_TBS_NXP_DiSEqC: sent TBSDISEQC_MESSAGE_PARAMS");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_TBS_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	HRESULT hr;

	ULONG bytesReturned = 0;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
											KSPROPERTY_BDA_TBSACCESS, 
											&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_DISEQC;
		CopyMemory(cmd.uc_diseqc_send_message, DiSEqC_Command, len);
		cmd.uc_diseqc_send_message_length = len;

		// make call into driver
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_TBS_DiSEqC: failed sending KSPROPERTY_BDA_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_TBS_DiSEqC: sent KSPROPERTY_BDA_TBSACCESS");
		return S_OK;
	}

	DISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
	CopyMemory(diseqc_msg.uc_diseqc_send_message, DiSEqC_Command, len);
	diseqc_msg.uc_diseqc_send_message_length = len;
	diseqc_msg.uc_diseqc_receive_message_length = 0;
	diseqc_msg.Tone_Data_Burst = PHANTOM_LNB_BURST_DISABLE;
	diseqc_msg.burst_tone = PHANTOM_LNB_BURST_MODULATED;
	diseqc_msg.tbscmd_mode = TBSDVBSCMD_MOTOR;
	diseqc_msg.receive_mode = PHANTOM_RXMODE_NOREPLY;
	diseqc_msg.b_last_message = TRUE;

	hr = m_pKsTunerPropSet->Get(KSPROPSETID_BdaTunerExtensionProperties,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&diseqc_msg, sizeof(diseqc_msg), &diseqc_msg, sizeof(diseqc_msg), &bytesReturned);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_TBS_DiSEqC: failed sending DISEQC_MESSAGE_PARAMS (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_TBS_DiSEqC: sent DISEQC_MESSAGE_PARAMS");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_TBS_Set22Khz(BOOL b22Khz)
{
	CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
	HRESULT hr;
	ULONG bytesReturned = 0;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
											KSPROPERTY_BDA_TBSACCESS, 
											&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_22K;
		cmd.tbs22k_mode = b22Khz ? TBSBURST_ON : TBSBURST_OFF;

		// make call into driver
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_TBS_Set22Khz: failed sending KSPROPERTY_BDA_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_TBS_Set22Khz: sent KSPROPERTY_BDA_TBSACCESS");
		return S_OK;
	}

	DISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
    diseqc_msg.tbscmd_mode = TBSDVBSCMD_22KTONEDATA;
	diseqc_msg.HZ_22K = b22Khz;

	hr = m_pKsTunerPropSet->Get(KSPROPSETID_BdaTunerExtensionProperties,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&diseqc_msg, sizeof(diseqc_msg), &diseqc_msg, sizeof(diseqc_msg), &bytesReturned);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_TBS_Set22Khz: failed sending DISEQC_MESSAGE_PARAMS (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_TBS_Set22Khz: sent DISEQC_MESSAGE_PARAMS");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_TBS_LNBPower(BOOL bPower)
{
	CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
	HRESULT hr;
	ULONG bytesReturned = 0;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
											KSPROPERTY_BDA_TBSACCESS, 
											&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_LNBPOWER;
		cmd.LNBPower_mode = bPower ? LNBPWR_ON : LNBPWR_OFF;

		// make call into driver
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_TBS_LNBPower: failed sending KSPROPERTY_BDA_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_TBS_LNBPower: sent KSPROPERTY_BDA_TBSACCESS");
		return S_OK;
	}

	DISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
    diseqc_msg.tbscmd_mode = TBSDVBSCMD_LNBPOWER;
    diseqc_msg.b_LNBPower = bPower;

	hr = m_pKsTunerPropSet->Get(KSPROPSETID_BdaTunerExtensionProperties,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&diseqc_msg, sizeof(diseqc_msg), &diseqc_msg, sizeof(diseqc_msg), &bytesReturned);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_TBS_LNBPower: failed set LNB Power state (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_TBS_LNBPower: set LNB Power state");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_QBOX_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);
	CheckPointer(DiSEqC_Command,E_POINTER);
	if (!len) return E_INVALIDARG;

	HRESULT hr;
	ULONG bytesReturned = 0;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
												KSPROPERTY_CTRL_TBSACCESS, 
												&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_DISEQC;
		CopyMemory(cmd.uc_diseqc_send_message, DiSEqC_Command, len);
		cmd.uc_diseqc_send_message_length = len;

		// make call into driver
		hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_QBOX_DiSEqC: failed sending KSPROPERTY_CTRL_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_QBOX_DiSEqC: sent KSPROPERTY_CTRL_TBSACCESS");
		return S_OK;
	}

	QBOXDVBSCMD QBOXCmd;
	if (len>_countof(QBOXCmd.motor)) return E_INVALIDARG;
	Z(QBOXCmd);
	CopyMemory(QBOXCmd.motor, DiSEqC_Command, len);

	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
		KSPROPERTY_CTRL_MOTOR,
		NULL, 0,
		&QBOXCmd, sizeof(QBOXCmd));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_QBOX_DiSEqC: failed sending KSPROPERTY_CTRL_MOTOR (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_QBOX_DiSEqC: sent KSPROPERTY_CTRL_MOTOR");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_QBOX_Set22Khz(BOOL b22Khz)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

	HRESULT hr;
	ULONG bytesReturned = 0;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
												KSPROPERTY_CTRL_TBSACCESS, 
												&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_22K;
		cmd.tbs22k_mode = b22Khz ? TBSBURST_ON : TBSBURST_OFF;

		// make call into driver
		hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_QBOX_Set22Khz: failed sending KSPROPERTY_CTRL_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_QBOX_Set22Khz: sent KSPROPERTY_CTRL_TBSACCESS");
		return S_OK;
	}

	QBOXDVBSCMD QBOXCmd;
    Z(QBOXCmd);
	QBOXCmd.HZ_22K = b22Khz;

	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
		KSPROPERTY_CTRL_22K_TONE,
		NULL, 0,
		&QBOXCmd, sizeof(QBOXCmd));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_QBOX_Set22Khz: failed sending KSPROPERTY_CTRL_22K_TONE (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_QBOX_Set22Khz: sent KSPROPERTY_CTRL_22K_TONE");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_QBOX_LNBPower(BOOL bPower)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);
	HRESULT hr;
	ULONG bytesReturned = 0;
	char text[256];

	DWORD type_support = 0;
	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
												KSPROPERTY_CTRL_TBSACCESS, 
												&type_support);

	if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
	{
		TBS_ACCESS_STRUCT cmd;
		Z(cmd);
		cmd.access_mode = TBSACCESS_LNBPOWER;
		cmd.LNBPower_mode = bPower ? LNBPWR_ON : LNBPWR_OFF;

		// make call into driver
		hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_TBSACCESS,
			NULL, 0,
			&cmd, sizeof(cmd));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_QBOX_Set22Khz: failed sending KSPROPERTY_CTRL_TBSACCESS (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_QBOX_Set22Khz: sent KSPROPERTY_CTRL_TBSACCESS");
		return S_OK;
	}

	QBOXDVBSCMD QBOXCmd;
    Z(QBOXCmd);
	QBOXCmd.LNB_POWER = bPower;
	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
		KSPROPERTY_CTRL_LNBPW,
		NULL, 0,
		&QBOXCmd, sizeof(QBOXCmd));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_QBOX_LNBPower: failed set LNB Power state (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_QBOX_LNBPower: set LNB Power state");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_TBS_SetPol(Polarisation ePol)
{
	HRESULT hr;
	DWORD type_support = 0;

	TBS_ACCESS_STRUCT cmd;
	cmd.access_mode = TBSACCESS_LNBPOWER;
	switch (ePol)
	{
	case BDA_POLARISATION_LINEAR_H:
	case BDA_POLARISATION_CIRCULAR_L:
		cmd.LNBPower_mode=LNBPWR_18V;
		break;
	case BDA_POLARISATION_LINEAR_V:
	case BDA_POLARISATION_CIRCULAR_R:
		cmd.LNBPower_mode=LNBPWR_13V;
		break;
	case BDA_POLARISATION_NOT_DEFINED:
		cmd.LNBPower_mode=LNBPWR_OFF;
		break;
	case BDA_POLARISATION_NOT_SET:
	default:
		cmd.LNBPower_mode=LNBPWR_ON;
	}

	if (BdaType==QBOX_BDA)
	{
		CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_TBSACCESS, 
			&type_support);

		if (SUCCEEDED(hr) && (type_support&KSPROPERTY_SUPPORT_SET))
		{
			return m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
												KSPROPERTY_CTRL_TBSACCESS,
												NULL, 0,
												&cmd, sizeof(cmd));
		}
	} else
	{
		CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_TBSACCESS, 
			&type_support);
		
		if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
		{
			// make call into driver
			return m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
				KSPROPERTY_BDA_TBSACCESS,
				NULL, 0,
				&cmd, sizeof(cmd));
		}
	}
	return E_NOTIMPL;
}

HRESULT CBdaGraph::DVBS_TBS_SetMIS(LONG mis)
{
	HRESULT hr;
	DWORD type_support = 0;

	MEDIAINFO cmd;
	cmd.StreamIdentify = mis;

	if (BdaType==QBOX_BDA)
	{
		CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_INPUTMULTISTREAMID, 
			&type_support);

		if (SUCCEEDED(hr) && (type_support&KSPROPERTY_SUPPORT_SET))
		{
			return m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
												KSPROPERTY_CTRL_INPUTMULTISTREAMID,
												NULL, 0,
												&cmd, sizeof(cmd));
		}
	} else
	{
		CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_INPUTMULTISTREAMID, 
			&type_support);
		
		if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
		{
			// make call into driver
			return m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
				KSPROPERTY_BDA_INPUTMULTISTREAMID,
				&cmd, sizeof(cmd),
				&cmd, sizeof(cmd));
		}
	}
	return E_NOTIMPL;
}

HRESULT CBdaGraph::DVBS_TBS_SetPLS( UINT PLSMode, DWORD PLSCode )
{
	HRESULT hr;
	DWORD type_support = 0;

	PLS_INFO cmd;
	cmd.PlsMode = PLSMode;
	cmd.PlsCode = PLSCode;

	if (BdaType==QBOX_BDA)
	{
		CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_PLS, 
			&type_support);

		if (SUCCEEDED(hr) && (type_support&KSPROPERTY_SUPPORT_SET))
		{
			return m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_QBOXControl,
												KSPROPERTY_CTRL_PLS,
												NULL, 0,
												&cmd, sizeof(cmd));
		}
	} else
	{
		CheckPointer(m_pKsTunerPropSet,E_NOINTERFACE);
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_PLS, 
			&type_support);
		
		if (SUCCEEDED(hr) && (type_support & KSPROPERTY_SUPPORT_SET))
		{
			// make call into driver
			return m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
				KSPROPERTY_BDA_PLS, 
				&cmd, sizeof(cmd),
				&cmd, sizeof(cmd));
		}
	}
	return E_NOTIMPL;
}

