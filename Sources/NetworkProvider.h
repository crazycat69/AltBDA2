#ifndef NETWORKPROVIDER_H
#define NETWORKPROVIDER_H

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

// {88600304-7567-49fc-B601-5B7062CB40ED}
DEFINE_GUID(CLSID_DVBNetworkProviderFilter, 
0x88600304, 0x7567, 0x49fc, 0xb6, 0x1, 0x5b, 0x70, 0x62, 0xcb, 0x40, 0xed);
// {5C9219C5-1CAD-4492-B490-CFBEBFCA4906}
DEFINE_GUID(CLSID_DVBNetworkProviderFilterProp, 
0x5c9219c5, 0x1cad, 0x4492, 0xb4, 0x90, 0xcf, 0xbe, 0xbf, 0xca, 0x49, 0x6);


class CDVBNetworkProviderFilter;

class CDVBNetworkProviderPin : public CBaseOutputPin
{
private:
	CDVBNetworkProviderFilter *pFilter;

public:
    HRESULT CheckMediaType(const CMediaType* pmt);
    HRESULT CompleteConnect(IPin *ReceivePin);
    HRESULT Run(REFERENCE_TIME tStart);
	CDVBNetworkProviderPin(CDVBNetworkProviderFilter *pFilter, CCritSec *pLock, HRESULT *phr);
	HRESULT DecideBufferSize(IMemAllocator *pAlloc, ALLOCATOR_PROPERTIES *ppropInputRequest);
};

class CDVBNetworkProviderFilter : public CBaseFilter, IBDA_NetworkProvider//, ITuner//,
									//ISpecifyPropertyPages, IDVBNetworkProviderFilter
{
private:
	CBaseOutputPin *pin;
	CCritSec m_pPinLock;
	CCritSec m_pSetupLock;

#define MAX_TOPOLOGY_NODES 10
#define MAX_REGISTERED_DEVICES 10
	struct {
		ULONG ID;
		BSTR Name;
		IUnknown *Control;
	} RegisteredDevices[MAX_REGISTERED_DEVICES];
	ULONG SID;

public:
	DECLARE_IUNKNOWN;

    CDVBNetworkProviderFilter(LPUNKNOWN pUnk, HRESULT *phr);

    // Pin enumeration
    CBasePin * GetPin(int n);
    int GetPinCount();
    STDMETHODIMP NonDelegatingQueryInterface(REFIID riid, void ** ppv);
    // --- ISpecifyPropertyPages ---
    // return our property pages
    //STDMETHODIMP GetPages(CAUUID * pPages);
    // IDVBNetworkproviderFilter - private interface to put/set properties
    //STDMETHODIMP get_Frequency(int *frequency);
    //STDMETHODIMP put_Frequency(int frequency);
    // IBDA_NetworkProvider
    STDMETHODIMP PutSignalSource(ULONG ulSignalSource);
    STDMETHODIMP GetSignalSource(ULONG *pulSignalSource);
    STDMETHODIMP GetNetworkType(GUID *pguidNetworkType);
    STDMETHODIMP PutTuningSpace(REFGUID guidTuningSpace);
    STDMETHODIMP GetTuningSpace(GUID *pguidTuingSpace);
    STDMETHODIMP RegisterDeviceFilter(IUnknown *pUnkFilterControl, ULONG *ppvRegisitrationContext);
    STDMETHODIMP UnRegisterDeviceFilter(ULONG pvRegistrationContext);
	// ITuner
/*	STDMETHODIMP get_TuningSpace( ITuningSpace **TuningSpace);
	STDMETHODIMP put_TuningSpace(ITuningSpace *TuningSpace);
	STDMETHODIMP EnumTuningSpaces(IEnumTuningSpaces **ppEnum);
	STDMETHODIMP get_TuneRequest(ITuneRequest **TuneRequest);
	STDMETHODIMP put_TuneRequest(ITuneRequest *TuneRequest);
	STDMETHODIMP Validate(ITuneRequest *TuneRequest);
	STDMETHODIMP get_PreferredComponentTypes(IComponentTypes **ComponentTypes);
	STDMETHODIMP put_PreferredComponentTypes(IComponentTypes *ComponentTypes);
	STDMETHODIMP get_SignalStrength(long *Strength);
	STDMETHODIMP TriggerSignalEvents(long Interval);*/

	HRESULT DoDVBSTuning(
		ULONG LowBandF,
		ULONG HighBandF,
		ULONG SwitchF,
		ULONG Frequency,
		SpectralInversion SpectrInv,
		ModulationType ModType,
		LONG SymRate,
		Polarisation Pol,
		BinaryConvolutionCodeRate Fec,
		LONG PosOpt);
	HRESULT DoDVBTTuning(
		ULONG Frequency,
		ULONG Bandwidth);
	HRESULT DoDVBCTuning(
		ULONG Frequency,
		LONG SymRate,
		ModulationType ModType);
	HRESULT GetSignalStatistics(BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality);
	HRESULT PutDVBSPolarity(Polarisation Pol);

private:
	LONG previous_PosOpt;
	IBDA_SignalStatistics *pSignalPresent;
	IBDA_SignalStatistics *pSignalLocked;
	IBDA_SignalStatistics *pSignalStrength;
	IBDA_SignalStatistics *pSignalQuality;


	HRESULT CreateOutputPin(IPin *pin);
	HRESULT GetMultipleTopology(IBaseFilter *pFilter, GUID Topology, int *node_cnt, void **pIUnknown);
	HRESULT GetTopology(IBaseFilter *pFilter, GUID Topology, void **pIUnknown);
	HRESULT StartChanges(IUnknown *Control);
	HRESULT CommitChanges(IUnknown *Control);

};

#endif /* NETWORKPROVIDER_H */