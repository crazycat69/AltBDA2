#include "stdafx.h"

#include "BdaGraph.h"
#include "amtbda.h"

HRESULT CBdaGraph::DVBT_Astrometa_SetPLP(LONG Plp)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_AMDVB_Ter,
		KSPROPERTY_AMDVB_PLPINFO, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		PLP_Info plp_info;
		memset(&plp_info,0, sizeof(plp_info));
		plp_info.current_PLP_ID = 0;
		plp_info.current_PLP_No = Plp;

		hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_AMDVB_Ter,
			KSPROPERTY_AMDVB_PLPINFO,
			NULL, 0,
			&plp_info, sizeof(plp_info));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBT_Astrometa_SetPLP: failed set PLP filter (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBT_Astrometa_SetPLP: set PLP filter");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBT_Astrometa_SetPLP: KSPROPERTY_AMDVB_PLPINFO not supported !");
	}

	return hr;
}

HRESULT CBdaGraph::DVBT_Astrometa_SetMode(DWORD Mode)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_POINTER);
	
	char text[256];
	HRESULT hr=S_OK;
	DWORD supported;

	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_AMDVB_Ctl,
		KSPROPERTY_AMDVB_DELSYS, &supported);

	DWORD submode = Mode == RM_FMDAB ? 0 : 6;

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		DWORD mode = 0;
		hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_AMDVB_Ctl,
			KSPROPERTY_AMDVB_DELSYS,
			NULL, 0,
			&submode, sizeof(submode));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBT_Astrometa_SetMode: failed set submode (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBT_Astrometa_SetMode: set submode");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBT_Astrometa_SetMode: KSPROPERTY_AMDVB_DELSYS not supported !");
	}
	Sleep(500);

	if (!submode) return hr;

	hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_AMDVB_Ter,
		KSPROPERTY_AMDVB_TERMODE, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		hr = m_pKsTunerFilterPropSet->Set(KSPROPSETID_AMDVB_Ter,
			KSPROPERTY_AMDVB_TERMODE,
			NULL, 0,
			&Mode, sizeof(Mode));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBT_Astrometa_SetMode: failed set mode (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBT_Astrometa_SetMode: set mode");
		ReportMessage(text);
	}
	else
	{
		hr = E_NOTIMPL;
		DebugLog("BDA2: DVBT_Astrometa_SetMode: KSPROPERTY_AMDVB_TERMODE not supported !");
	}

	return hr;
}
