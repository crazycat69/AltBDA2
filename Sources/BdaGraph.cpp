#include "stdafx.h"

#include "BdaGraph.h"
#include "cxtbda.h"
#include "qboxbda.h"
#include "omcbda.h"
#include "tvbda.h"
#include "cmpbda.h"
#include "asbda.h"
#include "gnpbda.h"
#include "netup_bda_api.h"
#include "CrazyBDA.h"
#include "amtbda.h"

CBdaGraph::CBdaGraph()
{
	message_callback = NULL;
	m_dwGraphRegister = 0;
	m_pFilterGraph = NULL;
	m_pTunerDevice = NULL;
	m_pNetworkProvider = NULL;
	m_pReceiver = NULL;
	m_pCallbackFilter = NULL;
	m_pKsTunerFilterPropSet = NULL;
	m_pKsVCPropSet = NULL;
	m_pKsTunerPropSet = NULL;
	m_pKsDemodPropSet = NULL;
	m_pKsCaptureFilterPropSet = NULL;
	m_pMediaControl = NULL;
	pCallbackInstance = NULL;
	pNetworkProviderInstance = NULL;
	hTT = hDW =  INVALID_HANDLE_VALUE;
	CoInitializeEx(0,COINIT_APARTMENTTHREADED);
}

CBdaGraph::~CBdaGraph()
{
}

HRESULT CBdaGraph::GetNetworkTuners(struct NetworkTuners *Tuners)
{
	Tuners->Count = 0;

	// Create the System Device Enumerator.
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
	{
		DebugLog("BDA2: DeviceDescription: Failed creating Device Enumerator");
		return hr;
	}

	// Obtain a class enumerator for the BDA tuner category.
	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(KSCATEGORY_BDA_NETWORK_TUNER, &pEnumCat, 0);

	if (hr == S_OK) 
	{
		// Enumerate the monikers.
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		int dev_enum_i = 0;

		while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK && Tuners->Count < 8)
		{
			IPropertyBag *pPropBag;
			hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
			if (SUCCEEDED(hr))
			{
				++dev_enum_i;
				// To retrieve the filter's friendly name, do the following:
				VARIANT varName;
				VariantInit(&varName);
				hr = pPropBag->Read(L"FriendlyName", &varName, 0);
				if (SUCCEEDED(hr))
				{
					// Display the name in your UI somehow.
					BSTR bStr = varName.bstrVal;
					WideCharToMultiByte(CP_ACP, 0, bStr, -1, Tuners->Tuner[Tuners->Count].Name, sizeof(Tuners->Tuner[Tuners->Count].Name), NULL, NULL);

					// check the BDA tuner type and availability
					// To create an instance of the filter, do the following:
					IBaseFilter *pFilter;
					hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&pFilter);
					if(SUCCEEDED(hr))
					{
						GUID type;

						hr = GetNetworkTunerType(pFilter, &type);
						if(SUCCEEDED(hr))
						{
							Tuners->Tuner[Tuners->Count].Id = dev_enum_i;
							Tuners->Tuner[Tuners->Count].Type = type;
							Tuners->Tuner[Tuners->Count].Availability = TRUE; // TODO - check availability
						}
						else
							DebugLog("BDA2: DeviceDescription: Failed getting network tuner type");

						char* tuner_name = Tuners->Tuner[Tuners->Count].Name;
						if ( strstr(tuner_name, "DVBLink Tuner" ) )
						{
							//Tuners->Tuner[Tuners->Count].Type = CLSID_NetworkProvider;
							Tuners->Tuner[Tuners->Count].Availability = FALSE;
						}
						
						// Technotrend check
						TTDevCat=UNKNOWN;
						if ( strstr(tuner_name, BDG2_NAME_S_TUNER) ||
							strstr(tuner_name, BDG2_NAME_C_TUNER) ||
							strstr(tuner_name, BDG2_NAME_T_TUNER) ||
							strstr(tuner_name, BDG2_NAME_S_TUNER_FAKE) ||
							strstr(tuner_name, BDG2_NAME_C_TUNER_NEW) ||
							strstr(tuner_name, BDG2_NAME_S_TUNER_NEW) ||
							strstr(tuner_name, BDG2_NAME_T_TUNER_NEW) )
							TTDevCat=BUDGET_2;
						if ( strstr(tuner_name, BUDGET3NAME_TUNER) ||
							strstr(tuner_name,  BUDGET3NAME_ATSC_TUNER) )
							TTDevCat=BUDGET_3;
						if ( strstr(tuner_name, USB2BDA_DVB_NAME_S_TUNER) ||
							strstr(tuner_name, USB2BDA_DVB_NAME_C_TUNER) ||
							strstr(tuner_name, USB2BDA_DVB_NAME_T_TUNER) ||
							strstr(tuner_name, USB2BDA_DVB_NAME_S_TUNER_FAKE) )
							TTDevCat=USB_2;
						if ( strstr(tuner_name, USB2BDA_DVBS_NAME_PIN_TUNER) )
							TTDevCat=USB_2_PINNACLE;
						if ( strstr(tuner_name, USB2BDA_DSS_NAME_TUNER) )
							TTDevCat=USB_2_DSS;
						if ( strstr(tuner_name, PREMIUM_NAME_DIGITAL_TUNER) )
							TTDevCat=PREMIUM;

						IPin* pPin = GetOutPin(pFilter, 0);
						if ( pPin && (TTDevCat!=UNKNOWN) && (TTDevCat!=USB_2_PINNACLE) )
						{
							DebugLog("BDA2: DeviceDescription: Check Technotrend device info");
							REGPINMEDIUM Medium;
							memset(&Medium, 0, sizeof(Medium));
							if (GetPinMedium(pPin, &Medium) == S_OK)
							{
								hTT = bdaapiOpenHWIdx(TTDevCat,Medium.dw1);
								if (INVALID_HANDLE_VALUE!=hTT)
								{
									if (!DVBS_TT_GetProdName(tuner_name,sizeof(Tuners->Tuner[Tuners->Count].Name)))
										DebugLog("BDA2: DeviceDescription: Can't get Technotrend product name");
									bdaapiClose (hTT);
									hTT = INVALID_HANDLE_VALUE;
								}
								else
									DebugLog("BDA2: DeviceDescription: Can't open Technotrend device via TT BDA API");
							}
							else
								DebugLog("BDA2: DeviceDescription: Can't get Tuner pin medium");
						}
						pFilter->Release();
						Tuners->Count += 1;
					}
					else
						DebugLog("BDA2: DeviceDescription: Failed binding moniker to object");
					VariantClear(&varName);
					pPropBag->Release();
				}
				else
					DebugLog("BDA2: DeviceDescription: Failed getting FriendlyName of the BDA_NETWORK_TUNER");
				pMoniker->Release();
			}
			else
				DebugLog("BDA2: DeviceDescription: Failed binding PropertyBag to storage");
		}
		pEnumCat->Release();
		pSysDevEnum->Release();
	}
	else
		DebugLog("BDA2: DeviceDescription: Failed creating KSCATEGORY_BDA_NETWORK_TUNER class enumerator");
	return hr;
}

BOOL CBdaGraph::DVBS_TT_GetProdName( char* pszProdName, size_t len )
{
	if (INVALID_HANDLE_VALUE==hTT)
		return FALSE;
	if (TTDevCat==USB_2_PINNACLE)
		return FALSE;
	TS_FilterNames TTInfo;
	TYPE_RET_VAL rc = bdaapiGetDevNameAndFEType (hTT, &TTInfo);
	if (RET_SUCCESS != rc)
		return FALSE;
	strncpy(pszProdName, TTInfo.szProductName, len);
	return TRUE;
}

HRESULT CBdaGraph::GetTunerPath(int idx, char* pTunerPath)
{
	// Create the System Device Enumerator.
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
	{
		DebugLog("BDA2: GetTunerPath: Failed creating Device Enumerator");
		return hr;
	}

	// Obtain a class enumerator for the BDA tuner category.
	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(KSCATEGORY_BDA_NETWORK_TUNER, &pEnumCat, 0);

	if (hr == S_OK) 
	{
		// Enumerate the monikers.
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		int i = 0;

		while((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (i < 8))
		{
			++i;
			if (i==idx)
			{
				LPOLESTR pszName;
				hr = pMoniker->GetDisplayName(NULL, NULL, &pszName);
				WideCharToMultiByte(CP_ACP, 0, pszName, -1, pTunerPath, MAX_PATH, 0, 0);
				CoTaskMemFree((void*)pszName);
				pMoniker->Release();
				break;
			}			
		}
		pEnumCat->Release();
	}
	else
		DebugLog("BDA2: GetTunerPath: Failed creating KSCATEGORY_BDA_NETWORK_TUNER class enumerator");

	pSysDevEnum->Release();
	return hr;
}

HRESULT CBdaGraph::GetNetworkTunerType(IBaseFilter *pFilter, GUID *type)
{
	BDANODE_DESCRIPTOR descriptors[20];
	IBDA_Topology *pTopology;
	ULONG cnt;
	HRESULT hr;

	*type = CLSID_NULL;

	hr = pFilter->QueryInterface(IID_IBDA_Topology, (void **)&pTopology);
	if(SUCCEEDED(hr))
	{
		hr = pTopology->GetNodeDescriptors(&cnt, 20, descriptors);
		if(SUCCEEDED(hr))
		{
			for(unsigned i=0; i<cnt; ++i)
			{
				if(IsEqualGUID(descriptors[i].guidFunction, KSNODE_BDA_QPSK_DEMODULATOR))
				{
					*type = CLSID_DVBSNetworkProvider;
					break;
				}
				else
				if(IsEqualGUID(descriptors[i].guidFunction, KSNODE_BDA_COFDM_DEMODULATOR))
				{
					*type = CLSID_DVBTNetworkProvider;
					break;
				}
				else
				if(IsEqualGUID(descriptors[i].guidFunction, KSNODE_BDA_QAM_DEMODULATOR))
				{
					*type = CLSID_DVBCNetworkProvider;
					break;
				}
				else
				if(IsEqualGUID(descriptors[i].guidFunction, KSNODE_BDA_8VSB_DEMODULATOR))
				{
					*type = CLSID_ATSCNetworkProvider;
					break;
				}
			}
		}
		else
			DebugLog("BDA2: GetNetworkTunerType: Failed getting node descriptors");
	}
	else
		DebugLog("BDA2: GetNetworkTunerType: Failed getting IBDA_Topology");

	return hr;
}

HRESULT CBdaGraph::FindTunerFilter(int dev_no, BSTR *bStr, IBaseFilter **pFilter)
{
	// Create the System Device Enumerator.
	HRESULT hr;
	ICreateDevEnum *pSysDevEnum = NULL;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum, (void **)&pSysDevEnum);
	if (FAILED(hr))
	{
		DebugLog("BDA2: FindTunerFilter: Failed creating SystemDeviceEnum");
		return hr;
	}
	// Obtain a class enumerator for the BDA tuner category.
	IEnumMoniker *pEnumCat = NULL;
	hr = pSysDevEnum->CreateClassEnumerator(KSCATEGORY_BDA_NETWORK_TUNER, &pEnumCat, 0);

	if (hr == S_OK) 
	{
		// Enumerate the monikers.
		IMoniker *pMoniker = NULL;
		ULONG cFetched;
		int dev_enum_i = 0;
		while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
		{
			++dev_enum_i;
			if(dev_no == dev_enum_i)
			{
				IPropertyBag *pPropBag;
				hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
				if (SUCCEEDED(hr))
				{
					// To retrieve the filter's friendly name, do the following:
					VARIANT varName;
					VariantInit(&varName);
					hr = pPropBag->Read(L"FriendlyName", &varName, 0);
					if (SUCCEEDED(hr))
					{
						// Display the name in your UI somehow.
						*bStr = SysAllocString(varName.bstrVal);
						VariantClear(&varName);

						pPropBag->Release();
						hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)pFilter);
						if(FAILED(hr))
							DebugLog("BDA2: FindTunerFilter: Failed binding moniker to object");
					}
					else
						DebugLog("BDA2: FindTunerFilter: Failed reading FriendlyName");
				}
				else
					DebugLog("BDA2: FindTunerFilter: Failed binding PropertyBag to storage");
			}
			pMoniker->Release();
		}
		pEnumCat->Release();
	}
	else
		DebugLog("BDA2: FindTunerFilter: Failed creating KSCATEGORY_BDA_NETWORK_TUNER class enumerator");
	pSysDevEnum->Release();
	return hr;
}

HRESULT CBdaGraph::AttachFilter(
    REFCLSID clsid, 
    IBaseFilter** ppFilter,
	PCHAR szMatchName
    )
{
    HRESULT                 hr = S_OK;
    BOOL                    fFoundFilter = FALSE;
    CComPtr <IMoniker>      pIMoniker;
    CComPtr <IEnumMoniker>  pIEnumMoniker;

	CComPtr <ICreateDevEnum> m_pICreateDevEnum = NULL;
	hr = m_pICreateDevEnum.CoCreateInstance(CLSID_SystemDeviceEnum);
	if FAILED(hr)
	{
		DebugLog("CBdaGraph::AttachFilter","Cannot CoCreate ICreateDevEnum");
		return hr;
	}

    // obtain the enumerator
    hr = m_pICreateDevEnum->CreateClassEnumerator(clsid, &pIEnumMoniker, 0);
    // the call can return S_FALSE if no moniker exists, so explicitly check S_OK
    if FAILED(hr)
    {
        DebugLog("CBdaGraph::AttachFilter","Cannot CreateClassEnumerator");
        return hr;
    }
    if (S_OK != hr)  // Class not found
    {
        DebugLog("CBdaGraph::AttachFilter","Class not found");
        return hr;
    }

    // next filter
	while(pIEnumMoniker->Next(1, &pIMoniker, 0) == S_OK)
    {
		// obtain filter's friendly name
        CComPtr <IPropertyBag>  pBag;
        hr = pIMoniker->BindToStorage(NULL, NULL, IID_IPropertyBag,
           reinterpret_cast<void**>(&pBag));
        if FAILED(hr)
        {
            DebugLog("CBdaGraph::AttachFilter","Cannot BindToStorage");
            return hr;
        }

        CComVariant varBSTR;
        hr = pBag->Read(L"FriendlyName", &varBSTR, NULL);
        if FAILED(hr)
        {
            DebugLog("CBdaGraph::AttachFilter","IPropertyBag->Read method failed");
            pIMoniker = NULL;
            continue;
        }

		TCHAR szName[MAX_PATH];
		WideCharToMultiByte(CP_ACP, 0, varBSTR.bstrVal, -1, szName, MAX_PATH, 0, 0);

		if (szMatchName)
			if (lstrcmp(szName, szMatchName)!=0)
			{
				pIMoniker = NULL;
				continue;
			}
		
		// bind the filter
		CComPtr <IBaseFilter>   pFilter;
		hr = pIMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,
			reinterpret_cast<void**>(&pFilter));
		if SUCCEEDED(hr)
		{
			if SUCCEEDED(pFilter.QueryInterface (ppFilter))
			{
				fFoundFilter = TRUE;
				return S_OK;
			}
		}
		pIMoniker = NULL;
		pFilter = NULL;
	} // while
	return E_HANDLE;
}

void CBdaGraph::MessageCallback(MSG_CB_PROC callback)
{
	message_callback = callback;
}

#ifdef _DEBUG
HRESULT CBdaGraph::AddGraphToROT(IUnknown *pUnkGraph, DWORD *pdwRegister)
{
	IMoniker *pMoniker;
	IRunningObjectTable *pROT;
	WCHAR wsz[128];
	HRESULT hr;

	if(FAILED(GetRunningObjectTable(0, &pROT)))
	{
		DebugLog("BDA2: AddGraphToROT: Failed getting ROT");
		return E_FAIL;
	}
	wsprintfW(wsz, L"FilterGraph %08x pid %08x - Dev_Bda2Driver\0", (DWORD_PTR) pUnkGraph, GetCurrentProcessId());
	hr = CreateItemMoniker(L"!", wsz, &pMoniker);
	if(FAILED(hr))
		DebugLog("BDA2: AddGraphToROT: Failed creating item moniker");
	else
	{
		hr = pROT->Register(ROTFLAGS_REGISTRATIONKEEPSALIVE, pUnkGraph, pMoniker, pdwRegister);
		if(FAILED(hr))
			DebugLog("BDA2: AddGraphToROT: Failed registering to ROT");
	}
	return hr;
}
HRESULT CBdaGraph::RemoveGraphFromROT(DWORD pdwRegister)
{
	IRunningObjectTable *pROT;

	if(FAILED(GetRunningObjectTable(0, &pROT)))
	{
		DebugLog("BDA2: RemoveGraphFromROT: Failed getting ROT");
		return E_FAIL;
	}
	return pROT->Revoke(pdwRegister);
}
#endif

void CBdaGraph::ReportMessage(char *text)
{
	DebugLog(text);
	if(message_callback)
	{
		struct MESSAGE_CALLBACK_DATA d;

		d.h = 0x0;
		d.message = text;

		message_callback(MESSAGE_CALLBACK_ID2, (char *)&d);
	}
}

HRESULT CBdaGraph::GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin) {
	CComPtr< IEnumPins > pEnum;
	*ppPin = NULL;

	HRESULT hr = pFilter->EnumPins(&pEnum);
	if(FAILED(hr))  return hr;

	ULONG ulFound;
	IPin *pPin;
	hr = E_FAIL;

	while (S_OK == pEnum->Next(1, &pPin, &ulFound)) {
		PIN_DIRECTION pindir = (PIN_DIRECTION)3;
		pPin->QueryDirection(&pindir);
		if (pindir == dirrequired) {
			if (iNum == 0) {
				*ppPin = pPin;  // Return the pin's interface
				hr = S_OK;      // Found requested pin, so clear error
				break;
			}
			iNum--;
		} 
		pPin->Release();
	}

	return hr;
}

IPin *CBdaGraph::GetInPin( IBaseFilter * pFilter, int nPin ) {
	CComPtr<IPin> pComPin=0;
	GetPin(pFilter, PINDIR_INPUT, nPin, &pComPin);
	return pComPin;
}

IPin *CBdaGraph::GetOutPin( IBaseFilter * pFilter, int nPin ) {
	CComPtr<IPin> pComPin=0;
	GetPin(pFilter, PINDIR_OUTPUT, nPin, &pComPin);
	return pComPin;
}

HRESULT CBdaGraph::GetPinMedium(IPin* pPin, REGPINMEDIUM* pMedium)
{
	if ( pPin == NULL || pMedium == NULL )
		return E_POINTER;

	CComPtr <IKsPin> pKsPin;
    KSMULTIPLE_ITEM *pmi;

    HRESULT hr = pPin->QueryInterface(IID_IKsPin, (void **)&pKsPin);
    if ( FAILED(hr) )
        return hr;  // Pin does not support IKsPin.

    hr = pKsPin->KsQueryMediums(&pmi);
    if ( FAILED(hr) )
        return hr;  // Pin does not support mediums.

    if ( pmi->Count )
    {
        // Use pointer arithmetic to reference the first medium structure.
        REGPINMEDIUM *pTemp = (REGPINMEDIUM*)(pmi + 1);
        memcpy(pMedium, pTemp, sizeof(REGPINMEDIUM));
    }

    CoTaskMemFree(pmi);

    return S_OK;
}

HRESULT CBdaGraph::BuildGraph(int selected_device_enum, enum VENDOR_SPECIFIC *VendorSpecific)
{
	char text[128];
	BSTR friendly_name_receiver;
	char receiver_name[128];

	HRESULT hr;

	hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IFilterGraph2, (void **)&m_pFilterGraph);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Can't instantiate FilterGraph");
		ReportMessage(text);
		return hr;
	}
#ifdef _DEBUG
	hr = AddGraphToROT(m_pFilterGraph, &m_dwGraphRegister);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Can't add FilterGraph to ROT");
		ReportMessage(text);
		return hr;
	}
#endif
	hr = S_OK;
	pNetworkProviderInstance = new CDVBNetworkProviderFilter(NULL, &hr);
	if (pNetworkProviderInstance == NULL || FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed instantiating custom Network Provider Filter");
		ReportMessage(text);
		return hr;
	}
	hr = pNetworkProviderInstance->QueryInterface(IID_IBaseFilter,(void **)&m_pNetworkProvider);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed creating custom Network Provider");
		ReportMessage(text);
		return hr;
	}
	hr = m_pFilterGraph->AddFilter(m_pNetworkProvider, L"Custom DVB-S|T|C Network Provider");
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed adding custom Network Provider filter to the graph");
		ReportMessage(text);
		return hr;
	}
	hr = FindTunerFilter(selected_device_enum, &friendly_name_tuner, &m_pTunerDevice);
	if(hr != S_OK)
	{
		sprintf(text,"BDA2: BuildGraph: Can't find Tuner filter #%d", selected_device_enum);
		ReportMessage(text);
		return hr;
	}
	// add tuner to the graph
	sprintf(text,"BDA2: BuildGraph: Adding Network Tuner to graph");
	ReportMessage(text);
	hr = m_pFilterGraph->AddFilter(m_pTunerDevice, friendly_name_tuner);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed adding Tuner filter to the graph");
		ReportMessage(text);
		return hr;
	}
	else
	{
		// Display the name in your UI somehow.
		WideCharToMultiByte(CP_ACP, 0, friendly_name_tuner, -1, receiver_name, sizeof(receiver_name), NULL, NULL);
		sprintf(text,"BDA2: BuildGraph: Added ID #%d '%s' Tuner filter to the graph",
			selected_device_enum, receiver_name);
		ReportMessage(text);
	}
	// connect Network provider with Tuner
	IPin *m_pP1, *m_pP2;
	m_pP1 = GetOutPin(m_pNetworkProvider, 0);
	if(m_pP1 == 0)
	{
		sprintf(text,"BDA2: BuildGraph: Failed finding the Output Pin of Network Provider");
		ReportMessage(text);
		return E_FAIL;
	}
	m_pP2 = GetInPin(m_pTunerDevice, 0);
	if(m_pP2 == 0)
	{
		sprintf(text,"BDA2: BuildGraph: Failed finding the Input Pin of Tuner filter");
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: BuildGraph: Connecting Network Provider with the Tuner filter");
	ReportMessage(text);

	hr = m_pFilterGraph->ConnectDirect(m_pP1, m_pP2, NULL);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed connecting Network Provider with the Tuner filter");
		ReportMessage(text);
		return hr;
	}
	sprintf(text,"BDA2: BuildGraph: Connected Network Provider with the Tuner filter");
	ReportMessage(text);

	VENDOR_SPECIFIC PrefBDA = *VendorSpecific;
	*VendorSpecific = VENDOR_SPECIFIC(PURE_BDA);

	// Technotrend check
	TTDevCat=UNKNOWN;
	if ( strstr(receiver_name, BDG2_NAME_S_TUNER) ||
		strstr(receiver_name, BDG2_NAME_C_TUNER) ||
		strstr(receiver_name, BDG2_NAME_T_TUNER) ||
		strstr(receiver_name, BDG2_NAME_S_TUNER_FAKE) ||
		strstr(receiver_name, BDG2_NAME_C_TUNER_NEW) ||
		strstr(receiver_name, BDG2_NAME_S_TUNER_NEW) ||
		strstr(receiver_name, BDG2_NAME_T_TUNER_NEW) )
		TTDevCat=BUDGET_2;
	if ( strstr(receiver_name, BUDGET3NAME_TUNER) ||
		strstr(receiver_name,  BUDGET3NAME_ATSC_TUNER) )
		TTDevCat=BUDGET_3;
	if ( strstr(receiver_name, USB2BDA_DVB_NAME_S_TUNER) ||
		strstr(receiver_name, USB2BDA_DVB_NAME_C_TUNER) ||
		strstr(receiver_name, USB2BDA_DVB_NAME_T_TUNER) ||
		strstr(receiver_name, USB2BDA_DVB_NAME_S_TUNER_FAKE) )
		TTDevCat=USB_2;
	if ( strstr(receiver_name, USB2BDA_DVBS_NAME_PIN_TUNER) )
		TTDevCat=USB_2_PINNACLE;
	if ( strstr(receiver_name, USB2BDA_DSS_NAME_TUNER) )
		TTDevCat=USB_2_DSS;
	if ( strstr(receiver_name, PREMIUM_NAME_DIGITAL_TUNER) )
		TTDevCat=PREMIUM;

	m_pP1 = GetOutPin(m_pTunerDevice, 0);
	// let's look if Demod exposes proprietary interfaces
	hr = m_pP1->QueryInterface(IID_IKsPropertySet, (void **)&m_pKsDemodPropSet);
	if (TTDevCat!=UNKNOWN)
	{
		DebugLog("BDA2: BuildGraph: checking for Technotrend DiSEqC interface");
		REGPINMEDIUM Medium;
		memset(&Medium, 0, sizeof(Medium));//CLSID clsMedium; DWORD dw1; DWORD dw2;
		if (GetPinMedium(m_pP1, &Medium) == S_OK)
		{
			
			hTT = bdaapiOpenHWIdx(TTDevCat,Medium.dw1);
			if (INVALID_HANDLE_VALUE!=hTT)
			{
				DebugLog("BDA2: BuildGraph: found Technotrend DiSEqC interface");
				*VendorSpecific = VENDOR_SPECIFIC(TT_BDA);
			}
		}
		else
			DebugLog("BDA2: BuildGraph: Can't get Tuner pin medium");
	}

	CComPtr <IKsObject> m_piKsObject; //KsObject Interface
	hr = m_pTunerDevice->QueryInterface(IID_IKsObject,(void**)&m_piKsObject);
	if SUCCEEDED(hr)
	{
		hDW = m_piKsObject->KsGetObjectHandle();
		DW_ID InfoDW;
		if SUCCEEDED(dwBDAGetDeviceID(hDW, &InfoDW))
		{
			DebugLog("BDA2: BuildGraph: found DvbWorld DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(DW_BDA);
		}
	}

	// let's look if Tuner exposes proprietary interfaces
	hr = m_pP2->QueryInterface(IID_IKsPropertySet, (void **)&m_pKsTunerPropSet);
	if (hr==S_OK && *VendorSpecific == VENDOR_SPECIFIC(PURE_BDA))
	{
		DWORD supported;
		sprintf(text,"BDA2: BuildGraph: Tuner exposes proprietary interfaces");
		ReportMessage(text);
		// Bestunar
		DebugLog("BDA2: BuildGraph: checking for Bestunar DiSEqC interface");
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionPropertiesBst,
			KSPROPERTY_BDA_DISEQC_MESSAGE, &supported);
		if(SUCCEEDED(hr) && supported)
		{
			DebugLog("BDA2: BuildGraph: found Bestunar DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(BST_BDA);
		}
		// Hauppauge
		DebugLog("BDA2: BuildGraph: checking for Hauppauge DiSEqC interface");
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionPropertiesHaup,
			KSPROPERTY_BDA_DISEQC_MESSAGE, &supported);
		if(SUCCEEDED(hr) && supported)
		{
			DebugLog("BDA2: BuildGraph: found Hauppauge DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(HAUP_BDA);
		}
		// Conexant
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_DISEQC_MESSAGE, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_GET) && (supported & KSPROPERTY_SUPPORT_SET) )
		{
			DebugLog("BDA2: BuildGraph: found Conexant DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(CXT_BDA);
		}
		// Turbosight
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_DISEQC_MESSAGE, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_GET) && (!(supported & KSPROPERTY_SUPPORT_SET)) )
		{
			DebugLog("BDA2: BuildGraph: found Turbosight-Conexant DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(TBS_BDA);
		}
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) && (!(supported & KSPROPERTY_SUPPORT_GET)) )
		{
			DebugLog("BDA2: BuildGraph: found Turbosight-NXP DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(TBS_NXP_BDA);
		}
		// Twinhan
		if (THBDA_IOCTL_CHECK_INTERFACE_Fun())
		{
			DebugLog("BDA2: BuildGraph: found Twinhan DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(TH_BDA);
		}
		// TeVii
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionPropertiesTeViiS460,
			KSPROPERTY_BDA_DISEQC, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) && (!(supported & KSPROPERTY_SUPPORT_GET)) )
		{
			iTVIdx=1;
			DebugLog("BDA2: BuildGraph: found TeVii S460 DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(TV_BDA);
		}
		else
		{
			hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionPropertiesTeViiS420,
				KSPROPERTY_BDA_DISEQC, &supported);
			if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) && (!(supported & KSPROPERTY_SUPPORT_GET)) )
			{
				iTVIdx=0;
				DebugLog("BDA2: BuildGraph: found TeVii S420 DiSEqC interface");
				*VendorSpecific = VENDOR_SPECIFIC(TV_BDA);
			}
		}
	}

	// let's look if Tuner filter exposes proprietary interfaces
	hr = m_pTunerDevice->QueryInterface(IID_IKsPropertySet, (void **)&m_pKsTunerFilterPropSet);
	if (hr==S_OK)
	{
		DWORD supported;
		// Turbosight QBOX
		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_MOTOR, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) && strcmp(receiver_name,"Prof 7500 DVBS Tuner"))
		{
			DebugLog("BDA2: BuildGraph: found Turbosight-QBOX DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(QBOX_BDA);
		}
		// Omicom
		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_OmcDiSEqCProperties,
			KSPROPERTY_OMC_DISEQC_WRITE, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
		{
			DebugLog("BDA2: BuildGraph: found Omicom DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(OMC_BDA);
		}
		// CrazyUnified
		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_CrazyTunerControl,
			KSPROPERTY_CRAZY_SET_DISEQC, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
		{
			DebugLog("BDA2: BuildGraph: found CrazyUnified DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(CRAZY_BDA);
		}
		// Astrometa
		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPSETID_AMDVB_Ter,
			KSPROPERTY_AMDVB_PLPINFO, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
		{
			DWORD mode=6;
			DebugLog("BDA2: BuildGraph: found Astrometa PLP interface");
			*VendorSpecific = VENDOR_SPECIFIC(AMT_BDA);
			ULONG bytesReturned = 0;
			hr = m_pKsTunerFilterPropSet->Get(KSPROPSETID_AMDVB_Ctl,
				KSPROPERTY_AMDVB_DELSYS,
				NULL, 0,
				&mode, sizeof(mode),
				&bytesReturned);
			Sleep(500);
			if (mode == 6)
				hr = m_pKsTunerFilterPropSet->Get(KSPROPSETID_AMDVB_Ter,
					KSPROPERTY_AMDVB_TERMODE,
					NULL, 0,
					&mode, sizeof(mode),
					&bytesReturned);
		}
	}

	hr = S_OK;
#ifdef SG_USE
	pCallbackInstance = new CSampleGrabberCB();
	if (!pCallbackInstance)
	{
		sprintf(text,"BDA2: BuildGraph: Failed instantiating SampleGrabber Callback");
		ReportMessage(text);
		return hr;
	}
    hr = CoCreateInstance(
                        CLSID_SampleGrabber,
                        NULL, 
                        CLSCTX_INPROC_SERVER,
                        IID_IBaseFilter, 
                        reinterpret_cast<void**>(&m_pCallbackFilter)
                        );
	if (FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed creating SampleGrabber Filter");
		ReportMessage(text);
		return hr;
	}

	CComPtr <ISampleGrabber> sg;
	hr = m_pCallbackFilter->QueryInterface(IID_ISampleGrabber, (void**)&sg);
	if FAILED(hr)
	{
		sprintf(text,"BDA2: BuildGraph: Failed QI SampleGrabber Filter");
		ReportMessage(text);
		return hr;
	}
	AM_MEDIA_TYPE mt;
	memset(&mt,0,sizeof(mt));
	mt.majortype = MEDIATYPE_Stream;
	mt.subtype = KSDATAFORMAT_SUBTYPE_BDA_MPEG2_TRANSPORT;
	hr = sg->SetMediaType(&mt);
	hr = sg->SetOneShot(FALSE);
	hr = sg->SetBufferSamples(FALSE);
	hr = sg->SetCallback(pCallbackInstance, 0);
#else
	pCallbackInstance = new CCallbackFilter(NULL, &hr);
	if (pCallbackInstance == NULL || FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed instantiating Callback Filter");
		ReportMessage(text);
		return hr;
	}
	hr = pCallbackInstance->QueryInterface(IID_IBaseFilter,(void **)&m_pCallbackFilter);
//	m_pCallbackFilter->AddRef();
	if (FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed creating Callback Filter");
		ReportMessage(text);
		return hr;
	}
#endif //SG_USE

	hr = m_pFilterGraph->AddFilter(m_pCallbackFilter, L"Callback");
	if (FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed adding Callback Filter to the FilterGraph");
		ReportMessage(text);
		return hr;
	}

	sprintf(text,"BDA2: BuildGraph: Trying to connect Tuner filter directly to Callback");
	ReportMessage(text);

	// try connecting Callback directly with Tuner
	m_pP1 = GetOutPin(m_pTunerDevice, 0);
	if(m_pP1 == 0)
	{
		sprintf(text,"BDA2: BuildGraph: Failed finding the Output Pin of the Tuner");
		ReportMessage(text);
		return E_FAIL;
	}
	m_pP2 = GetInPin(m_pCallbackFilter, 0);
	if(m_pP2 == 0)
	{
		sprintf(text,"BDA2: BuildGraph: Failed finding the Input Pin of the Callback Filter");
		ReportMessage(text);
		return E_FAIL;
	}
	hr = m_pFilterGraph->ConnectDirect(m_pP1, m_pP2, NULL);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed connecting the Tuner Filter with the Callback Filter");
		ReportMessage(text);

		// look for Receiver
		ICreateDevEnum *pSysDevEnum = NULL;
		hr = CoCreateInstance(CLSID_SystemDeviceEnum, NULL, CLSCTX_INPROC_SERVER,
			IID_ICreateDevEnum, (void **)&pSysDevEnum);
		if (FAILED(hr))
		{
			sprintf(text,"BDA2: BuildGraph: Failed creating device enumerator for Tuner");
			ReportMessage(text);
			return hr;
		}
		// Obtain a class enumerator for the BDA receiver category.
		IEnumMoniker *pEnumCat = NULL;
		hr = pSysDevEnum->CreateClassEnumerator(KSCATEGORY_BDA_RECEIVER_COMPONENT, &pEnumCat, 0);
		if (hr == S_OK) 
		{
			// Enumerate the monikers.
			IMoniker *pMoniker = NULL;
			ULONG cFetched;
			IBaseFilter *m_pFilter = NULL;
			while(pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK)
			{
				IPropertyBag *pPropBag;
				hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pPropBag);
				if (SUCCEEDED(hr))
				{
					// To retrieve the filter's friendly name, do the following:
					VARIANT varName;
					VariantInit(&varName);
					hr = pPropBag->Read(L"FriendlyName", &varName, 0);
					if (SUCCEEDED(hr))
					{
						char receiver_name[64];
						// Display the name in your UI somehow.
						friendly_name_receiver = SysAllocString(varName.bstrVal);
						WideCharToMultiByte(CP_ACP, 0, friendly_name_receiver, -1, receiver_name, sizeof(receiver_name), NULL, NULL);
						VariantClear(&varName);

						pPropBag->Release();
						hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter,(void**)&m_pFilter);
						if(FAILED(hr))
						{
							sprintf(text,"BDA2: BuildGraph: Failed binding moniker to object BDA_RECEIVER");
							ReportMessage(text);
							return hr;
						}
						// add potential receiver to the graph
						sprintf(text,"BDA2: BuildGraph: Adding potential Receiver (%s) to graph", receiver_name);
						ReportMessage(text);
						hr = m_pFilterGraph->AddFilter(m_pFilter, friendly_name_receiver);
						if(FAILED(hr))
						{
							sprintf(text,"BDA2: BuildGraph: Failed adding potential Receiver filter to the graph");
							ReportMessage(text);
							return hr;
						}
						sprintf(text,"BDA2: BuildGraph: Added potential Receiver (%s) to graph", receiver_name);
						ReportMessage(text);
						pMoniker->Release();
						// connect Tuner with potential Receiver
						m_pP1 = GetOutPin(m_pTunerDevice, 0);
						if(m_pP1 == 0)
						{
							sprintf(text,"BDA2: BuildGraph: Failed finding the Output Pin of Tuner");
							ReportMessage(text);
							return E_FAIL;
						}
						m_pP2 = GetInPin(m_pFilter, 0);
						if(m_pP2 == 0)
						{
							sprintf(text,"BDA2: BuildGraph: Failed finding the Input Pin of potential Receiver");
							ReportMessage(text);
							hr = E_FAIL;
						}
						else
							hr = m_pFilterGraph->ConnectDirect(m_pP1, m_pP2, NULL);
						if(FAILED(hr))
						{
							// found not compatible Receiver
							sprintf(text,"BDA2: BuildGraph: Failed connecting Tuner with potential Receiver '%s'", receiver_name);
							ReportMessage(text);

							m_pFilterGraph->RemoveFilter(m_pFilter);
							m_pFilter->Release();
							SysFreeString(friendly_name_receiver);
						}
						else
						{
							// found Receiver
							sprintf(text,"BDA2: BuildGraph: Found matching Receiver '%s'", receiver_name);
							ReportMessage(text);

							m_pReceiver = m_pFilter;
							break;
						}
					}
					else
					{
						sprintf(text,"BDA2: BuildGraph: Failed getting FriendlyName of potential Receiver");
						ReportMessage(text);
						return hr;
					}
				}
				else
				{
					sprintf(text,"BDA2: BuildGraph: Failed binding PropertyBag to storage for potential Receiver");
					ReportMessage(text);
					return hr;
				}
			}
			pEnumCat->Release();
			pSysDevEnum->Release();
		}
		else
		{
			sprintf(text,"BDA2: BuildGraph: Failed creating KSCATEGORY_BDA_RECEIVER_COMPONENT class enumerator");
			ReportMessage(text);
			return hr;
		}
		if(m_pReceiver == NULL)
		{
			sprintf(text,"BDA2: BuildGraph: Failed finding matching Receiver for the Tuner");
			ReportMessage(text);
			return hr;
		}
		// Capture filter found
		m_pP1 = GetOutPin(m_pReceiver, 0);
		if(m_pP1 == 0)
		{
			sprintf(text,"BDA2: BuildGraph: Failed finding the Output Pin of the Receiver");
			ReportMessage(text);
			return E_FAIL;
		}
		m_pP2 = GetInPin(m_pCallbackFilter, 0);
		if(m_pP2 == 0)
		{
			sprintf(text,"BDA2: BuildGraph: Failed finding the Input Pin of the Callback Filter");
			ReportMessage(text);
			return E_FAIL;
		}
		hr = m_pFilterGraph->ConnectDirect(m_pP1, m_pP2, NULL);
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: BuildGraph: Failed connecting the Capture Filter with the Callback Filter");
			ReportMessage(text);
			return hr;
		}
	}

	m_pP1 = GetOutPin(m_pTunerDevice, 0);
	// let's look if Demod exposes proprietary interfaces
	if SUCCEEDED(m_pP1->QueryInterface(IID_IKsPropertySet, (void **)&m_pKsDemodPropSet))
	{
		DWORD supported;
		sprintf(text,"BDA2: BuildGraph: Demod exposes proprietary interfaces");
		ReportMessage(text);
		// Genpix 3dparty
		DebugLog("BDA2: BuildGraph: checking for Genpix 3dparty DiSEqC interface");
		hr = m_pKsDemodPropSet->QuerySupported(KSPROPSETID_GnpBdaExtendedProperty,
			GNP_BDA_DISEQC, &supported);
		if(SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET))
		{
			DebugLog("BDA2: BuildGraph: found Genpix 3dparty DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(GNP_BDA);
		}
		// Netup
		DebugLog("BDA2: BuildGraph: checking for Netup DiSEqC interface");
		hr = m_pKsDemodPropSet->QuerySupported(KSPROPSETID_NetupExtProperties,
			KSPROPERTY_BDA_NETUP_IOCTL, &supported);
		if(SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET))
		{
			DebugLog("BDA2: BuildGraph: found Netup DiSEqC interface");
			*VendorSpecific = VENDOR_SPECIFIC(NETUP_BDA);
		}
	}

	GUID type;
	hr = GetNetworkTunerType(m_pTunerDevice, &type);

	if(IsEqualGUID(type, CLSID_DVBSNetworkProvider))
	{
		DWORD supported=0;
		// Microsoft
		DebugLog("BDA2: BuildGraph: checking for Microsoft DiSEqC interface");
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_ENABLE, &supported);
		if(SUCCEEDED(hr) && supported)
		{
			DebugLog("BDA2: BuildGraph: found Microsoft DiSEqC interface");
			if ( (*VendorSpecific == VENDOR_SPECIFIC(PURE_BDA)) || (PrefBDA == VENDOR_SPECIFIC(MS_BDA)) )
			{
				if (*VendorSpecific == VENDOR_SPECIFIC(TT_BDA))
					if (INVALID_HANDLE_VALUE!=hTT)
						bdaapiClose(hTT);
				*VendorSpecific = VENDOR_SPECIFIC(MS_BDA);
			}
		}
	}
	else
	{
		DWORD supported=0;

		DebugLog("BDA2: BuildGraph: checking for TBS PLP interface");
		hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_PLPINFO, &supported);
		if ( (*VendorSpecific == VENDOR_SPECIFIC(PURE_BDA)) || (PrefBDA == VENDOR_SPECIFIC(TBS_BDA)) )
			if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET) )
			{
				DebugLog("BDA2: BuildGraph: found Turbosight PLP interface");
				*VendorSpecific = VENDOR_SPECIFIC(TBS_BDA);
			}

		DebugLog("BDA2: BuildGraph: checking for TBS QBOX PLP interface");
		hr = m_pKsTunerFilterPropSet->QuerySupported(KSPROPERTYSET_QBOXControl,
			KSPROPERTY_CTRL_PLPINFO, &supported);
		if ( SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET))
		{
			DebugLog("BDA2: BuildGraph: found Turbosight-QBOX PLP interface");
			*VendorSpecific = VENDOR_SPECIFIC(QBOX_BDA);
		}

		DebugLog("BDA2: BuildGraph: checking for Microsoft PLP interface");
		hr = m_pKsDemodPropSet->QuerySupported(KSPROPSETID_BdaDigitalDemodulator,
				KSPROPERTY_BDA_PLP_NUMBER, &supported);
		if ( (*VendorSpecific == VENDOR_SPECIFIC(PURE_BDA)) || (PrefBDA == VENDOR_SPECIFIC(MS_BDA)) )
		{
			if(SUCCEEDED(hr) && (supported & KSPROPERTY_SUPPORT_SET))
			{
				DebugLog("BDA2: BuildGraph: found Microsoft PLP interface");
				*VendorSpecific = VENDOR_SPECIFIC(MS_BDA);
			}
			else
				*VendorSpecific = VENDOR_SPECIFIC(PURE_BDA);
		}
	}

	BdaType = *VendorSpecific;
	hr = m_pFilterGraph->QueryInterface(IID_IMediaControl, (void **)&m_pMediaControl);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: BuildGraph: Failed getting the MediaControl interface");
		ReportMessage(text);
		return hr;
	}
	return S_OK;
}

HRESULT CBdaGraph::RunGraph()
{
	HRESULT hr;
	char text[128];

	if(m_pMediaControl)
		hr = m_pMediaControl->Run();
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: RunGraph: Failed putting the Graph in Run state");
		ReportMessage(text);
		return hr;
	}
	return S_OK;
}

void CBdaGraph::CloseGraph()
{
	if(m_pFilterGraph == NULL)
		return;

	if (hTT!=INVALID_HANDLE_VALUE)
		bdaapiClose(hTT);

	if(m_pMediaControl)
	{
		m_pMediaControl->Stop();
		m_pMediaControl->Release();
		m_pMediaControl = NULL;
	}
	if(m_pCallbackFilter)
	{
		m_pFilterGraph->RemoveFilter(m_pCallbackFilter);
		m_pCallbackFilter->Release();
		m_pCallbackFilter = NULL;
	}
	if(m_pReceiver)
	{
		m_pFilterGraph->RemoveFilter(m_pReceiver);
		m_pReceiver->Release();
		m_pReceiver = NULL;
	}
	if(m_pTunerDevice)
	{
		m_pFilterGraph->RemoveFilter(m_pTunerDevice);
		m_pTunerDevice->Release();
		m_pTunerDevice = NULL;
	}
	if(m_pNetworkProvider)
	{
		m_pFilterGraph->RemoveFilter(m_pNetworkProvider);
		m_pNetworkProvider->Release();
		m_pNetworkProvider = NULL;
	}
	if(m_pKsTunerPropSet)
	{
		m_pKsTunerPropSet->Release();
		m_pKsTunerPropSet = NULL;
	}
	if(m_pKsDemodPropSet)
	{
		m_pKsDemodPropSet->Release();
		m_pKsDemodPropSet = NULL;
	}
	if(m_pKsTunerFilterPropSet)
	{
		m_pKsTunerFilterPropSet->Release();
		m_pKsTunerFilterPropSet = NULL;
	}
	if(m_pKsDeviceControl)
	{
		m_pKsDeviceControl->Release();
		m_pKsDeviceControl = NULL;
	}	
#ifdef _DEBUG
	if(m_dwGraphRegister)
	{
		RemoveGraphFromROT(m_dwGraphRegister);
		m_dwGraphRegister = 0;
	}
#endif
	if(pCallbackInstance)
		pCallbackInstance = NULL;
	if(pNetworkProviderInstance)
		pNetworkProviderInstance = NULL;
	m_pFilterGraph->Release();
	m_pFilterGraph = NULL;
}

void CBdaGraph::SetStreamCallbackProcedure(STR_CB_PROC callback)
{
	if(pCallbackInstance)
		pCallbackInstance->SetStreamCallbackProcedure(callback);
}

HRESULT CBdaGraph::DVBS_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			LONG PosOpt)
{
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBSTuning(
			LowBandF,
			HighBandF,
			SwitchF,
			Frequency,
			SpectrInv,
			ModType,
			SymRate,
			Pol,
			Fec,
			PosOpt);
	else
		return E_FAIL;
}

HRESULT CBdaGraph::DVBS_MS_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			Pilot S2Pilot,
			RollOff S2RollOff,
			ULONG ulLNBSource,
			BOOL bToneBurst)
{
	char text[256];
	HRESULT hr;

	KSPROPERTY instance_data;
	DWORD supported;
	// S2-Pilot
	hr = m_pKsDemodPropSet->QuerySupported(KSPROPSETID_BdaDigitalDemodulator,
		KSPROPERTY_BDA_PILOT, &supported);
	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		// set S2-Pilot
		hr = m_pKsDemodPropSet->Set(KSPROPSETID_BdaDigitalDemodulator,
			KSPROPERTY_BDA_PILOT,
			&instance_data, sizeof(instance_data), &S2Pilot, sizeof(S2Pilot));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_MS_Tune: failed setting Pilot to %d (hr=0x%8.8x)", S2Pilot, hr);
		else
			sprintf(text,"BDA2: DVBS_MS_Tune: set Pilot to %d", S2Pilot);
		ReportMessage(text);
	}

	// S2-RollOff
	hr = m_pKsDemodPropSet->QuerySupported(KSPROPSETID_BdaDigitalDemodulator,
		KSPROPERTY_BDA_ROLL_OFF, &supported);
	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		// set S2-Pilot
		hr = m_pKsDemodPropSet->Set(KSPROPSETID_BdaDigitalDemodulator,
			KSPROPERTY_BDA_ROLL_OFF,
			&instance_data, sizeof(instance_data), &S2RollOff, sizeof(S2RollOff));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_MS_Tune: failed setting RollOff to %d (hr=0x%8.8x)", S2RollOff, hr);
		else
			sprintf(text,"BDA2: DVBS_MS_Tune: set RollOff to %d", S2RollOff);
		ReportMessage(text);
	}

	if (ulLNBSource>0)
	{
    // enable/disable DiSEqC
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
		KSPROPERTY_BDA_DISEQC_ENABLE, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		BOOL bEnable=ulLNBSource>0;
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_ENABLE,
			&instance_data, sizeof(instance_data),
			&bEnable, sizeof(bEnable));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_MS_Tune: failed enable DiSEqC (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_MS_Tune: DiSEqC enabled");
		ReportMessage(text);
	}    
    // ToneBurst
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
		KSPROPERTY_BDA_DISEQC_USETONEBURST, &supported);
        
	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
        // set LNBSource
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_USETONEBURST,
			&instance_data, sizeof(instance_data),
			&bToneBurst, sizeof(bToneBurst));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_MS_Tune: failed set ToneBurst (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_MS_Tune: set ToneBurst to %d", bToneBurst);
		ReportMessage(text);
	}
    // Repeats
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
		KSPROPERTY_BDA_DISEQC_REPEATS, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		ULONG ulRepeats=2;
        // set LNBSource
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_REPEATS,
			&instance_data, sizeof(instance_data),
			&ulRepeats, sizeof(ulRepeats));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_MS_Tune: failed set Repeats (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_MS_Tune: set Repeats to %d", ulRepeats);
		ReportMessage(text);
	}
    // LNBSource
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
		KSPROPERTY_BDA_DISEQC_LNB_SOURCE, &supported);

	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
        // set LNBSource
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_LNB_SOURCE,
			&instance_data, sizeof(instance_data),
			&ulLNBSource, sizeof(ulLNBSource));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBS_MS_Tune: failed set LNBSource (0x%8.8x)", hr);
		else
			sprintf(text,"BDA2: DVBS_MS_Tune: set LNBSource to %d", ulLNBSource);
		ReportMessage(text);
	}
	}

	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBSTuning(
			LowBandF,
			HighBandF,
			SwitchF,
			Frequency,
			SpectrInv,
			ModType,
			SymRate,
			Pol,
			Fec,
			DEFAULT_POS_OPT);
	return E_FAIL;
}

HRESULT CBdaGraph::DVBS_OMC_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			int mis, int pls_mode, int pls_code)
{
	char text[256];
	HRESULT hr;

	KSPROPERTY instance_data;
	DWORD supported;

	hr = DVBS_OMC2_SetPLS(pls_mode&3, pls_code&0x3FFFF);
	hr = DVBS_OMC2_SetMIS(mis);
	ULONG SearchRange = SymRate>5000 ? 10000 : 5000;
	hr = DVBS_OMC2_Search(LowBandF,HighBandF,SwitchF,Frequency,SymRate,
						  SearchRange,OMC_BDA_COLD_LOCK,
						  Pol,ModType,Fec);
	if SUCCEEDED(hr) return hr;

	hr = DVBS_OMC2_SetSearchMode(OMC_BDA_COLD_LOCK);
	hr = DVBS_OMC2_SetSearchRange(SearchRange);

	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBSTuning(
			LowBandF,
			HighBandF,
			SwitchF,
			Frequency,
			SpectrInv,
			ModType,
			SymRate,
			Pol,
			Fec,
			DEFAULT_POS_OPT);
	return E_FAIL;
}

HRESULT CBdaGraph::DVBS_TT_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			LONG PosOpt)
{
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBSTuning(
			LowBandF,
			HighBandF,
			SwitchF,
			Frequency,
			SpectrInv,
			ModType,
			SymRate,
			Pol,
			Fec,
			PosOpt);
	return E_FAIL;
}

HRESULT CBdaGraph::DVBS_DW_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			UINT diseqc_port,
			UINT tone_burst )
{
	if (INVALID_HANDLE_VALUE==hDW)
		return E_FAIL;

	UINT pol=0;
	switch(Pol)
	{
	case BDA_POLARISATION_LINEAR_H:
	case BDA_POLARISATION_CIRCULAR_L:
		pol=1;break;
	case BDA_POLARISATION_LINEAR_V:
	case BDA_POLARISATION_CIRCULAR_R:
		pol=0;break;
	}

	if (!SwitchF)
	{
		if ((!LowBandF) && HighBandF)
		{
			SwitchF = Frequency-1000;
			LowBandF = HighBandF;
		} else if (LowBandF && (!HighBandF))
		{
			SwitchF = Frequency+1000;
			HighBandF = LowBandF;
		}
	}

	return dwBDATune(hDW,Frequency,SymRate,Frequency > SwitchF ? HighBandF:LowBandF,pol,Frequency > SwitchF,Fec,
		ModType,diseqc_port,tone_burst);
}

HRESULT CBdaGraph::DVBS_HAUP_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			DWORD S2Pilot,
			DWORD S2RollOff,
			LONG PosOpt)
{
	DWORD ret_len;
	DWORD instance[1024];
	char text[256];
	HRESULT hr;

	if(ModType == BDA_MOD_8PSK)
	{
		// DVB-S2
		// set Pilot
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionPropertiesHaup,
			KSPROPERTY_BDA_PILOT_HAUP,
			instance, 32, &S2Pilot, sizeof(S2Pilot));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): failed setting Pilot to %d (hr=0x%8.8x)", S2Pilot, hr);
			ReportMessage(text);
//			return E_FAIL;
		}
		else
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): set Pilot to %d", S2Pilot);
			ReportMessage(text);
		}
		hr = m_pKsTunerPropSet->Get(KSPROPSETID_BdaTunerExtensionPropertiesHaup,
			KSPROPERTY_BDA_PILOT_HAUP,
			instance, 32, &S2Pilot, sizeof(S2Pilot), &ret_len);
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): failed getting Pilot (hr=0x%8.8x)", hr);
			ReportMessage(text);
		}
		else
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): get Pilot: 0x%8.8x", S2Pilot);
			ReportMessage(text);
		}
		// set Roll Off
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionPropertiesHaup,
			KSPROPERTY_BDA_ROLLOFF_HAUP,
			instance, 32, &S2RollOff, sizeof(S2RollOff));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): failed setting RollOff to %d (hr=0x%8.8x)", S2RollOff, hr);
			ReportMessage(text);
//			return E_FAIL;
		}
		else
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): set RollOff to %d", S2RollOff);
			ReportMessage(text);
		}
		hr = m_pKsTunerPropSet->Get(KSPROPSETID_BdaTunerExtensionPropertiesHaup,
			KSPROPERTY_BDA_ROLLOFF_HAUP,
			instance, 32, &S2RollOff, sizeof(S2RollOff), &ret_len);
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): failed getting RollOff (hr=0x%8.8x)", hr);
			ReportMessage(text);
		}
		else
		{
			sprintf(text,"BDA2: DVBS_HAUP_Tune (8PSK): get RollOff: 0x%8.8x", S2RollOff);
			ReportMessage(text);
		}
	}
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBSTuning(
			LowBandF,
			HighBandF,
			SwitchF,
			Frequency,
			SpectrInv,
			ModType,
			SymRate,
			Pol,
			Fec,
			PosOpt);
	return E_FAIL;
}

HRESULT CBdaGraph::DVBS_CXT_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			DWORD S2Pilot,
			DWORD S2RollOff,
			LONG PosOpt)
{
	char text[256];
	HRESULT hr;

	if(ModType == BDA_MOD_8PSK)
	{
		// DVB-S2
		BDA_NBC_PARAMS NBCMessageParams;
		switch (S2Pilot)
		{
		case PILOT_OFF:
			NBCMessageParams.pilot = PHANTOM_PILOT_OFF;
			break;
		case PILOT_ON:
			NBCMessageParams.pilot = PHANTOM_PILOT_OFF;
			break;
		default:
			NBCMessageParams.pilot = PHANTOM_PILOT_UNKNOWN;
		}

		switch (S2RollOff)
		{
		case ROLLOFF_20:
			NBCMessageParams.rolloff = PHANTOM_ROLLOFF_020;
			break;
		case ROLLOFF_25:
			NBCMessageParams.rolloff = PHANTOM_ROLLOFF_025;
			break;
		case ROLLOFF_35:
			NBCMessageParams.rolloff = PHANTOM_ROLLOFF_035;
			break;
		default:
			NBCMessageParams.pilot = PHANTOM_PILOT_UNKNOWN;
		}

		KSPROPERTY instance_data;

		// set NBC Params
		hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
			KSPROPERTY_BDA_NBC_PARAMS,
			&instance_data, sizeof(instance_data), &NBCMessageParams, sizeof(NBCMessageParams));
		if FAILED(hr)
		{
			sprintf(text,"BDA2: DVBS_CXT_Tune (8PSK): failed setting Pilot, RollOff to (hr=0x%8.8x)", hr);
			ReportMessage(text);
		}
		else
		{
			sprintf(text,"BDA2: DVBS_CXT_Tune (8PSK): set Pilot, RollOff");
			ReportMessage(text);
		}
	}
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBSTuning(
			LowBandF,
			HighBandF,
			SwitchF,
			Frequency,
			SpectrInv,
			ModType,
			SymRate,
			Pol,
			Fec,
			PosOpt);
	return E_FAIL;
}

HRESULT CBdaGraph::DVBT_Tune(
			ULONG Frequency,
			ULONG Bandwidth)
{
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBTTuning(Frequency, Bandwidth);
	else
		return E_FAIL;
}

HRESULT CBdaGraph::DVBT_MS_Tune(
			ULONG Frequency,
			ULONG Bandwidth, LONG Plp)
{
	char text[256];
	HRESULT hr;

	KSPROPERTY instance_data;
	DWORD supported;
	// PLP
	hr = m_pKsDemodPropSet->QuerySupported(KSPROPSETID_BdaDigitalDemodulator,
		KSPROPERTY_BDA_PLP_NUMBER, &supported);
	if ( SUCCEEDED(hr) && ( supported & KSPROPERTY_SUPPORT_SET) )
	{
		// set PLP
		hr = m_pKsDemodPropSet->Set(KSPROPSETID_BdaDigitalDemodulator,
			KSPROPERTY_BDA_PLP_NUMBER,
			&instance_data, sizeof(instance_data), &Plp, sizeof(Plp));
		if FAILED(hr)
			sprintf(text,"BDA2: DVBT_Tune: failed setting PLP to %d (hr=0x%8.8x)", Plp, hr);
		else
			sprintf(text,"BDA2: DVBT_Tune: set PLP to %d", Plp);
		ReportMessage(text);
	}

	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBTTuning(Frequency, Bandwidth);
	else
		return E_FAIL;
}

HRESULT CBdaGraph::DVBT_Astrometa_Tune(
			ULONG Frequency,
			ULONG Bandwidth,
			int Mode,
			LONG Plp)
{
	char text[256];
	HRESULT hr;

	switch(Mode)
	{
		case DELSYS_DVBCA:
		case DELSYS_DVBCB:
		case DELSYS_DVBCC:
			Mode = RM_DVB_C; break;
		case DELSYS_DVBT:
			Mode = RM_DVB_T; break;
		case DELSYS_DVBT2:
		default:
			Mode = RM_DVB_T2;
	}

	DVBT_Astrometa_SetMode(Mode);

	if(pNetworkProviderInstance)
	{
		hr = pNetworkProviderInstance->DoDVBTTuning(Frequency, Bandwidth);
		hr = DVBT_Astrometa_SetPLP(Plp);
		return hr;
	}
	else
		return E_FAIL;
}

HRESULT CBdaGraph::DVBC_Tune(
	ULONG Frequency,
	LONG SymRate,
	ModulationType ModType)
{
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->DoDVBCTuning(Frequency, SymRate, ModType);
	else
		return E_FAIL;
}

HRESULT CBdaGraph::DVBS_SetPolarity(Polarisation Pol)
{
	if(pNetworkProviderInstance)
		return pNetworkProviderInstance->PutDVBSPolarity(Pol);
	else
		return E_FAIL;
}

HRESULT CBdaGraph::DVBS_GetSignalsStat(BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality)
{
	HRESULT hr=E_FAIL;
	if(pNetworkProviderInstance)
		hr = pNetworkProviderInstance->GetSignalStatistics(pPresent, pLocked, pStrength, pQuality);
	return hr;
}

HRESULT CBdaGraph::DVBS_TT_GetSignalsStat(BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality)
{
	if (INVALID_HANDLE_VALUE==hTT)
		return E_FAIL;

	TYPE_RET_VAL rc;
	DWORD stat[4];
	rc = bdaapiGetTuneStats (hTT,stat,sizeof(stat));
	if (rc)
		return E_FAIL;
	*pLocked=(BOOLEAN)stat[2];
	*pStrength=stat[1];
	*pQuality=stat[3];
	*pPresent=(BOOLEAN)stat[0];
	return S_OK;
}

char* ErrorMessageTTBDA ( TYPE_RET_VAL err )
{
	char *err_str;
	switch ( err )
	{
	case RET_SUCCESS:
		return NULL;
	case RET_NOT_IMPL:
		err_str = "operation is not implemented for the opened handle";
		break;
	case RET_NOT_SUPPORTED:
    	err_str = "operation is not supported for the opened handle";
		break;
	case RET_ERROR_HANDLE:
    	err_str = "the given HANDLE seems not to be correct";
		break;
	case RET_IOCTL_NO_DEV_HANDLE:
    	err_str = "the internal IOCTL subsystem has no device handle";
		break;
	case RET_IOCTL_FAILED:
        err_str = "the internal IOCTL failed";
		break;
	case RET_IR_ALREADY_OPENED:
        err_str = "the infrared interface is already initialised";
		break;
	case RET_IR_NOT_OPENED:
        err_str = "the infrared interface is not initialised";
		break;
	case RET_TO_MANY_BYTES:
		err_str = "length exceeds maximum in EEPROM-Userspace operation";
		break;
	case RET_CI_ERROR_HARDWARE:
		err_str = "common interface hardware error";
		break;
	case RET_CI_ALREADY_OPENED:
		err_str = "common interface already opened";
		break;
	case RET_TIMEOUT:
		err_str = "operation finished with timeout";
		break;
	case RET_READ_PSI_FAILED:
		err_str = "read psi failed";
		break;
	case RET_NOT_SET:
		err_str = "not set";
		break;
	case RET_ERROR:
		err_str = "operation finished with general error";
		break;
	case RET_ERROR_POINTER:
		err_str = "operation finished with illegal pointer";
		break;
	case RET_INCORRECT_SIZE:
		err_str = "the tunerequest structure did not have the expected size";
		break;
	case RET_TUNER_IF_UNAVAILABLE:
		err_str = "the tuner interface was not available";
		break;
	case RET_UNKNOWN_DVB_TYPE:
		err_str = "an unknown DVB type has been specified for the tune request";
		break;
	case RET_BUFFER_TOO_SMALL:
		err_str = "length of buffer is too small";
		break;
	default:
		err_str = "unknown error";
	}
	return err_str;
}

HRESULT CBdaGraph::DVBS_TT_DiSEqC(BYTE len, BYTE *DiSEqC_Command, BYTE tb)
{
	if (INVALID_HANDLE_VALUE==hTT)
		return E_FAIL;

	TYPE_RET_VAL rc;
	char text[256];
	rc = bdaapiSetDiSEqCMsg(hTT,DiSEqC_Command,len,0,tb,BDA_POLARISATION_LINEAR_V);
	if (rc)
	{
		sprintf(text,"BDA2: DVBS_TT_DiSEqC: failed - %s", ErrorMessageTTBDA(rc));
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_TT_DiSEqC: success");
	ReportMessage(text);
	return S_OK;
}

HRESULT CBdaGraph::DVBS_MS_DiSEqC(BYTE len, BYTE *DiSEqC_Command, BYTE repeats)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>8))
		return E_INVALIDARG;

	HRESULT hr = S_OK;
    DWORD type_support = 0;
	char text[256];

    // See if a tuner property set is supported
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
												 KSPROPERTY_BDA_DISEQC_ENABLE, 
												 &type_support);
    if (type_support & KSPROPERTY_SUPPORT_SET)
	{
		BOOL bEnable=TRUE;
		KSPROPERTY instance_data;
        // make call into driver
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_ENABLE,
			&instance_data, sizeof(instance_data),
			&bEnable, sizeof(BOOL));
	}

	// See if a tuner property set is supported
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
												 KSPROPERTY_BDA_DISEQC_REPEATS, 
												 &type_support);

    if (type_support & KSPROPERTY_SUPPORT_SET)
	{
		ULONG ulRepeats = repeats;
		KSPROPERTY instance_data;
        // make call into driver
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_REPEATS,
			&instance_data, sizeof(instance_data),
			&ulRepeats, sizeof(ULONG));
	}

	BDA_DISEQC_SEND diseqc_send_req;
	memcpy(diseqc_send_req.argbPacketData, DiSEqC_Command, len);
	diseqc_send_req.ulPacketLength = len;
	diseqc_send_req.ulRequestId = 1;

    // See if a tuner property set is supported
  	hr = m_pKsTunerPropSet->QuerySupported(KSPROPSETID_BdaDiseqCommand,
												 KSPROPERTY_BDA_DISEQC_SEND, 
												 &type_support);
        
    if (type_support & KSPROPERTY_SUPPORT_SET)
	{
		KSPROPERTY instance_data;
        // make call into driver
	  	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaDiseqCommand,
			KSPROPERTY_BDA_DISEQC_SEND,
			&instance_data, sizeof(instance_data),
			&diseqc_send_req, sizeof(BDA_DISEQC_SEND));
		if(FAILED(hr))
		{
			sprintf(text,"BDA2: DVBS_MS_DiSEqC: failed sending DiSEqC command (0x%8.8x)", hr);
			ReportMessage(text);
			return E_FAIL;
		}
		sprintf(text,"BDA2: DVBS_MS_DiSEqC: sent DiSEqC command");
	}
	else
	{
		sprintf(text,"BDA2: DVBS_MS_DiSEqC: not supported");
		return E_NOTIMPL;
	}

	ReportMessage(text);
	return S_OK;
}

HRESULT CBdaGraph::DVBS_TeVii_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	HRESULT hr = S_OK;

	PRIVATE_DISEQC diseqc_msg;
	CopyMemory(diseqc_msg.message,DiSEqC_Command,len);
	diseqc_msg.length=len;
	diseqc_msg.b_last_message = TRUE;
	diseqc_msg.power = 0;
	hr = m_pKsTunerPropSet->Set(iTVIdx ? KSPROPSETID_BdaTunerExtensionPropertiesTeViiS460 :
										KSPROPSETID_BdaTunerExtensionPropertiesTeViiS420,
										KSPROPERTY_BDA_DISEQC,
										&diseqc_msg, sizeof(diseqc_msg), &diseqc_msg, sizeof(diseqc_msg));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_TeVii_DiSEqC: success");
	else
		ReportMessage("BDA2: DVBS_TeVii_DiSEqC: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_TH_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>12))
		return E_INVALIDARG;

	DiSEqC_DATA diseqc_cmd;
	diseqc_cmd.command_len = len;
	memcpy(diseqc_cmd.command, DiSEqC_Command, len);
	if (THBDA_IOCTL_SET_DiSEqC_Fun(&diseqc_cmd))
	{
		ReportMessage("BDA2: DVBS_TH_DiSEqC: success");
		return S_OK;
	}
	ReportMessage("BDA2: DVBS_TH_DiSEqC: failed");
	return E_FAIL;
}

HRESULT CBdaGraph::DVBS_TH_LNBPower(BOOL bPower)
{
	LNB_DATA lnb_data;
	if (THBDA_IOCTL_GET_LNB_DATA_Fun(&lnb_data))
	{
		lnb_data.LNB_POWER = bPower ? LNB_POWER_ON : LNB_POWER_OFF;
		if (THBDA_IOCTL_SET_LNB_DATA_Fun(&lnb_data))
		{
			ReportMessage("BDA2: DVBS_TH_LNBPower: success");
			return S_OK;
		}
	}
	ReportMessage("BDA2: DVBS_TH_LNBPower: failed");
	return S_OK;
}

HRESULT CBdaGraph::DVBS_TH_LNBSource (BYTE Port, BOOL bToneBurst)
{
	LNB_DATA lnb_data;
	if (THBDA_IOCTL_GET_LNB_DATA_Fun(&lnb_data))
	{
		lnb_data.DiSEqC_Port = Port;
		lnb_data.Tone_Data_Burst = bToneBurst;
		if (THBDA_IOCTL_SET_LNB_DATA_Fun(&lnb_data))
		{
			ReportMessage("BDA2: DVBS_TH_LNBSource: success");
			return S_OK;
		}
	}
	ReportMessage("BDA2: DVBS_TH_LNBSource: failed");
	return S_OK;
}

HRESULT CBdaGraph::DVBS_DW_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	if (INVALID_HANDLE_VALUE==hDW)
		return E_FAIL;

	char text[256];
	HRESULT hr = dwBDADiseqSendCommand(hDW,DiSEqC_Command,len);
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_DW_DiSEqC: failed sending DiSEqC command(0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_DW_DiSEqC: sent DiSEqC command");
	ReportMessage(text);
	return S_OK;
}

HRESULT CBdaGraph::DVBS_AnySee_DiSEqC(BYTE len, BYTE *DiSEqC_Command, BYTE ToneBurst)
{
	CheckPointer(m_pKsCaptureFilterPropSet,E_NOINTERFACE);
	PROPERTY_DiSEqC_S cmd;

	if (len>_countof(cmd.Data))
		return E_INVALIDARG;

	if (len) CheckPointer(DiSEqC_Command,E_POINTER);

	HRESULT hr = S_OK;
	
	CopyMemory(cmd.Data,DiSEqC_Command,len);
	cmd.dwLength=len;
	cmd.ToneBurst=ToneBurst;

	hr = m_pKsCaptureFilterPropSet->Set(GUID_ANYSEE_CAPTURE_FILTER_PROPERTY,
								PROPERTY_SEND_DiSEqC_DATA,
								&cmd, sizeof(cmd),
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_AnySee_DiSEqC: success");
	else
		ReportMessage("BDA2: DVBS_AnySee_DiSEqC: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_GenpixOld_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsDemodPropSet,E_NOINTERFACE);
	GNP_DISEQC_CMD cmd;
	
	if (!len || len>_countof(cmd.ucMessage))
		return E_INVALIDARG;

	CheckPointer(DiSEqC_Command,E_POINTER);

	HRESULT hr = S_OK;
	
	cmd.ucMessageLength=len;
	CopyMemory(cmd.ucMessage,DiSEqC_Command,len);
	hr = m_pKsDemodPropSet->Set(KSPROPSETID_GnpBdaExtendedProperty,
								GNP_BDA_DISEQC,
								NULL, 0,
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_GenpixOld_DiSEqC: success");
	else
		ReportMessage("BDA2: DVBS_GenpixOld_DiSEqC: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Genpix_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);
	TUNER_COMMAND cmd;
	Z(cmd);

	if (!len || len>_countof(cmd.DiSEqC_Command))
		return E_INVALIDARG;

	CheckPointer(DiSEqC_Command,E_POINTER);

	HRESULT hr = S_OK;
	
	CopyMemory(cmd.DiSEqC_Command,DiSEqC_Command,len);
	cmd.DiSEqC_Length=len;
	cmd.ForceHighVoltage=FALSE;

	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_GnpTunerControl,
								KSPROPERTY_SET_DISEQC,
								NULL, 0,
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Genpix_DiSEqC: success");
	else
		ReportMessage("BDA2: DVBS_Genpix_DiSEqC: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Genpix_ToneBurst(BOOL bToneBurst)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

	HRESULT hr = S_OK;

	TUNER_COMMAND cmd;
	Z(cmd);
	cmd.DiSEqC_Command[0] = bToneBurst ? 1:0;
	cmd.DiSEqC_Length = 0;
	cmd.ForceHighVoltage = FALSE;
	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_GnpTunerControl,
								KSPROPERTY_SET_DISEQC,
								NULL, 0,
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Genpix_ToneBurst: success");
	else
		ReportMessage("BDA2: DVBS_Genpix_ToneBurst: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Netup_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsDemodPropSet,E_NOINTERFACE);

	CheckPointer(DiSEqC_Command,E_POINTER);
	if (!len) return E_INVALIDARG;

	HRESULT hr = S_OK;

	NETUP_BDA_EXT_CMD cmd;
	Z(cmd);

	cmd.dwCmd = NETUP_IOCTL_CMD(NETUP_IOCTL_DISEQC_WRITE,0,0);
	cmd.lpInputBuffer = DiSEqC_Command;
	cmd.dwInputBufferLength=len;

	hr = m_pKsDemodPropSet->Set(KSPROPSETID_NetupExtProperties,
								KSPROPERTY_BDA_NETUP_IOCTL,
								&cmd, sizeof(cmd),
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Netup_DiSEqC: success");
	else
		ReportMessage("BDA2: DVBS_Netup_DiSEqC: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Crazy_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);
	CRAZY_TUNER_COMMAND cmd;
	Z(cmd);

	if (!len || len>_countof(cmd.DiSEqC_Command))
		return E_INVALIDARG;

	CheckPointer(DiSEqC_Command,E_POINTER);

	HRESULT hr = S_OK;
	
	CopyMemory(cmd.DiSEqC_Command,DiSEqC_Command,len);
	cmd.DiSEqC_Length=len;

	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_CrazyTunerControl,
								KSPROPERTY_CRAZY_SET_DISEQC,
								&cmd, sizeof(cmd),
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Crazy_DiSEqC: success");
	else
		ReportMessage("BDA2: DVBS_Crazy_DiSEqC: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Crazy_ToneBurst(BOOL bToneBurst)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);

	HRESULT hr = S_OK;

	CRAZY_TUNER_COMMAND cmd;
	Z(cmd);

	cmd.DiSEqC_Command[0] = bToneBurst ? 1:0;
	cmd.DiSEqC_Length = 0;
	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_CrazyTunerControl,
								KSPROPERTY_CRAZY_SET_DISEQC,
								&cmd, sizeof(cmd),
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Crazy_ToneBurst: success");
	else
		ReportMessage("BDA2: DVBS_Crazy_ToneBurst: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Crazy_LNBPower(BOOL bPower)
{
	CheckPointer(m_pKsTunerFilterPropSet,E_NOINTERFACE);
	HRESULT hr = S_OK;
	

	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_CrazyTunerControl,
								KSPROPERTY_CRAZY_SET_POL,
								&bPower, sizeof(bPower),
								&bPower, sizeof(bPower));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Crazy_LNBPower: success");
	else
		ReportMessage("BDA2: DVBS_Crazy_LNBPower: failed");

	return hr;
}

HRESULT CBdaGraph::DVBS_Crazy_Tune(
			ULONG LowBandF,
			ULONG HighBandF,
			ULONG SwitchF,
			ULONG Frequency,
			SpectralInversion SpectrInv,
			ModulationType ModType,
			LONG SymRate,
			Polarisation Pol,
			BinaryConvolutionCodeRate Fec,
			int mis, int pls_mode, int pls_code)
{
	char text[256];
	HRESULT hr;

	KSPROPERTY instance_data;
	DWORD supported;

	pls_mode &= 3;
	pls_code &= 0x3FFFF;

	CRAZY_TUNER_COMMAND cmd;

	cmd.FrequencyMhz = Frequency/1000;
	cmd.SymbolRateKsps = SymRate;
	cmd.LOFLowMhz = LowBandF/1000;
	cmd.LOFHighMhz = HighBandF/1000;
	cmd.SwitchFreqMhz = SwitchF/1000;
	cmd.SignalPolarisation = Pol;
	cmd.Modulation = ModType;
	cmd.FECRate = Fec;
	if (mis > -1)
		cmd.IsId = (pls_mode<<26) | (pls_code<<8) | (mis & 0xff);
	else
		cmd.IsId = -1;

	hr = m_pKsTunerFilterPropSet->Set(KSPROPERTYSET_CrazyTunerControl,
								KSPROPERTY_CRAZY_SET_FREQUENCY,
								&cmd, sizeof(cmd),
								&cmd, sizeof(cmd));
	if(SUCCEEDED(hr))
		ReportMessage("BDA2: DVBS_Crazy_Tune: success");
	else
		ReportMessage("BDA2: DVBS_Crazy_Tune: failed");

	return hr;
}
