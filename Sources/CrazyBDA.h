/*
 * Free DVB-S/S2 BDA Driver
 * =========================
 * Author:		CrazyCat <crazycat69@narod.ru>
 *
 * File:		CrazyBDA.h
 * Description:	Define and register our KsPropertySet so other BDA developers
 *				can use IKsPropertySet to perform tuning operations.
 */

// PropertySet GUID
DEFINE_GUIDSTRUCT( "A3E871E9-1F10-473e-99BD-EE70E0D2F070", KSPROPERTYSET_CrazyTunerControl );
#define KSPROPERTYSET_CrazyTunerControl DEFINE_GUIDNAMED( KSPROPERTYSET_CrazyTunerControl )

// PropertySet Commands
typedef enum
{
	KSPROPERTY_CRAZY_SET_FREQUENCY = 0,
	KSPROPERTY_CRAZY_SET_DISEQC,
	KSPROPERTY_CRAZY_GET_SIGNAL_STATS,
	KSPROPERTY_CRAZY_GET_DEVTYPE,
	KSPROPERTY_CRAZY_GET_I2C,
	KSPROPERTY_CRAZY_SET_POL,
	KSPROPERTY_CRAZY_SET_LED,
} KSPROPERTY_CRAZY_TUNER_COMMAND;

// Port switch selector
typedef enum
{
	CRAZY_PortA = 1,
	CRAZY_PortB,
	CRAZY_PortC,
	CRAZY_PortD
} CRAZY_DiSEqC_Port;

//<summary>
// TUNER_COMMAND
// Structure for BDA KsPropertySet extension
// to support diseqc.
//
// This is what BDA developers will use to
// interact with us.
//</summary>
typedef struct _CRAZY_TUNER_COMMAND
{

	ULONG FrequencyMhz;
	ULONG LOFLowMhz;
	ULONG LOFHighMhz;
	ULONG SwitchFreqMhz;
	ULONG SymbolRateKsps;

	Polarisation SignalPolarisation;	// Polarisation from <bdatypes.h>

	UINT DvbStandard; // 1 or 2 (for S1, S2, or S3 in the future), 0 for Auto
	ModulationType Modulation; // From bdatypes.h
	BinaryConvolutionCodeRate FECRate; // From bdatypes.h
	RollOff S2RollOff; // From bdatypes.h
	Pilot S2Pilot; // From bdatypes.h
	LONG IsId;

	CRAZY_DiSEqC_Port DiSEqC_Switch;

	UINT DiSEqC_Length;
	UCHAR DiSEqC_Command[8];

	UINT StrengthPercent;
	UINT QualityPercent;
	
	BOOL IsLocked;

	LONG RFLevel;/* dBm */
	LONG SNR10;		/* dB, snr * 10 */
	ULONG BER10e7;	/* ber * 10e7 */
} CRAZY_TUNER_COMMAND, *PCRAZY_TUNER_COMMAND;
enum I2CCmd
{
	I2C_Write = 0,
	I2C_Read,
	I2C_WriteRead,
	I2C_WriteReadNoStop
};

#define MAX_I2C 16
typedef struct _I2C_DATA
{
	BYTE Bus;
	BYTE Slave;
	BYTE Len;
	BYTE ReadLen;
	BYTE Buffer[MAX_I2C];
} I2C_DATA, *PI2C_DATA;

enum DevType
{
	Unk = -1,
	Qbox_STV0299 = 0,
	Qbox_STV0288 = 1,
	Qbox2_STV0903 = 2,
	TeVii_ZL10313 = 3,
};
