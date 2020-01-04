#ifndef CALLBACKFILTER_H
#define CALLBACKFILTER_H

//#pragma warning(disable: 4097 4511 4512 4514 4705)

// {260A8904-FA59-4789-80EA-9A4D92BB6A9C}

#include <Streams.h>
#include <mmreg.h>
#include <msacm.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <ks.h>
#include <ksmedia.h>
#include <bdatypes.h>
#include <bdamedia.h>
#include <bdaiface.h>
#include <bdatif.h>
#include <uuids.h>
#include <tuner.h>
#include <commctrl.h>
#include <atlbase.h>
#include <strsafe.h>

#include <initguid.h>
#ifdef SG_USE
#include <qedit.h>
#endif //SG_USE

#include "Dll.h"

#ifdef SG_USE
class CSampleGrabberCB : public ISampleGrabberCB
{
public:
	CSampleGrabberCB () : Callback(NULL), packet_residue_len(0) {};
	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }

	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
	{	if(riid == IID_ISampleGrabberCB || riid == IID_IUnknown)
		{	*ppv = (void *) static_cast<ISampleGrabberCB*> (this);
			return NOERROR;
		}
		return E_NOINTERFACE;
	}

	STDMETHODIMP SampleCB(double t, IMediaSample * s)
	{
		BYTE* buf = NULL;
		s->GetPointer(&buf);
		if(buf == NULL) return 0;
		long l = s->GetSize();
		if (s->IsPreroll() == S_OK) return 0;
		if (s->IsDiscontinuity() == S_OK) return 0;
		OnBufferData( buf, s->GetActualDataLength() );
		return 0;
	}

	STDMETHODIMP BufferCB(double t, BYTE * buf, long len)
	{	return 0;
	}

	HRESULT OnBufferData(BYTE *data, int len);

	void SetStreamCallbackProcedure(STR_CB_PROC);
private:
	STR_CB_PROC Callback;
	BYTE packet_residue[188];
	size_t packet_residue_len;
};
#else
DEFINE_GUID(CLSID_CallbackFilter, 
0x260a8904, 0xfa59, 0x4789, 0x80, 0xea, 0x9a, 0x4d, 0x92, 0xbb, 0x6a, 0x9c);

class CCallbackFilter;

class CCallbackFilterPin : public CRenderedInputPin
{
	CCallbackFilter		* const m_pFilter;		// Main renderer object
    CCritSec			* const m_pReceiveLock;	// Sample critical section

public:
    CCallbackFilterPin(TCHAR *pObjectName,
				CCallbackFilter *pFilter,
				CCritSec *pLock,
				CCritSec *pReceiveLock,
				LPUNKNOWN pUnk,
				HRESULT * phr);

    HRESULT CheckMediaType(const CMediaType* pmt);
    // Do something with this media sample
    STDMETHODIMP Receive(IMediaSample *pSample);
    STDMETHODIMP ReceiveCanBlock();

};

class CCallbackFilter : public CBaseFilter//, ITuneRequestInfo
{

    CCritSec m_Lock;                // Main renderer critical section
    CCritSec m_ReceiveLock;         // Sublock for received samples 
    CCallbackFilterPin *m_pPin;          // A simple rendered input pin
public:
    DECLARE_IUNKNOWN;
    CCallbackFilter(LPUNKNOWN pUnk, HRESULT *pHr);
	~CCallbackFilter();

    // Pin enumeration
    CBasePin * GetPin(int);
    int GetPinCount();
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);

	HRESULT OnBufferData(BYTE* data, int len);
	void LockReader() { m_ReceiveLock.Lock(); }
	void UnlockReader() { m_ReceiveLock.Unlock(); }

	void SetStreamCallbackProcedure(STR_CB_PROC);
/*
	STDMETHODIMP GetLocatorData(ITuneRequest *);
	STDMETHODIMP GetComponentData(ITuneRequest *CurrentRequest);
	STDMETHODIMP CreateComponentList(ITuneRequest *CurrentRequest);
	STDMETHODIMP GetNextProgram(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest);
	STDMETHODIMP GetPreviousProgram(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest);
	STDMETHODIMP GetNextLocator(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest);
	STDMETHODIMP GetPreviousLocator(ITuneRequest *CurrentRequest, ITuneRequest **TuneRequest);
*/
private:
	STR_CB_PROC Callback;
	BYTE packet_residue[188];
	size_t packet_residue_len;
};
#endif

#endif /* CALLBACKFILTER_H */
