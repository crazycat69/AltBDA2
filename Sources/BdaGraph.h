#ifndef BDAGRAPH_H
#define BDAGRAPH_H

#pragma warning( disable : 4995 4996 ) // no depreciated warnings

#include "CallbackFilter.h"
#include "NetworkProvider.h"
#include "Configuration.h"

#include <ksproxy.h>

#include <stdio.h>

#include "dwBdaApi.h"
#include <ttBdaDrvApi.h>
#include "THIOCtrl.h"
#include "omcbda.h"

#define Z(a) ZeroMemory(&a, sizeof(a))

typedef enum _SpecDevType {
	UNDEF,
	OMC2,
	TT1600,
	PROF7301,
	PROF8000,
	PROF7500,
	TBS6925V1,
	TBS6925V2,
	TBS5920,
	TBS5980,
	TBS5925,
	TBS6926
} SpecDevType;

// defaults for tune request
#define DEFAULT_FREQUENCY_C		394000L
#define DEFAULT_FREQUENCY_S		10700000L
#define DEFAULT_FREQUENCY_T		722000L
#define DEFAULT_SYMBOLRATE_C	6900L
#define DEFAULT_SYMBOLRATE_S	30000L
#define DEFAULT_SYMBOLRATE_T	0L
#define DEFAULT_MODULATION_C	BDA_MOD_64QAM
#define DEFAULT_MODULATION_S	BDA_MOD_QPSK
#define DEFAULT_MODULATION_T	BDA_MOD_64QAM
#define DEFAULT_INVERSION_C		BDA_SPECTRAL_INVERSION_INVERTED
#define DEFAULT_INVERSION_S		BDA_SPECTRAL_INVERSION_AUTOMATIC
#define DEFAULT_INVERSION_T		BDA_SPECTRAL_INVERSION_NORMAL
#define DEFAULT_POLARISATION	BDA_POLARISATION_LINEAR_H
#define DEFAULT_FEC				BDA_BCC_RATE_NOT_SET
#define DEFAULT_LOW_OSCILLATOR	9750000L
#define DEFAULT_HIGH_OSCILLATOR	10600000L
#define DEFAULT_LNB_SWITCH		11700000L
#define DEFAULT_POS_OPT			-1L
#define DEFAULT_BANDWIDTH		BDA_BW_AUTO
#define DEFAULT_ONID			-1L
#define DEFAULT_SID 			-1L
#define DEFAULT_TSID			-1L

struct NetworkTuners
{
	int Count;
	struct
	{
		GUID Type;
		char Name[128];
		int Id;
		BOOLEAN Availability;
	} Tuner[MAX_DEVICES];
};

class CBdaGraph
{
public:
	CBdaGraph();
	~CBdaGraph();
	
	VENDOR_SPECIFIC	BdaType;
	BSTR friendly_name_tuner;
	void MessageCallback(MSG_CB_PROC message_callback);

	HRESULT GetNetworkTuners(struct NetworkTuners *);
	HRESULT GetNetworkTunerType(IBaseFilter *pFilter, GUID *type);
	HRESULT FindTunerFilter(int dev_no, BSTR *bStr, IBaseFilter **pFilter);
	HRESULT BuildGraph(int, enum VENDOR_SPECIFIC *);
	HRESULT RunGraph(void);
	void CloseGraph(void);

	HRESULT DVBS_SetPolarity(Polarisation Pol);
	HRESULT DVBS_Tune(
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
	HRESULT DVBS_MS_Tune(
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
		ULONG LNBSource,
		BOOL bToneBurst);
	HRESULT DVBS_OMC_Tune(
		ULONG LowBandF,
		ULONG HighBandF,
		ULONG SwitchF,
		ULONG Frequency,
		SpectralInversion SpectrInv,
		ModulationType ModType,
		LONG SymRate,
		Polarisation Pol,
		BinaryConvolutionCodeRate Fec,
		int MIS, int pls_mode, int pls_code);
	HRESULT DVBS_Crazy_Tune(
		ULONG LowBandF,
		ULONG HighBandF,
		ULONG SwitchF,
		ULONG Frequency,
		SpectralInversion SpectrInv,
		ModulationType ModType,
		LONG SymRate,
		Polarisation Pol,
		BinaryConvolutionCodeRate Fec,
		int MIS, int pls_mode, int pls_code);
	HRESULT DVBS_TT_Tune(
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
	HRESULT DVBS_DW_Tune(
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
		UINT tone_burst);
	HRESULT DVBS_HAUP_Tune(
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
		LONG PosOpt);
	HRESULT DVBS_CXT_Tune(
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
		LONG PosOpt);
	HRESULT DVBT_Tune(
		ULONG Frequency,
		ULONG Bandwidth);
	HRESULT DVBT_MS_Tune(
		ULONG Frequency,
		ULONG Bandwidth, LONG Plp);
	HRESULT DVBT_Astrometa_Tune(
		ULONG Frequency,
		ULONG Bandwidth, int Mode, LONG Plp);
	HRESULT DVBC_Tune(
		ULONG Frequency,
		LONG SymRate,
		ModulationType ModType);

	HRESULT DVBS_GetSignalsStat(BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality);
	
	HRESULT DVBS_TT_GetSignalsStat(BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality);
	BOOL DVBS_TT_GetProdName( char* pszProdName, size_t len );
	HRESULT DVBS_TT_DiSEqC(BYTE len, BYTE *DiSEqC_Command, BYTE tb);

	HRESULT DVBS_MS_DiSEqC(BYTE len, BYTE *DiSEqC_Command, BYTE repeat);

	HRESULT DVBS_BST_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_HAUP_DiSEqC(BYTE len, BYTE *DiSEqC_Command);

	HRESULT DVBS_CXT_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_CXT_LNBPower (BOOL bPower);

	HRESULT DVBS_QBOX_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_QBOX_Set22Khz(BOOL b22Khz);
	HRESULT DVBS_QBOX_LNBPower (BOOL bPower);
	HRESULT DVBS_QBOX_I2CWrite ( BYTE SlaveAddress, BYTE* pBuffer, ULONG Len );
	HRESULT DVBS_QBOX_I2CRead ( BYTE SlaveAddress, BYTE* pBuffer, ULONG Len );
	HRESULT DVBS_QBOX_I2CWriteRead ( BYTE SlaveAddress, BYTE* pBuffer, ULONG Len, BYTE* pReadBuffer, ULONG ReadLen );

	HRESULT DVBS_TBS_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_TBS_Set22Khz(BOOL b22Khz);
	HRESULT DVBS_TBS_LNBPower (BOOL bPower);
	HRESULT DVBS_TBS_SetPol(Polarisation ePol);
	HRESULT DVBS_TBS_SetMIS(LONG mis);
	HRESULT DVBS_TBS_SetPLP(LONG plp);
	HRESULT DVBS_TBS_SetPLS( UINT PLSMode, DWORD PLSCode );

	HRESULT DVBS_TBS_NXP_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	BYTE	DVBS_TBS_NXP_I2C_BusNum();
	HRESULT DVBS_TBS_NXP_I2CWrite (BYTE SlaveAddress, BYTE* pSubAddr, ULONG SubAddrLen, BYTE* pBuffer, ULONG Len );
	HRESULT DVBS_TBS_NXP_I2CRead (BYTE SlaveAddress, BYTE* pSubAddr, ULONG SubAddrLen, BYTE* pBuffer, ULONG Len );

	HRESULT DVBS_TBS_STV090X_HWInit(BOOL FastTune, BOOL MIS, BOOL MIS00, BYTE MISID);
	HRESULT DVBS_TBS_STV090X_HWTerm();
	HRESULT DVBS_TBS_STV090X_Tune(
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
		int MIS, int pls_mode, int pls_code);
	HRESULT DVBS_TBS_STV090X_GetSignalsStat(int LevelsType, BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality);

	HRESULT DVBS_TeVii_GetSignalsStat(BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality);
	HRESULT DVBS_TeVii_DiSEqC(BYTE len, BYTE *DiSEqC_Command);

	HRESULT DVBS_TH_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_TH_LNBPower(BOOL bPower);
	HRESULT DVBS_TH_LNBSource (BYTE Port, BOOL bToneBurst);

	HRESULT DVBS_DW_DiSEqC(BYTE len, BYTE *DiSEqC_Command);

	HRESULT DVBS_OMC2_GetSignalsStat(int LevelsType, BOOLEAN *pPresent, BOOLEAN *pLocked, LONG *pStrength, LONG *pQuality);	
	HRESULT DVBS_OMC2_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_OMC2_ToneBurst(BOOL bToneBurst);
	HRESULT DVBS_OMC2_Set22Khz(BOOL b22Khz);	

	HRESULT DVBS_OMC2_SetMIS(LONG mis);
	HRESULT DVBS_OMC2_SetPLS( UINT PLSMode, DWORD PLSCode );
	HRESULT DVBS_OMC2_SetSearchRange(ULONG ulSearchRange);
	HRESULT DVBS_OMC2_SetSearchMode(OMC_BDA_SEARCH_MODE SearchMode);
	HRESULT DVBS_OMC2_Search( ULONG LOF1,
 							  ULONG LOF2,
							  ULONG LOFSW,
							  ULONG Freq,
							  ULONG SR,
							  ULONG SearchRange,
							  OMC_BDA_SEARCH_MODE SearchMode,
							  Polarisation Pol,
							  ModulationType Mod,
							  BinaryConvolutionCodeRate Fec);

	HRESULT DVBS_Compro_LNBPower(BOOL bPower);
	HRESULT DVBS_Compro_Set22Khz(BOOL b22Khz);
	HRESULT DVBS_Compro_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_AnySee_DiSEqC(BYTE len, BYTE *DiSEqC_Command, BYTE ToneBurst);
	HRESULT DVBS_GenpixOld_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_Genpix_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_Genpix_ToneBurst(BOOL bToneBurst);
	HRESULT DVBS_Netup_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_Crazy_DiSEqC(BYTE len, BYTE *DiSEqC_Command);
	HRESULT DVBS_Crazy_ToneBurst(BOOL bToneBurst);
	HRESULT DVBS_Crazy_LNBPower(BOOL bPower);

	HRESULT DVBT_Astrometa_SetPLP(LONG Plp);
	HRESULT DVBT_Astrometa_SetMode(DWORD Mode);

	void SetStreamCallbackProcedure(STR_CB_PROC);

private:
	HRESULT AttachFilter(
		REFCLSID clsid,
		IBaseFilter** ppFilter,
		PCHAR szMatchName = NULL
		);
	HRESULT GetTunerPath(int idx, char* pTunerPath);
	HRESULT AddGraphToROT(IUnknown *pUnkGraph, DWORD *pdwRegister);
	HRESULT RemoveGraphFromROT(DWORD pdwRegister);
	void ReportMessage(char *text);
	HRESULT GetPin( IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin);
	IPin *GetInPin( IBaseFilter * pFilter, int nPin );
	IPin *GetOutPin( IBaseFilter * pFilter, int nPin );
    HRESULT GetPinMedium(IPin* pPin, REGPINMEDIUM* pMedium);

	DWORD			m_dwGraphRegister;		//registration number for the RunningObjectTable
	IFilterGraph2	*m_pFilterGraph;		// for current graph
	IBaseFilter		*m_pTunerDevice;		// for tuner device filter
	IBaseFilter		*m_pNetworkProvider;	// network provider filter
	IBaseFilter		*m_pReceiver;			// receiver
	IBaseFilter		*m_pCallbackFilter;		// calbback filter
	IMediaControl	*m_pMediaControl;		// media control	
#ifdef SG_USE
	CSampleGrabberCB *pCallbackInstance;	// SampleGrabber callback object
#else
	CCallbackFilter	*pCallbackInstance;		// callback filter object
#endif //SG_USE
	IKsPropertySet	*m_pKsTunerFilterPropSet; // Tuner filter proprietary interface
	IKsPropertySet	*m_pKsCaptureFilterPropSet; // Capture filter proprietary interface
	IKsPropertySet	*m_pKsTunerPropSet;		// Tuner proprietary interface
	IKsPropertySet	*m_pKsDemodPropSet;		// Demod proprietary interface
	IKsControl		*m_pKsDeviceControl;	// IKsControl for device
	IKsPropertySet	*m_pKsVCPropSet;		// IKsPropertySet for Video Capture	

	CDVBNetworkProviderFilter *pNetworkProviderInstance; // network provider object

	MSG_CB_PROC message_callback;
	DEVICE_CAT TTDevCat;
	HANDLE hTT, hDW;
	BOOL bTVDLL;
	int iTVIdx;

	//THBDA Ioctl functions
	BOOL THBDA_IOControl( DWORD  dwIoControlCode,
		LPVOID lpInBuffer,
		DWORD  nInBufferSize,
		LPVOID lpOutBuffer,
		DWORD  nOutBufferSize,
		LPDWORD lpBytesReturned);
	BOOL THBDA_IOCTL_CHECK_INTERFACE_Fun(void);
	BOOL THBDA_IOCTL_SET_LNB_DATA_Fun(LNB_DATA *pLNB_DATA);
	BOOL THBDA_IOCTL_GET_LNB_DATA_Fun(LNB_DATA *pLNB_DATA);
	BOOL THBDA_IOCTL_SET_DiSEqC_Fun(DiSEqC_DATA *pDiSEqC_DATA);
	BOOL THBDA_IOCTL_GET_DiSEqC_Fun(DiSEqC_DATA *pDiSEqC_DATA);
};

#endif /* BDAGRAPH_H */
