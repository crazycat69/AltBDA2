#ifndef TBSBDA_H
#define TBSBDA_H

// this is defined in bda tuner/demod driver source (splmedia.h)
const GUID KSPROPSETID_BdaTunerExtensionProperties =
{0xfaa8f3e5, 0x31d4, 0x4e41, {0x88, 0xef, 0xd9, 0xeb, 0x71, 0x6f, 0x6e, 0xc9}};

// this is defined in bda tuner/demod driver source (splmedia.h)
// this is defined in bda tuner/demod driver source (splmedia.h)
typedef enum {
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
                                        //corrections and adjustments
    KSPROPERTY_BDA_NBC_PARAMS,
	KSPROPERTY_BDA_BLIND_SCAN,
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
} KSPROPERTY_BDA_TUNER_EXTENSION;

const BYTE DISEQC_TX_BUFFER_SIZE = 150; // 3 bytes per message * 50 messages
const BYTE DISEQC_RX_BUFFER_SIZE = 8;   // reply fifo size, hardware limitation

/*******************************************************************************************************/
/* PHANTOM_LNB_BURST */
/*******************************************************************************************************/
typedef enum _PHANTOMLnbburst  {
    PHANTOM_LNB_BURST_UNDEF,                    /* undefined (results in an error) */
	PHANTOM_LNB_BURST_MODULATED,                /* Modulated: Tone B               */
    PHANTOM_LNB_BURST_UNMODULATED,              /* Not modulated: Tone A           */
}   PHANTOM_LNB_TONEBURST;

/*******************************************************************************************************/
/* PHANTOM_LNB_TONEBURST_EN_SET */
/*******************************************************************************************************/
typedef enum _PHANTOMLnbToneBurstEn  {
    PHANTOM_LNB_TONEBURST_ENABLE,	/* Enable tone burst */
    PHANTOM_LNB_DATABURST_ENABLE,	/* Enable tone burst */
    PHANTOM_LNB_BURST_DISABLE		/* Disable tone burst*/    
}   PHANTOM_LNB_BURST;

/*******************************************************************************************************/
/* PHANTOM_RXMODE */
/*******************************************************************************************************/
typedef enum _PHANTOMRxMode  {
    PHANTOM_RXMODE_INTERROGATION=0,              /* Demod expects multiple devices attached */
    PHANTOM_RXMODE_QUICKREPLY=1,                 /* demod expects 1 rx (rx is suspended after 1st rx received) */
    PHANTOM_RXMODE_NOREPLY=2                     /* demod expects to receive no Rx message(s) */
}   PHANTOM_RXMODE;

/////////////LIUZHENG ,ADDED ///////////
typedef enum _TBSDVBSExtensionPropertiesCMDMode {
    TBSDVBSCMD_LNBPOWER=0x00,      
    TBSDVBSCMD_MOTOR=0x01,            
    TBSDVBSCMD_22KTONEDATA=0x02,               
    TBSDVBSCMD_DISEQC=0x03              
}   TBSDVBSExtensionPropertiesCMDMode;

// DVB-S/S2 DiSEqC message parameters
typedef struct _DISEQC_MESSAGE_PARAMS
{
    UCHAR      uc_diseqc_send_message[DISEQC_TX_BUFFER_SIZE+1];
    UCHAR      uc_diseqc_send_message_length;

    UCHAR      uc_diseqc_receive_message[DISEQC_RX_BUFFER_SIZE+1];
    UCHAR      uc_diseqc_receive_message_length;    
    
    PHANTOM_LNB_TONEBURST burst_tone;	//Burst tone at last-message: (modulated = ToneB; Un-modulated = ToneA). 
    PHANTOM_RXMODE    receive_mode;		//Reply mode: interrogation/no reply/quick reply.
    TBSDVBSExtensionPropertiesCMDMode tbscmd_mode;

    UCHAR      HZ_22K;			// HZ_22K_OFF | HZ_22K_ON
    UCHAR      Tone_Data_Burst;		// Data_Burst_ON | Tone_Burst_ON |Tone_Data_Disable
    UCHAR      uc_parity_errors;	// Parity errors:  0 indicates no errors; binary 1 indicates an error.
    UCHAR      uc_reply_errors;		// 1 in bit i indicates error in byte i. 
    BOOL       b_last_message;		// Indicates if current message is the last message (TRUE means last message).
    BOOL       b_LNBPower;		// liuzheng added for lnb power static
    
} DISEQC_MESSAGE_PARAMS, *PDISEQC_MESSAGE_PARAMS;

//added 2011 01 27 liuy
typedef struct TBSDISEQC_MESSAGE_PARAMS
{
	UCHAR      uc_diseqc_send_message[10];
	UCHAR      uc_diseqc_send_message_length;

} TBSDISEQC_MESSAGE_PARAMS;
//end 2011 01 27

/*******************************************************************************************************/
/* PHANTOM_ROLLOFF */
/*******************************************************************************************************/
typedef enum _PHANTOMRollOff  {   /* matched filter roll-off factors */
    PHANTOM_ROLLOFF_020=0,             /*   roll-off factor is 0.2  */
    PHANTOM_ROLLOFF_025=1,            /*   roll-off factor is 0.25 */
    PHANTOM_ROLLOFF_035=2,            /*   roll-off factor is 0.35 */
    PHANTOM_ROLLOFF_UNDEF=0xFF        /*   roll-off factor is undefined */
}   PHANTOM_ROLLOFF;
/*******************************************************************************************************/
/* Pilot                                                                                               */
/*******************************************************************************************************/
typedef enum _PHANTOMPilot {
    PHANTOM_PILOT_OFF     = 0,
    PHANTOM_PILOT_ON      = 1,
    PHANTOM_PILOT_UNKNOWN = 2 /* not used */
}   PHANTOM_PILOT;

// DVBS2 required channel attributes not covered by BDA
typedef struct _BDA_NBC_PARAMS
{
    PHANTOM_ROLLOFF	rolloff;
    PHANTOM_PILOT	pilot;
	int				dvbtype;// 1 for dvbs 2 for dvbs2 0 for auto
    BinaryConvolutionCodeRate fecrate;
	ModulationType	modtype;
	
} BDA_NBC_PARAMS, *PBDA_NBC_PARAMS;

//------------------------------------------------------------
//
//
//  BDA Demodulator Extension Properties
//
// {B51C4994-0054-4749-8243-029A66863636}
const GUID KSPROPSETID_CustomIRCaptureProperties = 
{ 0xb51c4994, 0x54, 0x4749, { 0x82, 0x43, 0x2, 0x9a, 0x66, 0x86, 0x36, 0x36 }};

#define IRCAPTURE_COMMAND_START        1
#define IRCAPTURE_COMMAND_STOP         2
#define IRCAPTURE_COMMAND_FLUSH        3

typedef struct 
{   
    DWORD             dwAddress;
    DWORD             dwCommand;  

} KSPROPERTY_IRCAPTURE_KEYSTROKES_S, *PKSPROPERTY_IRCAPTURE_KEYSTROKES_S;

typedef struct 
{    
    CHAR             CommandCode;    

} KSPROPERTY_IRCAPTURE_COMMAND_S, *PKSPROPERTY_IRCAPTURE_COMMAND_S;

typedef enum
{
    KSPROPERTY_IRCAPTURE_KEYSTROKES         = 0,
    KSPROPERTY_IRCAPTURE_COMMAND            = 1

}KSPROPERTY_IRCAPTURE_PROPS;

DEFINE_GUIDSTRUCT( "CBBF16AE-2A99-477e-B0D7-9C2274EB209E", KSPROPSETID_CX_GOSHAWK2_DIAG_PROP);
#define KSPROPSETID_CX_GOSHAWK2_DIAG_PROP  DEFINE_GUIDNAMED( KSPROPSETID_CX_GOSHAWK2_DIAG_PROP )

typedef enum {
    CX_GOSHAWK2_DIAG_PROP_I2C_ACCESS             = 0,
    CX_GOSHAWK2_DIAG_PROP_I2C_WRITE              = 1,
    CX_GOSHAWK2_DIAG_PROP_I2C_READ               = 2,
    CX_GOSHAWK2_DIAG_PROP_I2C_WRITE_THEN_READ    = 3    
} CX_GOSHAWK2_DIAGNOSTIC_PROPERTIES;

#define MAX_SUBADDRESS_BYTES    8
#define MAX_I2C_BYTES           16
#define AUDIO_ADC_I2C_ADDRESS   0x34
#define EEPROM_I2C_ADDRESS		0xA0

typedef enum
{
    KSPROPERTY_DISABLE_USERMODE_I2C = 0,
    KSPROPERTY_ENABLE_USERMODE_I2C  = 1
}KSPROPERTY_I2C_STATES;

typedef struct _I2C_ACCESS
{
    KSPROPERTY_I2C_STATES state;    
}I2C_ACCESS, *PI2C_ACCESS;

typedef struct _I2C_STRUCT
{
    BYTE i2c_interface_select;
    BYTE chip_address;
    BYTE num_bytes;
    BYTE bytes[MAX_I2C_BYTES];
}I2C_STRUCT, *PI2C_STRUCT;

typedef struct _I2C_WRITE_THEN_READ_STRUCT
{
    BYTE i2c_interface_select;
    BYTE chip_address;
    BYTE num_write_bytes;
    BYTE write_bytes[MAX_SUBADDRESS_BYTES];
    BYTE num_read_bytes;
    BYTE read_bytes[MAX_I2C_BYTES];
}I2C_WRITE_THEN_READ_STRUCT, *PI2C_WRITE_THEN_READ_STRUCT;

// {8EE0EBA8-7DA6-4aa9-BD80-87E76B8ADBC8}
static const GUID SAA716X_REG_ACC_PROPERTY = 
{ 0x8ee0eba8, 0x7da6, 0x4aa9, { 0xbd, 0x80, 0x87, 0xe7, 0x6b, 0x8a, 0xdb, 0xc8 } };

// {DCDF1D99-AEAC-445c-9C1F-DEF8DF2D8C97}
static const GUID SAA716X_I2C_ACC_PROPERTY = 
{ 0xdcdf1d99, 0xaeac, 0x445c, { 0x9c, 0x1f, 0xde, 0xf8, 0xdf, 0x2d, 0x8c, 0x97 } };

// {FBB1C466-BE03-470f-9A8F-A2AA0FA2C0EE}
static const GUID VAMP_I2C_ACCESS_PROPERTY = 
{ 0xfbb1c466, 0xbe03, 0x470f, { 0x9a, 0x8f, 0xa2, 0xaa, 0xf, 0xa2, 0xc0, 0xee } };

// exactly these structures need to be defined to interact with the private 
// enumeration and reg access interface
//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the Reg access request structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    KSPROPERTY ksProperty;
    DWORD dwDeviceType;        //type enum for the device
    WORD wImplementationId;   //implementation id for the device
    BYTE  ucInstanceId;        //instance id of this device type
    BYTE* pucAddressOffset;    //pointer to register offset of the device   
    DWORD dwSizeAddressOffset; //size of the register offset of the device
    BYTE* pucDataToGet;        //pointer to DataToGet for 'Get' calls
    DWORD dwSizeDataToGet;     //size of the DataToGet in bytes for 'Get' calls
} tMainDevicePropRegAccessRequest, *ptMainDevicePropRegAccessRequest;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the Reg access data structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    BYTE* pucDataToSet;     //pointer to DataToSet for 'Set' calls
    DWORD dwSizeDataToSet;  //number of bytes in the DataToSet for 'Set' calls
} tMainDevicePropRegAccessData, *ptMainDevicePropRegAccessData;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the Reg access request structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    KSPROPERTY ksProperty;
    DWORD  dwDeviceType;         //type enum for the device
    WORD  wImplementationId;    //implementation id for the device
    BYTE*  pucNumberOfInstances; //pointer to number of instances of the device   
} tMainDevicePropEnumRequest, *ptMainDevicePropEnumRequest;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the enumeration access data structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    BYTE ucDummy; //reserved
} tMainDevicePropEnumData, *ptMainDevicePropEnumData;

#define MAIN_I2C_ENUM           0
#define MAIN_I2C_ACCESS         1

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the I2C enumeration request structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    KSPROPERTY        ksProperty;
    BYTE*  pucNoOfInstances;    //pointer to number of instances
} tMainDevicePropI2CEnumRequest, *ptMainDevicePropI2CEnumRequest;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the I2C enumeration data info structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    BYTE      ucDummy;            //  set/get value for the requested pin
} tMainDevicePropI2CEnumData, *ptMainDevicePropI2CEnumData;


//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the I2C access request structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    KSPROPERTY        ksProperty;
    BYTE             ucInstanceId;     //instance id of this device type
    BYTE             ucSlaveAddress;   //SlaveAddress of the I2C Device
    BYTE*  pucSubAddress;    //pointer to SubAddress of the I2C Device   
    DWORD            dwSizeSubAddress; //size of the SubAddress of the I2C Device
    BYTE*  pucDataToGet;     //pointer to DataToGet for 'Get' calls
    DWORD            dwSizeDataToGet;  //size of the DataToGet in bytes for 'Get' calls
} tMainDevicePropI2CAccessRequest, *ptMainDevicePropI2CAccessRequest;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the Reg access data structure.
//
//////////////////////////////////////////////////////////////////////////////
typedef struct
{
    BYTE*  pucDataToSet;     //pointer to DataToSet for 'Set' calls
    DWORD            dwSizeDataToSet;  //number of bytes in the DataToSet for 'Set' calls
} tMainDevicePropI2CAccessData, *ptMainDevicePropI2CAccessData;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the I2C access request structure.
//
//////////////////////////////////////////////////////////////////////////////

typedef struct
{
    KSPROPERTY ksProperty;
    BYTE  ucSlaveAddress;   //SlaveAddress of the I2C Device
    BYTE* pucSubAddress;    //pointer to SubAddress of the I2C Device   
    DWORD dwSizeSubAddress; //size of the SubAddress of the I2C Device
    BYTE* pucDataToGet;     //pointer to DataToGet for 'Get' calls
    DWORD dwSizeDataToGet;  //size of the DataToGet in bytes for 'Get' calls
} tI2CAccessRequest, *ptI2CAccessRequest;

//////////////////////////////////////////////////////////////////////////////
//
// Description:
//  Describes the Reg access data structure.
//
/////////////////////////////////////////////////////////////////////////////
typedef struct
{
    BYTE* pucDataToSet;     //pointer to DataToSet for 'Set' calls
    DWORD dwSizeDataToSet;  //number of bytes in the DataToSet for 'Set' calls
} tI2CAccessData, *ptI2CAccessData;

#endif //TBSBDA_H
