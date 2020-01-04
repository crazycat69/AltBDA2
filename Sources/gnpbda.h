#ifndef GNPBDA_H
#define GNPBDA_H

/******* 3dparty Genpix SW1,2 BDA driver from Shree Ganesha Inc. *******/

//Used to extend the feature of the BDA
//{0B5221EB-F4C4-4976-B959-EF74427464D9}
DEFINE_GUIDSTRUCT("0B5221EB-F4C4-4976-B959-EF74427464D9", KSPROPSETID_GnpBdaExtendedProperty);
#define KSPROPSETID_GnpBdaExtendedProperty DEFINE_GUIDNAMED(KSPROPSETID_GnpBdaExtendedProperty)

//Extended Property List
typedef enum __KSPROPERTY_EXTENDED
{
	/* DiSEqC Command */
	GNP_BDA_DISEQC = 0 //Extension Property 1
}KSPROPERTY_GNP_EXTENDED;

//Enumeration can be used during Simple Tone Burst Command
typedef enum __SIMPLE_TONE_BURST 
{
	SEC_MINI_A,
	SEC_MINI_B
}SIMPLE_TONE_BURST;

#define MAX_DISEQC_COMMAND_LENGTH 6

//DiSEqC Command related Structure definitions
typedef struct _DISEQC_COMMAND {
	BYTE ucMessage[MAX_DISEQC_COMMAND_LENGTH];
	BYTE ucMessageLength;
}GNP_DISEQC_CMD, *PGNP_DISEQC_CMD;

/******* New Genpix BDA driver *******/
// PropertySet GUID
DEFINE_GUIDSTRUCT( "DF981009-0D8A-430e-A803-17C514DC8EC0", KSPROPERTYSET_GnpTunerControl );
#define KSPROPERTYSET_GnpTunerControl DEFINE_GUIDNAMED( KSPROPERTYSET_GnpTunerControl )

// PropertySet Commands
typedef enum
{
    KSPROPERTY_SET_FREQUENCY,
	KSPROPERTY_SET_DISEQC,
	KSPROPERTY_GET_SIGNAL_STATS

} KSPROPERTY_TUNER_COMMAND;

// Port switch selector
typedef enum
{
	None = 0,
	PortA = 1,
	PortB = 2,
	PortC = 3,
	PortD = 4,
	BurstA = 5,
	BurstB = 6,
	SW21_Dish_1 = 7,
	SW21_Dish_2 = 8,
	SW42_Dish_1 = 9,
	SW42_Dish_2 = 10,
	SW44_Dish_2 = 11,
	SW64_Dish_1A = 12,
	SW64_Dish_1B = 13,
	SW64_Dish_2A = 14,
	SW64_Dish_2B = 15,
	SW64_Dish_3A = 16,
	SW64_Dish_3B = 17,
	Twin_LNB_1 = 18,
	Twin_LNB_2 = 19,
	Quad_LNB_2 = 20
} DiSEqC_Port;

//
// TUNER_COMMAND
// Structure for BDA KsPropertySet extension
// to support diseqc.
//
// This is what BDA developers will use to
// interact with us.
//
typedef struct _TUNER_COMMAND
{
	// Actual tuned Frequency (if tune is succesfull) is returned as FrequencyMhz
	// during every KSPROPERTY_GET_SIGNAL_STATS call
	ULONG FrequencyMhz;
	ULONG LOFLowMhz;
	ULONG LOFHighMhz;
	ULONG SwitchFreqMhz;	// use SwitchFreqMhz = DishProLNB for DishPro LNBs
	ULONG SymbolRateKsps;

	Polarisation SignalPolarisation;	// Polarisation from <bdatypes.h>

	ModulationType Modulation;			// ModulationType from <bdatypes.h>
	BinaryConvolutionCodeRate FECRate;	// BinaryConvolutionCodeRate from <bdatypes.h>

	DiSEqC_Port DiSEqC_Switch;
	UINT DiSEqC_Repeats;

	UINT DiSEqC_Length;			// these three parameters are used only by
	UCHAR DiSEqC_Command[8];	// KSPROPERTY_SET_DISEQC function, they are ignored when 
	BOOL ForceHighVoltage;		// standard BDA calls (or KSPROPERTY_SET_FREQUENCY) are used

	UINT StrengthPercent;		// KSPROPERTY_GET_SIGNAL_STATS call updates  
	UINT QualityPercent;		// these three parameters & FrequencyMhz (see above)
	BOOL IsLocked;

} TUNER_COMMAND, *PTUNER_COMMAND;

//
// for bandstacked LNBs (aka DishPro LNBs),
// set SwitchFreqMhz = DishProLnb
// driver will select appropriate LOF based on the signal polarization
//
#define DishProLnb (20000)

#endif GNPBDA_H

