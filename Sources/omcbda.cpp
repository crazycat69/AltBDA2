#include "stdafx.h"

#include "BdaGraph.h"

HRESULT CBdaGraph::DVBS_OMC2_Set22Khz(BOOL b22Khz)
{
    CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);

   char text[256];
   HRESULT hr=S_OK;
   DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcDiSEqCProperties,
		KSPROPERTY_OMC_DISEQC_SET22K, &supported);

	if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcDiSEqCProperties,
			KSPROPERTY_OMC_DISEQC_SET22K,
			NULL, 0,
			&b22Khz, sizeof(b22Khz));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_Set22Khz: failed sending KSPROPERTY_OMC_DISEQC_SET22K (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_Set22Khz: sent KSPROPERTY_OMC_DISEQC_SET22K");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_Set22Khz: KSPROPERTY_OMC_DISEQC_SET22K not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_ToneBurst(BOOL bToneBurst)
{
    CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
   
   char text[256];
   HRESULT hr=S_OK;
   DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcDiSEqCProperties,
		KSPROPERTY_OMC_DISEQC_TONEBURST, &supported);

	if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcDiSEqCProperties,
			KSPROPERTY_OMC_DISEQC_TONEBURST,
			NULL, 0,
			&bToneBurst, sizeof(bToneBurst));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_ToneBurst: failed sending KSPROPERTY_OMC_DISEQC_TONEBURST (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_ToneBurst: sent KSPROPERTY_OMC_DISEQC_TONEBURST");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_ToneBurst: KSPROPERTY_OMC_DISEQC_TONEBURST not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
    CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	CheckPointer(DiSEqC_Command,E_INVALIDARG);

	OMC_BDA_DISEQC_DATA diseqc_data;
	if ( (len==0) || (len>sizeof(diseqc_data.pBuffer)) )
		return E_INVALIDARG;

	CopyMemory(diseqc_data.pBuffer,DiSEqC_Command,len);
	diseqc_data.nLen = len;
	diseqc_data.nRepeatCount = 0;

	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcDiSEqCProperties,
		KSPROPERTY_OMC_DISEQC_WRITE, &supported);

	if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcDiSEqCProperties,
			KSPROPERTY_OMC_DISEQC_WRITE,
			NULL, 0,
			&diseqc_data, sizeof(diseqc_data));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_DiSEqC: failed sending KSPROPERTY_OMC_DISEQC_WRITE (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_DiSEqC: sent KSPROPERTY_OMC_DISEQC_WRITE");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_DiSEqC: KSPROPERTY_OMC_DISEQC_WRITE not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_SetMIS(LONG mis)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcCustomProperties,
		KSPROPERTY_OMC_CUSTOM_MIS_FILTER, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcCustomProperties,
			KSPROPERTY_OMC_CUSTOM_MIS_FILTER,
			NULL, 0,
			&mis, sizeof(mis));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_SetMIS: failed set MIS filter (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_SetMIS: set MIS filter");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_SetMIS: KSPROPERTY_OMC_CUSTOM_MIS_FILTER not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_SetPLS( UINT PLSMode, DWORD PLSCode )
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

    hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcCustomProperties,
					   KSPROPERTY_OMC_CUSTOM_PLS_SCRAM, 
					   &supported);

	if (SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET))
	{
		OMC_BDA_PLS_PARAM param = { PLSMode, PLSCode };
		// make call into driver
		hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcCustomProperties,
			KSPROPERTY_OMC_CUSTOM_PLS_SCRAM,
			NULL, 0,
			&param, sizeof(param));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_SetPLS: failed set PLS code (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_SetPLS: set PLS code");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_SetPLS: KSPROPERTY_OMC_CUSTOM_PLS_SCRAM not supported !");
	}

	return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_SetSearchMode(OMC_BDA_SEARCH_MODE SearchMode)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcCustomProperties,
		KSPROPERTY_OMC_CUSTOM_SEARCH_MODE, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcCustomProperties,
			KSPROPERTY_OMC_CUSTOM_SEARCH_MODE,
			NULL, 0,
			&SearchMode, sizeof(SearchMode));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_SetSearchMode: failed set search mode (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_SetSearchMode: set search mode");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_SetSearchMode: KSPROPERTY_OMC_CUSTOM_SEARCH_MODE not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_SetSearchRange(ULONG ulSearchRange)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcCustomProperties,
		KSPROPERTY_OMC_CUSTOM_SEARCH_RANGE, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_OmcCustomProperties,
			KSPROPERTY_OMC_CUSTOM_SEARCH_RANGE,
			NULL, 0,
			&ulSearchRange, sizeof(ulSearchRange));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_SetSearchRange: failed set search range (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_SetSearchRange: set search range");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_SetSearchRange: KSPROPERTY_OMC_CUSTOM_SEARCH_RANGE not supported !");
	}

   return hr;
}

HRESULT CBdaGraph::DVBS_OMC2_Search( ULONG LOF1,
									 ULONG LOF2,
									 ULONG LOFSW,
									 ULONG Freq,
									 ULONG SR,
									 ULONG SearchRange,
									 OMC_BDA_SEARCH_MODE SearchMode,
									 Polarisation Pol,
									 ModulationType Mod,
									 BinaryConvolutionCodeRate Fec)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;
	ULONG bytesReturned = 0;

	OMC_BDA_SEARCH_PARAMS params;
	params.LofLow = LOF1;
	params.LofHigh = LOF2;
	params.LofSwitch = LOFSW;
	params.SR = SR;
	params.SearchMode = SearchMode;
	params.SearchRange = SearchRange;
	params.Freq = Freq;
	params.Pol = Pol;
	params.Mod = Mod;
	params.Fec = Fec;

	OMC_BDA_SEARCH_RESULTS results;

  	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcCustomProperties,
		KSPROPERTY_OMC_CUSTOM_SEARCH, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_GET) )
	{
	  	hr = m_pKsTunerFilterPropSet->Get(KSPROPSETID_OmcCustomProperties,
			KSPROPERTY_OMC_CUSTOM_SEARCH,
			&params, sizeof(params),
			&results, sizeof(results),
			&bytesReturned);
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_OMC2_Search: failed set/get search (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_OMC2_Search: search carrier");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBS_OMC2_Search: KSPROPERTY_OMC_CUSTOM_SEARCH_MODE not supported !");
	}

   return hr;
}
