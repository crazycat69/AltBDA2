#include "stdafx.h"
#include "BdaGraph.h"
#include "cmpbda.h"

HRESULT CBdaGraph::DVBS_Compro_LNBPower(BOOL bPower)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(VAMP_DVBS_DISEQC_ACCESS_PROPERTY,
		VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL, &supported);

	if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
	{
		bPower = bPower ? POWER_ON : POWER_OFF;
	  	hr = m_pKsTunerFilterPropSet->Set(VAMP_DVBS_DISEQC_ACCESS_PROPERTY,
			VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL,
			NULL, 0,
			&bPower, sizeof(bPower));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_Compro_LNBPower: failed sending VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_Compro_LNBPower: sent VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_Compro_LNBPower: VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_Compro_Set22Khz(BOOL b22Khz)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

	hr = m_pKsTunerFilterPropSet->QuerySupported(VAMP_DVBS_DISEQC_ACCESS_PROPERTY,
		VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL, &supported);

	if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
	{
		b22Khz = b22Khz ? L22KHZ_TONE_ON : L22KHZ_TONE_OFF;
	  	hr = m_pKsTunerFilterPropSet->Set(VAMP_DVBS_DISEQC_ACCESS_PROPERTY,
			VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL,
			NULL, 0,
			&b22Khz, sizeof(b22Khz));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_Compro_Set22Khz: failed sending VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_Compro_Set22Khz: sent VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_Compro_Set22Khz: VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_Compro_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);
	CheckPointer(DiSEqC_Command,E_INVALIDARG);
	if (len>8) return E_INVALIDARG;

	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(VAMP_DVBS_DISEQC_ACCESS_PROPERTY,
		VAMP_DVBS_DISEQC_PROP_ID_RAW_COMMAND, &supported);

	if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
	{
		DWORD cmd_len = len;
		BYTE cmd[8];
		memcpy(cmd,DiSEqC_Command,len);
	  	hr = m_pKsTunerFilterPropSet->Set(VAMP_DVBS_DISEQC_ACCESS_PROPERTY,
			VAMP_DVBS_DISEQC_PROP_ID_RAW_COMMAND,
			&cmd_len, sizeof(cmd_len) + sizeof(KSPROPERTY),
			&cmd, sizeof(cmd));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_Compro_DiSEqC: failed sending VAMP_DVBS_DISEQC_PROP_ID_RAW_COMMAND (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_Compro_DiSEqC: sent VAMP_DVBS_DISEQC_PROP_ID_RAW_COMMAND");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_Compro_DiSEqC: VAMP_DVBS_DISEQC_PROP_ID_RAW_COMMAND not supported !");
	}

   return hr;
}
