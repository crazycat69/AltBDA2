#ifndef CXTBDA_H
#define CXTBDA_H

const GUID KSPROPSETID_BdaTunerExtensionProperties =
{0xfaa8f3e5, 0x31d4, 0x4e41, {0x88, 0xef, 0xd9, 0xeb, 0x71, 0x6f, 0x6e, 0xc9}};

const GUID KSPROPSETID_BdaTunerExtensionPropertiesHaup  =
{0xfaa8f3e5, 0x31d4, 0x4e41, {0x88, 0xef, 0x00, 0xa0, 0xc9, 0xf2, 0x1f, 0xc7}};

DEFINE_GUIDSTRUCT("03CBDCB9-36DC-4072-AC42-2F94F4ECA05E", KSPROPSETID_BdaTunerExtensionPropertiesBst);
#define KSPROPSETID_BdaTunerExtensionPropertiesBst DEFINE_GUIDNAMED(KSPROPSETID_BdaTunerExtensionPropertiesBst)

typedef enum
{
    KSPROPERTY_BDA_DISEQC_MESSAGE = 0,  //Custom property for Diseqc messaging
	KSPROPERTY_BDA_DISEQC_INIT,         //Custom property for Intializing Diseqc.
	KSPROPERTY_BDA_SCAN_FREQ,           //Not supported 
	KSPROPERTY_BDA_CHANNEL_CHANGE,      //Custom property for changing channel
	KSPROPERTY_BDA_DEMOD_INFO,          //Custom property for returning demod FW state and version
	KSPROPERTY_BDA_EFFECTIVE_FREQ,      //Not supported 
	KSPROPERTY_BDA_SIGNAL_STATUS,       //Custom property for returning signal quality, strength, BER and other attributes
	KSPROPERTY_BDA_LOCK_STATUS,         //Custom property for returning demod lock indicators 
	KSPROPERTY_BDA_ERROR_CONTROL,       //Custom property for controlling error correction and BER window
	KSPROPERTY_BDA_CHANNEL_INFO,        //Custom property for exposing the locked values of frequency,symbol rate etc after
	// Geniatech
	KSPROPERTY_BDA_NBC_PARAMS,
	KSPROPERTY_BDA_LNB_POWER,
	// TBS
    KSPROPERTY_BDA_GET_MEDIAINFO,
    KSPROPERTY_BDA_STREAMTYPE_PARAMS,
    KSPROPERTY_BDA_INPUTMULTISTREAMID,	//added 2011 01 27
    KSPROPERTY_BDA_MACADDR,				//added 2010 12 07
    KSPROPERTY_BDA_SETHV,				//added 2011 05 21 control HV , 22k , h=1;v=0; 
    KSPROPERTY_BDA_SET22K,				//22k 0 off; 1 on
	KSPROPERTY_BDA_CI_ACCESS,			//BOB ci /6992,6618,6928
	KSPROPERTY_BDA_UNICABLE,			//******added 2011 10 19 interface for all card*****/
	KSPROPERTY_BDA_PMT_ACCESS,			// ci mce  /6992,6618,6928
	KSPROPERTY_BDA_TBSACCESS,
	KSPROPERTY_BDA_PLPINFO,				//added 2012 11 09 liuy DVBT2 PLP interface
	KSPROPERTY_BDA_PLS,					//added 2013 02 25 liuy
	KSPROPERTY_BDA_MODCODES,
	KSPROPERTY_BDA_TBSI2CACCESS,		//added 20140617 lucy For TBS PCI-E bridge I2C interface
	// Hauppage
	KSPROPERTY_BDA_PILOT_HAUP = 0x20,
	KSPROPERTY_BDA_ROLLOFF_HAUP = 0x21
} KSPROPERTY_BDA_TUNER_EXTENSION;

// LNB tone burst modulated enums
typedef enum
{
    TONE_BURST_UNMODULATED = 0,
    TONE_BURST_MODULATED
} TONE_BURST_MODULATION_TYPE;

typedef enum RxMode{
	RXMODE_DEFAULT=0,		/* use current register setting  */
	RXMODE_INTERROGATION=1,	/* Demod expects multiple devices attached */
	RXMODE_QUICKREPLY,		/* demod expects 1 rx (rx is suspended after 1st rx received) */
	RXMODE_NOREPLY,			/* demod expects to receive no Rx message(s) */
} RXMODE;

typedef enum DiseqcVer{
	DISEQC_VER_UNDEF=0,		/* undefined (results in an error) */
	DISEQC_VER_1X	=1,		/* Employs DiseqC version 1.x */
	DISEQC_VER_2X	=2,		/* Employs DiseqC version 2.x */
	ECHOSTAR_LEGACY	=3		/* Employs Echostar Legacy LNB messaging. */
} DISEQC_VER;

const BYTE DISEQC_TX_BUFFER_SIZE = 150; // 3 bytes per message * 50 messages
const BYTE DISEQC_RX_BUFFER_SIZE = 8;  // reply fifo size, hardware limitation

typedef struct _DISEQC_MESSAGE_PARAMS
{
    UCHAR      uc_diseqc_send_message[DISEQC_TX_BUFFER_SIZE+1];
    UCHAR      uc_diseqc_receive_message[DISEQC_RX_BUFFER_SIZE+1];
    ULONG      ul_diseqc_send_message_length;
    ULONG      ul_diseqc_receive_message_length;
    ULONG      ul_amplitude_attenuation;
    BOOL       b_tone_burst_modulated;
    DISEQC_VER diseqc_version;
    RXMODE     receive_mode;
    BOOL       b_last_message;
	
} DISEQC_MESSAGE_PARAMS, *PDISEQC_MESSAGE_PARAMS;

/*******************************************************************************************************/
/* PHANTOM_LNB_BURST */
/*******************************************************************************************************/
typedef enum _PHANTOMLnbburst  {
    PHANTOM_LNB_BURST_MODULATED=1,                /* Modulated: Tone B               */
    PHANTOM_LNB_BURST_UNMODULATED,                /* Not modulated: Tone A           */
    PHANTOM_LNB_BURST_UNDEF=0                     /* undefined (results in an error) */
}   PHANTOM_LNB_BURST;
/*******************************************************************************************************/
/* PHANTOM_RXMODE */
/*******************************************************************************************************/
typedef enum _PHANTOMRxMode  {
    PHANTOM_RXMODE_INTERROGATION=0,              /* Demod expects multiple devices attached */
    PHANTOM_RXMODE_QUICKREPLY=1,                 /* demod expects 1 rx (rx is suspended after 1st rx received) */
    PHANTOM_RXMODE_NOREPLY=2                     /* demod expects to receive no Rx message(s) */
}   PHANTOM_RXMODE;
/*******************************************************************************************************/
/* Pilot                                                                                               */
/*******************************************************************************************************/
typedef enum _PHANTOMPilot {
    PHANTOM_PILOT_OFF     = 0,
    PHANTOM_PILOT_ON      = 1,
    PHANTOM_PILOT_UNKNOWN = 2 /* not used */
}   PHANTOM_PILOT;
/*******************************************************************************************************/
/* PHANTOM_ROLLOFF */
/*******************************************************************************************************/
typedef enum _PHANTOMRollOff  {	/* matched filter roll-off factors */
    PHANTOM_ROLLOFF_020=0,		/*   roll-off factor is 0.20  */
    PHANTOM_ROLLOFF_025=1,		/*   roll-off factor is 0.25 */
    PHANTOM_ROLLOFF_035=2,		/*   roll-off factor is 0.35 */
    PHANTOM_ROLLOFF_UNDEF=0xFF	/*   roll-off factor is undefined */
}   PHANTOM_ROLLOFF;

// DVBS2 required channel attributes not covered by BDA
typedef struct _BDA_NBC_PARAMS
{
    PHANTOM_ROLLOFF      rolloff;
    PHANTOM_PILOT        pilot;
} BDA_NBC_PARAMS, *PBDA_NBC_PARAMS;//

#endif //CXTBDA_H
