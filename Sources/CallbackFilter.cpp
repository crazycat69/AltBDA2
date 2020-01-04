#include "stdafx.h"

#include "CallbackFilter.h"

#ifdef SG_USE
HRESULT CSampleGrabberCB::OnBufferData(BYTE *data, int len)
{
	int i;

	if(Callback && len>=188)
	{
//		DbgLog((LOG_TRACE,0,TEXT("Callback: len=%d\n", len)));

		i = 0;
		// we have packet residue from previous IMediaSample?
		if(packet_residue_len)
		{
			if(data[0] == 0x47)
				packet_residue_len = 0;
			else
			{
				for(;packet_residue_len!=188;++i,++packet_residue_len)
					packet_residue[packet_residue_len] = data[i];
				Callback(packet_residue);
				packet_residue_len = 0;
			}
		}
		// process IMediaSample
		while(len-i >= 188)
		{
			if(data[i] == 0x47)
			{
				memcpy(packet_residue,data+i,188);
				Callback(packet_residue);
				//Callback(data+i);
				i+=188;
			}
			else
				++i;
		}
		// put leftover from IMediaSample into residue for next time
		for(;i<len;++i)
			if(data[i]==0x47)
				for(;i<len;++i,++packet_residue_len)
					packet_residue[packet_residue_len] = data[i];
	}
	return S_OK;
}

void CSampleGrabberCB::SetStreamCallbackProcedure(STR_CB_PROC proc)
{
	Callback = proc;
}
#else
CCallbackFilterPin::CCallbackFilterPin(TCHAR *pObjectName,
		CCallbackFilter *pFilter, CCritSec *pLock, CCritSec *pReceiveLock,
		LPUNKNOWN pUnk, HRESULT * phr) :
    CRenderedInputPin((TCHAR *)NAME("CallbackFilterInputPin"),
                  pFilter,                   // Filter
                  pLock,                     // Locking
                  phr,                       // Return code
                  (LPCWSTR)L"Input"),        // Pin name
		m_pReceiveLock(pReceiveLock),
		m_pFilter(pFilter)
{
}

// Check if the pin can support this specific proposed type and format
//
HRESULT CCallbackFilterPin::CheckMediaType(const CMediaType *m)
{
	CheckPointer(m,E_POINTER);

	if(! IsEqualGUID(m->majortype, MEDIATYPE_Stream))
		return VFW_E_INVALIDMEDIATYPE;
	if(!IsEqualGUID(m->subtype, KSDATAFORMAT_SUBTYPE_BDA_MPEG2_TRANSPORT) &&
		!IsEqualGUID(m->subtype, MEDIASUBTYPE_MPEG2_TRANSPORT) &&
		!IsEqualGUID(m->subtype, KSDATAFORMAT_TYPE_MPEG2_TRANSPORT))
		return VFW_E_INVALIDSUBTYPE;
    return S_OK;
}

// We don't hold up source threads on Receive
STDMETHODIMP CCallbackFilterPin::ReceiveCanBlock()
{
	return S_FALSE;
    //return S_OK;
}

STDMETHODIMP CCallbackFilterPin::Receive(IMediaSample *pSample)
{
    CheckPointer(pSample,E_POINTER);
    CAutoLock lock(m_pReceiveLock);
	PBYTE pbData;

	HRESULT hr = pSample->GetPointer(&pbData);
	if (FAILED(hr)) {
		return hr;
	}

    return m_pFilter->OnBufferData(pbData, pSample->GetActualDataLength());
}


CCallbackFilter::CCallbackFilter(LPUNKNOWN pUnk, HRESULT *pHr) :
	CBaseFilter(NAME("CCallbackFilter"), pUnk, &m_Lock, CLSID_CallbackFilter),
	Callback(NULL), packet_residue_len(0)
{
    m_pPin = new CCallbackFilterPin(NAME("CCallbackFilterPin"),
                               this,
                               &m_Lock,
                               &m_ReceiveLock,
                               GetOwner(),
							   pHr);
    if (m_pPin == NULL) {
        if (pHr)
            *pHr = E_OUTOFMEMORY;
        return;
	}
}

CCallbackFilter::~CCallbackFilter()
{
}

CBasePin * CCallbackFilter::GetPin(int n)
{
    if (n == 0) {
        return m_pPin;
    } else {
        return NULL;
    }
}

int CCallbackFilter::GetPinCount()
{
    return 1;
}

HRESULT CCallbackFilter::OnBufferData(BYTE* data, int len)
{
	int i;

    CAutoLock lock(m_pLock);

	if(Callback && len>=188)
	{
//		DbgLog((LOG_TRACE,0,TEXT("Callback: len=%d\n", len)));

		i = 0;
		// we have packet residue from previous IMediaSample?
		if(packet_residue_len)
		{
			if(data[0] == 0x47)
				packet_residue_len = 0;
			else
			{
				for(;packet_residue_len!=188;++i,++packet_residue_len)
					packet_residue[packet_residue_len] = data[i];
				Callback(packet_residue);
				packet_residue_len = 0;
			}
		}
		// process IMediaSample
		while(len-i >= 188)
		{
			if(data[i] == 0x47)
			{
				memcpy(packet_residue,data+i,188);
				Callback(packet_residue);
				//Callback(data+i);
				i+=188;
			}
			else
				++i;
		}
		// put leftover from IMediaSample into residue for next time
		for(;i<len;++i)
			if(data[i]==0x47)
				for(;i<len;++i,++packet_residue_len)
					packet_residue[packet_residue_len] = data[i];
	}
	return S_OK;
}

void CCallbackFilter::SetStreamCallbackProcedure(STR_CB_PROC proc)
{
	Callback = proc;
}

STDMETHODIMP CCallbackFilter::NonDelegatingQueryInterface(REFIID riid, void **ppv)
{
    CheckPointer(ppv,E_POINTER);
    CAutoLock lock(m_pLock);

/*	if (riid == IID_ITuneRequestInfo) {
        HRESULT hr = GetInterface((ITuneRequestInfo *) this, ppv);
		return hr;
	} else {*/
        // Pass the buck
        HRESULT hr = CBaseFilter::NonDelegatingQueryInterface(riid, ppv);
		return hr;
//   }
} // NonDelegatingQueryInterface

/*
STDMETHODIMP CCallbackFilter::GetLocatorData(ITuneRequest *Request)
{
    CAutoLock lock(m_pLock);
	return S_OK;
}
STDMETHODIMP CCallbackFilter::GetComponentData(ITuneRequest *CurrentRequest)
{
    CAutoLock lock(m_pLock);
	return S_OK;
}
STDMETHODIMP CCallbackFilter::CreateComponentList(ITuneRequest *CurrentRequest)
{
    CAutoLock lock(m_pLock);
	return S_OK;
}
STDMETHODIMP CCallbackFilter::GetNextProgram(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest)
{
    CAutoLock lock(m_pLock);
	return E_NOTIMPL;
}
STDMETHODIMP CCallbackFilter::GetPreviousProgram(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest)
{
    CAutoLock lock(m_pLock);
	return E_NOTIMPL;
}
STDMETHODIMP CCallbackFilter::GetNextLocator(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest)
{
    CAutoLock lock(m_pLock);
	return E_NOTIMPL;
}
STDMETHODIMP CCallbackFilter::GetPreviousLocator(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest)
{
    CAutoLock lock(m_pLock);
	return E_NOTIMPL;
}
*/
#endif //SG_USE
