#include "stdafx.h"
#include "NetworkProvider.h"

CDVBNetworkProviderPin::CDVBNetworkProviderPin(CDVBNetworkProviderFilter *m_pFilter, CCritSec *pLock, HRESULT *phr) :
	CBaseOutputPin(NAME("CDVBNetworkProviderPin"), m_pFilter, pLock, phr, L"Antenna Out")
{
	pFilter = m_pFilter;
}

HRESULT CDVBNetworkProviderPin::CheckMediaType(const CMediaType* pmt)
{
	CheckPointer(pmt,E_POINTER);

	if(! IsEqualGUID(pmt->majortype, KSDATAFORMAT_TYPE_BDA_ANTENNA))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderPin.CheckMediaType: Wrong MajorType BDA Antenna")));
		return VFW_E_INVALIDMEDIATYPE;
	}
    return S_OK;
}

HRESULT CDVBNetworkProviderPin::DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *pRequest)
{
	HRESULT hr;

    CheckPointer(pAlloc, E_POINTER);
    CheckPointer(pRequest, E_POINTER);

	pRequest->cBuffers = 0;
    pRequest->cbBuffer = 0;

    ALLOCATOR_PROPERTIES Actual;
    hr = pAlloc->SetProperties(pRequest, &Actual);
    if (FAILED(hr)) 
    {
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderPin.DecideBufferSize: failed setting properties of Allocator")));
        return hr;
    }

    // Is this allocator unsuitable?
    if (Actual.cbBuffer < pRequest->cbBuffer) 
    {
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderPin.DecideBufferSize: unsuitable Allocator")));
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CDVBNetworkProviderPin::CompleteConnect(IPin *pReceivePin)
{
	CheckPointer(pReceivePin,E_POINTER);
	
/*
	HRESULT hr;
	hr = pFilter->CreateOutputPin(pReceivePin);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderPin.CompleteConnect.CreateOutputPin: failed")));
		return hr;
	}
*/
	return CBaseOutputPin::CompleteConnect(pReceivePin);
}

HRESULT CDVBNetworkProviderPin::Run(REFERENCE_TIME tStart)
{
	IPin *pin;

	pin = GetConnected();
	if (pin == NULL)
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderPin.Run: connected Pin is NULL")));
		return E_FAIL;
	}

	/*
	HRESULT hr;
	hr = pFilter->CreateOutputPin(pin);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderPin.Run: CreateOutputPin Failed")));
		return hr;
	}
	*/
	return CBaseOutputPin::Run(tStart);
}

CDVBNetworkProviderFilter::CDVBNetworkProviderFilter(LPUNKNOWN pUnk, HRESULT *phr) :
	CBaseFilter(TEXT("NetwProv filter"), pUnk, &m_pSetupLock, CLSID_DVBNetworkProviderFilter),
	SID(0),
	previous_PosOpt(0L),
	pSignalPresent(NULL),
	pSignalLocked(NULL),
	pSignalStrength(NULL),
	pSignalQuality(NULL)

{
	pin = new CDVBNetworkProviderPin(this, &m_pPinLock, phr);
}

CBasePin * CDVBNetworkProviderFilter::GetPin(int n)
{
    if (n == 0) {
        return pin;
    } else {
        return NULL;
    }
}

int CDVBNetworkProviderFilter::GetPinCount()
{
    return 1;
}

STDMETHODIMP CDVBNetworkProviderFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    CAutoLock lock(m_pLock);

	if (riid == IID_IBDA_NetworkProvider) {
        HRESULT hr = GetInterface((IBDA_NetworkProvider *) this, ppv);
		return hr;
    } else
/*	if (riid == IID_ISpecifyPropertyPages) {
        HRESULT hr = GetInterface((ISpecifyPropertyPages *) this, ppv);
		return hr;
    } else*/
	if (riid == IID_ITuner) {
        HRESULT hr = GetInterface((ITuner *) this, ppv);
		return hr;
    } else {
        // Pass the buck
        HRESULT hr = CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
		return hr;
	}
} // NonDelegatingQueryInterface
/*
// ==============Implementation of the IPropertypages Interface ===========

//
// GetPages
//
STDMETHODIMP CDVBNetworkProviderFilter::GetPages(CAUUID * pPages)
{
    CheckPointer(pPages,E_POINTER);

    pPages->cElems = 1;
    pPages->pElems = (GUID *) CoTaskMemAlloc(sizeof(GUID));
    if (pPages->pElems == NULL) {
        return E_OUTOFMEMORY;
    }

    *(pPages->pElems) = CLSID_DVBNetworkProviderFilterProp;

    return NOERROR;

} // GetPages

//
// put_GargleShape
//
// Alter the waveform between triangle and square
//
STDMETHODIMP CDVBNetworkProviderFilter::put_Frequency(int iFrequency)
{
    if (iFrequency<0 || iFrequency>250000)
        return E_INVALIDARG;

    m_Frequency = iFrequency;
//    CPersistStream::SetDirty(TRUE);                     // Need to scribble

    return NOERROR;
} // put_Frequency

//
// get_Frequency
//
// Return 0 if the current shape is triangle, 1 if it's square
//
STDMETHODIMP CDVBNetworkProviderFilter::get_Frequency(int *iFrequency)
{
    CheckPointer(iFrequency,E_POINTER);

    *iFrequency = m_Frequency;
    return NOERROR;

} // get_Frequency

*/

STDMETHODIMP CDVBNetworkProviderFilter::PutSignalSource(ULONG ulSignalSource)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::GetSignalSource(ULONG *pulSignalSource)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::GetNetworkType(GUID *pguidNetworkType)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::PutTuningSpace(REFGUID guidTuningSpace)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::GetTuningSpace(GUID *pguidTuingSpace)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::RegisterDeviceFilter(IUnknown *pUnkFilterControl, ULONG *ppvRegisitrationContext)
{
	HRESULT hr;

	if(pin->GetConnected() != NULL)
		hr = CreateOutputPin(pin->GetConnected());
    CAutoLock lock(&m_pSetupLock);
	if(SID < MAX_REGISTERED_DEVICES)
	{
		RegisteredDevices[SID].Control = pUnkFilterControl;
		RegisteredDevices[SID].ID = SID;
		*ppvRegisitrationContext = ++SID;
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::RegisterDeviceFilter: succeeded SID=%d"), SID));
	}
	else
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::RegisterDeviceFilter: failed")));
		return E_FAIL;
	}
	return S_OK;
}
STDMETHODIMP CDVBNetworkProviderFilter::UnRegisterDeviceFilter(ULONG pvRegistrationContext)
{
	DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::UnRegisterDeviceFilter: succeeded SID=%d"), SID));
	--SID;
	return S_OK;
}

// ITuner
/*STDMETHODIMP CDVBNetworkProviderFilter::get_TuningSpace(ITuningSpace **TuningSpace)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::put_TuningSpace(ITuningSpace *TuningSpace)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::EnumTuningSpaces(IEnumTuningSpaces **ppEnum)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::get_TuneRequest(ITuneRequest **TuneRequest)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::put_TuneRequest(ITuneRequest *TuneRequest)
{
	HRESULT hr;
	if(SID)
	{
		ITuningSpace *pTuningSpace;
		GUID NetworkType;
		hr = TuneRequest->get_TuningSpace(&pTuningSpace);
		pTuningSpace->get__NetworkType(&NetworkType);
		if(IsEqualGUID(NetworkType, CLSID_DVBSNetworkProvider)) //TODO other net types
		{
			hr = DoDVBSTuning(RegisteredDevices.Control, TuneRequest);
		}
	}

	return S_OK;
}
STDMETHODIMP CDVBNetworkProviderFilter::Validate(ITuneRequest *TuneRequest)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::get_PreferredComponentTypes(IComponentTypes **ComponentTypes)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::put_PreferredComponentTypes(IComponentTypes *ComponentTypes)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::get_SignalStrength(long *Strength)
{
	return E_NOTIMPL;
}
STDMETHODIMP CDVBNetworkProviderFilter::TriggerSignalEvents(long Interval)
{
	return E_NOTIMPL;
}*/

HRESULT CDVBNetworkProviderFilter::CreateOutputPin(IPin *Apin)
{
	const int NUM_MAPPINGS = 20;
	PIN_INFO pin_info;
	ULONG pinIn, pinOut;
	ULONG mappings[NUM_MAPPINGS];
	ULONG num_maps;
	IBDA_Topology *topology;
	IEnumPins *enum_pins;
	IPin *pin;
	HRESULT hr;

	pinIn = 0;
	pinOut = 1;
	topology = NULL;
	hr = Apin->QueryPinInfo(&pin_info);
	if(SUCCEEDED(hr))
	{
		if(pin_info.pFilter != NULL)
		{
			hr = pin_info.pFilter->QueryInterface(IID_IBDA_Topology, (void **)&topology);
			if(SUCCEEDED(hr))
			{
				hr = topology->GetPinTypes(&num_maps, NUM_MAPPINGS, mappings);
				if(SUCCEEDED(hr))
				{
					if(num_maps > 1)
					{
						pinIn = mappings[0];
						pinOut = mappings[1];
					}
					hr = pin_info.pFilter->EnumPins(&enum_pins);
					if(SUCCEEDED(hr))
					{
						hr = enum_pins->Next(1, &pin, NULL);
						if(hr != S_OK)
							hr = topology->CreatePin(mappings[0], &pinIn);
						pin = NULL;
						if(hr == S_OK)
						{
							hr = enum_pins->Next(1, &pin, NULL);
							if(hr != S_OK)
								hr = topology->CreatePin(mappings[1], &pinOut);
						}
					}
					else
						DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::CreateOutputPin: failed enumerating Pins")));
				}
				else
					DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::CreateOutputPin: failed getting Pin types")));
			}
			else
				DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::CreateOutputPin: failed getting Topology interface")));
		}
		else
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::CreateOutputPin: Filter for Pin is NULL")));
	}
	else
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter::CreateOutputPin: failed querying pin info")));
	if((topology != NULL) && SUCCEEDED(hr))
		hr = topology->CreateTopology(pinIn, pinOut);
	else
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.CreateOutputPin: failed CreateTopology")));
		hr = E_FAIL;
	}
	return hr;
}

HRESULT CDVBNetworkProviderFilter::GetMultipleTopology(IBaseFilter *p_Filter, GUID Topology, int *node_cnt, void **p_Interface)
{
	IBDA_Topology *p_Topology;
	ULONG node_type_cnt, node_type_array[MAX_TOPOLOGY_NODES];
	ULONG node_interface_cnt;
	GUID node_interface_array[MAX_TOPOLOGY_NODES];
	IUnknown *p_IUnknown;
	void *p_Interface_tmp;

	*node_cnt = 0;
	if(p_Filter->QueryInterface(IID_IBDA_Topology, (void **)&p_Topology) == S_OK)
	{
		if(p_Topology->GetNodeTypes(&node_type_cnt, 10, node_type_array) == S_OK)
		{
			for(unsigned i=0; i<node_type_cnt; ++i)
			{
				if(p_Topology->GetNodeInterfaces(node_type_array[i], &node_interface_cnt, 10, node_interface_array) == S_OK)
					for(unsigned j=0; j<node_interface_cnt; ++j)
					{
						if(IsEqualGUID(node_interface_array[j], Topology))
							if(p_Topology->GetControlNode(0, 1, node_type_array[i], &p_IUnknown) == S_OK)
							{
								if(p_IUnknown->QueryInterface(Topology, (void **)&p_Interface_tmp) == S_OK)
								{
									p_IUnknown->Release();
									p_Interface[(*node_cnt)++] = p_Interface_tmp;
								}
								else
								{
									DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetMultipleTopology: failed getting Node's interface")));
									p_IUnknown->Release();
									return E_FAIL;
								}
							}
							else
								DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetMultipleTopology: failed getting ControlNodes")));
					}
				else
					DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetMultipleTopology: failed getting NodeInterfaces")));
			}
			p_Topology->Release();
			return S_OK;
		}
		else
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetMultipleTopology: failed getting NodeTypes")));
		p_Topology->Release();
	}
	DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetMultipleTopology: failed getting Topology")));

	return E_FAIL;
}

HRESULT CDVBNetworkProviderFilter::GetTopology(IBaseFilter *p_Filter, GUID Topology, void **p_Interface)
{
	IBDA_Topology *p_Topology;
	ULONG node_type_cnt, node_type_array[MAX_TOPOLOGY_NODES];
	ULONG node_interface_cnt;
	GUID node_interface_array[MAX_TOPOLOGY_NODES];
	IUnknown *p_IUnknown;

	if(p_Filter->QueryInterface(IID_IBDA_Topology, (void **)&p_Topology) == S_OK)
	{
		if(p_Topology->GetNodeTypes(&node_type_cnt, 10, node_type_array) == S_OK)
			for(unsigned i=0; i<node_type_cnt; ++i)
			{
				if(p_Topology->GetNodeInterfaces(node_type_array[i], &node_interface_cnt, 10, node_interface_array) == S_OK)
					for(unsigned j=0; j<node_interface_cnt; ++j)
					{
						if(IsEqualGUID(node_interface_array[j], Topology))
							if(p_Topology->GetControlNode(0, 1, node_type_array[i], &p_IUnknown) == S_OK)
							{
								if(p_IUnknown->QueryInterface(Topology, (void **)p_Interface) == S_OK)
								{
									p_IUnknown->Release();
									return S_OK;
								}
								else
								{
									DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetTopology: failed getting Node's interface")));
									p_IUnknown->Release();
									return E_FAIL;
								}
							}
							else
								DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetTopology: failed getting ControlNodes")));
					}
				else
					DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetTopology: failed getting NodeInterfaces")));
			}
		else
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetTopology: failed getting NodeTypes")));
		p_Topology->Release();
	}
	DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetTopology: failed getting Topology")));

	return E_FAIL;
}

HRESULT CDVBNetworkProviderFilter::GetSignalStatistics(
		BOOLEAN *pPresent,
		BOOLEAN *pLocked,
		LONG *pStrength,
		LONG *pQuality)
{
	HRESULT hr;

    CAutoLock lock(m_pLock);
	// do we have the interfaces?
	if(pSignalPresent && pSignalLocked && pSignalStrength && pSignalQuality)
	{
		pSignalPresent->get_SignalPresent(pPresent);
		pSignalLocked->get_SignalLocked(pLocked);
		pSignalStrength->get_SignalStrength(pStrength);
		pSignalQuality->get_SignalQuality(pQuality);
	}
	else
	{
		// get the interfaces
		IBaseFilter *filt;
		int node_cnt;
		int device_index;
		IBDA_SignalStatistics *pSignalStatistics[MAX_TOPOLOGY_NODES];
		BOOLEAN Present = FALSE;
		BOOLEAN Locked = FALSE;
		LONG Strength = 0L;
		LONG Quality = 0L;

		for(device_index=0; device_index<MAX_REGISTERED_DEVICES; ++device_index)
		{
			hr = RegisteredDevices[device_index].Control->QueryInterface(IID_IBaseFilter, (void **)&filt);
			if(FAILED(hr))
			{
				DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetSignalStatistics: failed getting Control device Interface")));
				return hr;
			}
			hr = GetMultipleTopology(filt, IID_IBDA_SignalStatistics, &node_cnt, (void **)pSignalStatistics);
			if(SUCCEEDED(hr))
				break;
		}
		if(device_index == SID)
		{
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetSignalStatistics: failed finding IBDA_SignalStatistics topology")));
			return E_FAIL;
		}
		for(int i=0; i<node_cnt; ++i)
		{
			if(pSignalStatistics[i]->get_SignalPresent(&Present) == S_OK) 
			{
				pSignalPresent = pSignalStatistics[i];
				*pPresent = Present;
			}
			if(pSignalStatistics[i]->get_SignalLocked(&Locked) == S_OK)
			{
				pSignalLocked = pSignalStatistics[i];
				*pLocked = Locked;
			}
			if(pSignalStatistics[i]->get_SignalStrength(&Strength) == S_OK)
			{
				pSignalStrength = pSignalStatistics[i];
				*pStrength = Strength;
			}
			if(pSignalStatistics[i]->get_SignalQuality(&Quality) == S_OK)
			{
				pSignalQuality = pSignalStatistics[i];
				*pQuality = Quality;
			}
			if(pSignalPresent && pSignalLocked && pSignalStrength && pSignalQuality)
				break; // found all interfaces
		}
		if(node_cnt == 0)
		{
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.GetSignalStatistics: failed getting Signal Statistics")));
			return E_FAIL;
		}
	}
	return S_OK;
}

HRESULT CDVBNetworkProviderFilter::StartChanges(IUnknown *Control)
{
	HRESULT hr;
	IBDA_DeviceControl *ctrl;

	hr = Control->QueryInterface(IID_IBDA_DeviceControl, (void **)&ctrl);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.StartChanges: failed getting Control device Interface")));
		return hr;
	}
	hr = ctrl->StartChanges();
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.StartChanges: failed")));
	return hr;
}

HRESULT CDVBNetworkProviderFilter::CommitChanges(IUnknown *Control)
{
	HRESULT hr;
	IBDA_DeviceControl *ctrl;

	hr = Control->QueryInterface(IID_IBDA_DeviceControl, (void **)&ctrl);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.CommitChanges: failed getting Control device Interface")));
		return hr;
	}
	Sleep(50);
	hr = ctrl->CheckChanges();
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.CommitChanges: failed CheckChanges")));
		return hr;
	}
	hr = ctrl->CommitChanges();
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.CommitChanges: failed CommitChanges")));
	return hr;
}

HRESULT CDVBNetworkProviderFilter::DoDVBSTuning(
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
	HRESULT hr;
	IBaseFilter *filt;
	IBDA_FrequencyFilter *pFreqFilter = NULL;
	IBDA_DigitalDemodulator *pDemodFilter = NULL;
	IBDA_LNBInfo *pLNB = NULL;
	ULONG device_index;

    CAutoLock lock(m_pLock);
	for(device_index=0; device_index<SID; ++device_index)
	{
		hr = RegisteredDevices[device_index].Control->QueryInterface(IID_IBaseFilter, (void **)&filt);
		if(FAILED(hr))
		{
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed getting Control device Interface")));
			return hr;
		}
		hr = GetTopology(filt, IID_IBDA_FrequencyFilter, (void **)&pFreqFilter);
		if(SUCCEEDED(hr))
			break;
	}
	if(device_index == SID)
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed finding IBDA_FrequencyFilter topology")));
		return E_FAIL;
	}
	hr = GetTopology(filt, IID_IBDA_DigitalDemodulator, (void **)&pDemodFilter);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed finding IBDA_DigitalDemodulator topology")));
		return E_FAIL;
	}
	hr = GetTopology(filt, IID_IBDA_LNBInfo, (void **)&pLNB);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed finding IBDA_LNBInfo topology")));
		return E_FAIL;
	}

	hr = StartChanges(RegisteredDevices[device_index].Control);

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

	hr = pLNB->put_LocalOscilatorFrequencyLowBand(LowBandF);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_LocalOscilatorFrequencyLowBand")));
	hr = pLNB->put_LocalOscilatorFrequencyHighBand(HighBandF);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_LocalOscilatorFrequencyHighBand")));
	hr = pLNB->put_HighLowSwitchFrequency(SwitchF);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_HighLowSwitchFrequency")));
	hr = pFreqFilter->put_Range(PosOpt);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_Range")));
	else
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: put_Range 0x%4.4x"),PosOpt));
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_Range")));
	hr = pFreqFilter->put_Polarity(Pol);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_Polarity")));
	hr = pFreqFilter->put_FrequencyMultiplier(1000L);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_FrequencyMultiplier")));
	hr = pFreqFilter->put_Frequency(Frequency);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_Frequency")));
	hr = pDemodFilter->put_SpectralInversion(&SpectrInv);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_SpectralInversion")));
	hr = pDemodFilter->put_ModulationType(&ModType);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_ModulationType")));
	hr = pDemodFilter->put_SymbolRate((ULONG *)&SymRate);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_SymbolRate")));
	/*
	FECMethod FecMethod=BDA_FEC_VITERBI;
	hr = pDemodFilter->put_InnerFECMethod(&FecMethod);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_InnerFECRate")));
	*/
	hr = pDemodFilter->put_InnerFECRate(&Fec);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_InnerFECRate")));
	hr = CommitChanges(RegisteredDevices[device_index].Control);
	return hr;
}

HRESULT CDVBNetworkProviderFilter::DoDVBTTuning(
		ULONG Frequency,
		ULONG Bandwidth)
{
	HRESULT hr;
	IBaseFilter *filt;
	IBDA_FrequencyFilter *pFreqFilter = NULL;
	ULONG device_index;

    CAutoLock lock(m_pLock);
	for(device_index=0; device_index<SID; ++device_index)
	{
		hr = RegisteredDevices[device_index].Control->QueryInterface(IID_IBaseFilter, (void **)&filt);
		if(FAILED(hr))
		{
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBTTuning: failed getting Control device Interface")));
			return hr;
		}
		hr = GetTopology(filt, IID_IBDA_FrequencyFilter, (void **)&pFreqFilter);
		if(SUCCEEDED(hr))
			break;
	}
	if(device_index == SID)
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBTTuning: failed finding IBDA_FrequencyFilter topology")));
		return E_FAIL;
	}

	hr = StartChanges(RegisteredDevices[device_index].Control);
	hr = pFreqFilter->put_FrequencyMultiplier(1000L);
//	if(FAILED(hr))
//		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBTTuning: failed put_FrequencyMultiplier")));
	hr = pFreqFilter->put_Bandwidth(Bandwidth); // *1000?????????????
		// removed *1000 to satisfy Airstar2 with Technisat 4.4.1 drivers
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBTTuning: failed put_Bandwidth")));
	hr = pFreqFilter->put_Frequency(Frequency);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBTTuning: failed put_Frequency")));
	hr = CommitChanges(RegisteredDevices[device_index].Control);
	return hr;
}

HRESULT CDVBNetworkProviderFilter::DoDVBCTuning(
		ULONG Frequency,
		LONG SymRate,
		ModulationType ModType)
{
	HRESULT hr;
	IBaseFilter *filt;
	IBDA_FrequencyFilter *pFreqFilter = NULL;
	IBDA_DigitalDemodulator *pDemodFilter = NULL;
	ULONG device_index;

    CAutoLock lock(m_pLock);
	for(device_index=0; device_index<SID; ++device_index)
	{
		hr = RegisteredDevices[device_index].Control->QueryInterface(IID_IBaseFilter, (void **)&filt);
		if(FAILED(hr))
		{
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed getting Control device Interface")));
			return hr;
		}
		hr = GetTopology(filt, IID_IBDA_FrequencyFilter, (void **)&pFreqFilter);
		if(SUCCEEDED(hr))
			break;
	}
	if(device_index == SID)
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed finding IBDA_FrequencyFilter topology")));
		return E_FAIL;
	}
	hr = GetTopology(filt, IID_IBDA_DigitalDemodulator, (void **)&pDemodFilter);
	if(FAILED(hr))
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed finding IBDA_DigitalDemodulator topology")));
		return E_FAIL;
	}

	hr = StartChanges(RegisteredDevices[device_index].Control);
	hr = pFreqFilter->put_FrequencyMultiplier(1000L);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed put_FrequencyMultiplier")));
	hr = pFreqFilter->put_Frequency(Frequency);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed put_Frequency")));
	hr = pDemodFilter->put_ModulationType(&ModType);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed put_ModulationType")));
	hr = pDemodFilter->put_SymbolRate((ULONG *)&SymRate);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBCTuning: failed put_SymbolRate")));
	hr = CommitChanges(RegisteredDevices[device_index].Control);
	return hr;
}

/*
CFactoryTemplate g_Templates[1] = 
{
    { 
      L"My Component",                // Name
      &CLSID_DVBNetworkProviderFilterProp,             // CLSID
      CDVBNetworkProviderFilterProp::CreateInstance,   // Method to create an instance of MyComponent
      NULL,                           // Initialization function
      NULL                            // Set-up information (for filters)
    }
};
int g_cTemplates = sizeof(g_Templates) / sizeof(g_Templates[0]);    
*/

HRESULT CDVBNetworkProviderFilter::PutDVBSPolarity(Polarisation Pol)
{
	HRESULT hr;
	IBaseFilter *filt;
	IBDA_FrequencyFilter *pFreqFilter = NULL;
	ULONG device_index;

	CAutoLock lock(m_pLock);
	for(device_index=0; device_index<SID; ++device_index)
	{
		hr = RegisteredDevices[device_index].Control->QueryInterface(IID_IBaseFilter, (void **)&filt);
		if(FAILED(hr))
		{
			DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed getting Control device Interface")));
			return hr;
		}
		hr = GetTopology(filt, IID_IBDA_FrequencyFilter, (void **)&pFreqFilter);
		if(SUCCEEDED(hr))
			break;
	}
	if(device_index == SID)
	{
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed finding IBDA_FrequencyFilter topology")));
		return E_FAIL;
	}
	hr = pFreqFilter->put_Polarity(Pol);
	if(FAILED(hr))
		DbgLog((LOG_TRACE,0,TEXT("BDA2: CDVBNetworkProviderFilter.DoDVBSTuning: failed put_Polarity")));
	return hr;
}
