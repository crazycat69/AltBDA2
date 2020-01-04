#ifndef OMCBDA_H
#define OMCBDA_H

#include <ks.h>

#define STATIC_KSPROPSETID_OmcCustomProperties\
	0x7DB2DEE6L, 0x42B4, 0x423d, 0xA2, 0xF7, 0x19, 0xC3, 0x2E, 0x51, 0xCC, 0xC1
DEFINE_GUIDSTRUCT("7DB2DEE6-42B4-423d-A2F7-19C32E51CCC1", KSPROPSETID_OmcCustomProperties);
#define KSPROPSETID_OmcCustomProperties DEFINE_GUIDNAMED(KSPROPSETID_OmcCustomProperties)

#define STATIC_KSPROPSETID_OmcI2CProperties\
	0x7DB2DEE9L, 0x42B4, 0x423d, 0xA2, 0xF7, 0x19, 0xC3, 0x2E, 0x51, 0xCC, 0xC1
DEFINE_GUIDSTRUCT("7DB2DEE9-42B4-423d-A2F7-19C32E51CCC1", KSPROPSETID_OmcI2CProperties);
#define KSPROPSETID_OmcI2CProperties DEFINE_GUIDNAMED(KSPROPSETID_OmcI2CProperties)

#define STATIC_KSPROPSETID_OmcDiSEqCProperties\
	0x7DB2DEEAL, 0x42B4, 0x423d, 0xA2, 0xF7, 0x19, 0xC3, 0x2E, 0x51, 0xCC, 0xC1
DEFINE_GUIDSTRUCT("7DB2DEEA-42B4-423d-A2F7-19C32E51CCC1", KSPROPSETID_OmcDiSEqCProperties);
#define KSPROPSETID_OmcDiSEqCProperties DEFINE_GUIDNAMED(KSPROPSETID_OmcDiSEqCProperties)

typedef enum _KSPROPERTY_OMC_CUSTOM
{
	KSPROPERTY_OMC_CUSTOM_SIGNAL_OFFSET	= 0,
	KSPROPERTY_OMC_CUSTOM_SEARCH_MODE,
	KSPROPERTY_OMC_CUSTOM_SEARCH_RANGE,
	KSPROPERTY_OMC_CUSTOM_SEARCH,
	KSPROPERTY_OMC_CUSTOM_SIGNAL_INFO,
	KSPROPERTY_OMC_CUSTOM_STREAM_INFO,
	KSPROPERTY_OMC_CUSTOM_MIS_FILTER,
	KSPROPERTY_OMC_CUSTOM_RFSCAN,
	KSPROPERTY_OMC_CUSTOM_IQSCAN,
	KSPROPERTY_OMC_CUSTOM_PLS_SCRAM,
} KSPROPERTY_OMC_CUSTOM;

typedef enum _OMC_BDA_SEARCH_MODE
{
	OMC_BDA_COLD_LOCK = 0,	/* only the SR is known */
	OMC_BDA_WARM_LOCK,		/* offset freq and SR are known */
	OMC_BDA_BLIND_SCAN		/* offset freq and SR are Unknown */
}OMC_BDA_SEARCH_MODE;

typedef struct _OMC_BDA_SEARCH_PARAMS
{
	OMC_BDA_SEARCH_MODE		SearchMode;	/* Search mode */
	ULONG					SearchRange;/* Search range in Khz, 1000-10000 Khz */

	ULONG					LofLow;		/* Freq in Khz */
	ULONG					LofHigh;	/* Freq in Khz */
	ULONG					LofSwitch;	/* Freq in Khz */

	ULONG						Freq;	/* Freq in Khz */
	Polarisation				Pol;	/* Polarization */
	ULONG						SR;		/* Symbol Rate in Ks, for BlindScan used as Minimal SR */
	ModulationType				Mod;	/* Standard/Modulation */
	BinaryConvolutionCodeRate	Fec;	/* FEC, for BlindScan not used*/
}OMC_BDA_SEARCH_PARAMS, *POMC_BDA_SEARCH_PARAMS;

typedef struct _OMC_BDA_SEARCH_RESULTS
{
	BOOL	Lock;	/* Carrier lock */

	ULONG						Freq;	/* Freq in Khz */
	ULONG						SR;		/* Symbol Rate in Ks */
	ModulationType				Mod;	/* Modulation */
	BinaryConvolutionCodeRate	Fec;	/* FEC */
	SpectralInversion			Inv;	/* Spectral inversion */
	RollOff						RollOff;/* RollOff factor */
	Pilot						Pilot;	/* Pilot factor */

	ULONG	CarrierWidth;	/* Carrier width in Khz */

	LONG	RFLevel;		/* dBm */
	LONG	SNR10;			/* dB, snr * 10 */
	LONG	BER10e7;		/* ber * 10e7 */
}OMC_BDA_SEARCH_RESULTS, *POMC_BDA_SEARCH_RESULTS;

typedef struct _OMC_BDA_SIGNAL_INFO
{
	BOOL	Present;
	BOOL	Lock;
	LONG	RFLevel;	/* dBm */
	LONG	SNR10;		/* dB, snr * 10 */
	LONG	BER10e7;	/* ber * 10e7 */
}OMC_BDA_SIGNAL_INFO, *POMC_BDA_SIGNAL_INFO;

typedef enum _OMC_BDA_STREAM_TYPE
{
	OMC_BDA_TRANSPORT_STREAM = 0,
	OMC_BDA_GENERIC_CONTINOUS_STREAM,
	OMC_BDA_GENERIC_PACKETIZED_STREAM,
	OMC_BDA_RESERVED_STREAM
}OMC_BDA_STREAM_TYPE;

typedef enum _OMC_BDA_CODING_TYPE
{
	OMC_BDA_CCM = 0,
	OMC_BDA_ACM,
	OMC_BDA_VCM = OMC_BDA_ACM
}OMC_BDA_CODING_TYPE;

typedef enum _OMC_BDA_FRAME_LEN
{
	OMC_BDA_FRAME_LONG = 0,
	OMC_BDA_FRAME_SHORT
}OMC_BDA_FRAME_LEN;

#define MAX_MIS 16
typedef struct _OMC_BDA_STREAM_INFO
{
	OMC_BDA_STREAM_TYPE	StreamType;
	OMC_BDA_CODING_TYPE	CodingType;
	OMC_BDA_FRAME_LEN	FrameLen;
	ULONG	MISCount;			/* 0 - single stream, >0 - count of input streams */
	BYTE	MISID[MAX_MIS];		/* Input streams IDs */
	BOOL	ISSYI;				/* Input Stream Synchronisation */
	BOOL	NPD;				/* Null Packet Deletion */
} OMC_BDA_STREAM_INFO, *POMC_BDA_STREAM_INFO;

typedef struct _OMC_BDA_RFSCAN_PARAM
{
	ULONG			Freq;		/* Freq in Khz */
	ULONG			LofLow;		/* Freq in Khz */
	ULONG			LofHigh;	/* Freq in Khz */
	ULONG			LofSwitch;	/* Freq in Khz */
	Polarisation	Pol;		/* Polarization */
	BOOL			RawAGC;
} OMC_BDA_RFSCAN_PARAM, *POMC_BDA_RFSCAN_PARAM;

typedef enum _OMC_BDA_IQSCAN_POINT
{
	OMC_BDA_IQSCAN_DEMOD_OUT = 0,	/* Demod IQ out */
	OMC_BDA_IQSCAN_EQUAL_OUT,		/* Equalizer out */
	OMC_BDA_IQSCAN_DEROT2_OUT,		/* Derotator 2 out */
	OMC_BDA_IQSCAN_SYM_INTSYM_OUT,	/* Symbols & Inter Symbols out */
	OMC_BDA_IQSCAN_SYM_OUT,			/* Symbols out */
	OMC_BDA_IQSCAN_INTSYM_OUT,		/* Inter Symbols out */
	OMC_BDA_IQSCAN_DEROT1_OUT,		/* Derotator 2 out */
	OMC_BDA_IQSCAN_IQMISM_OUT,		/* IQ mismatches out */
	OMC_BDA_IQSCAN_DEMOD_IN,		/* Demod input */
} OMC_BDA_IQSCAN_POINT;

#define OMC_BDA_IQSAMPLES 100

typedef struct _OMC_BDA_PLS_PARAM
{
	UINT	PLSMode;
	DWORD	PLSCode;
} OMC_BDA_PLS_PARAM, *POMC_BDA_PLS_PARAM;

typedef enum _KSPROPERTY_OMC_DISEQC
{
	KSPROPERTY_OMC_DISEQC_WRITE	= 0,
	KSPROPERTY_OMC_DISEQC_READ,				
	KSPROPERTY_OMC_DISEQC_SET22K,
	KSPROPERTY_OMC_DISEQC_ENCABLOSSCOMP,
	KSPROPERTY_OMC_DISEQC_TONEBURST
}KSPROPERTY_OMC_DISEQC;

typedef struct _OMC_BDA_DISEQC_DATA
{
	ULONG			nLen;
	UCHAR			pBuffer[64];
	ULONG			nRepeatCount;
}OMC_BDA_DISEQC_DATA, *POMC_BDA_DISEQC_DATA;

typedef enum _KSPROPERTY_OMC_I2C
{
	KSPROPERTY_OMC_I2C_WRITE = 1,
	KSPROPERTY_OMC_I2C_READ
}KSPROPERTY_OMC_I2C;

typedef struct _OMC_BDA_I2C_DATA
{
	UCHAR			ucSlave;
	UCHAR			pBuffer[512];
	ULONG			nLen;
	BOOLEAN			bLocked;
} OMC_BDA_I2C_DATA, *POMC_BDA_I2C_DATA;

#endif //OMCBDA_H
