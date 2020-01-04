#ifndef QBOXBDA_H
#define QBOXBDA_H

DEFINE_GUIDSTRUCT( "C6EFE5EB-855A-4f1b-B7AA-87B5E1DC4113", KSPROPERTYSET_QBOXControl );
#define KSPROPERTYSET_QBOXControl DEFINE_GUIDNAMED( KSPROPERTYSET_QBOXControl )

typedef enum
{
    KSPROPERTY_CTRL_TUNER,
	KSPROPERTY_CTRL_IR,
	KSPROPERTY_CTRL_22K_TONE,
	KSPROPERTY_CTRL_MOTOR,
	KSPROPERTY_CTRL_LNBPW,
	KSPROPERTY_CTRL_LOCK_TUNER,
	KSPROPERTY_CTRL_MAC,
	KSPROPERTY_CTRL_DEVICEID,
	KSPROPERTY_CTRL_CI_ACCESS,
	KSPROPERTY_CTRL_BLIND_SCAN,  //added 2011 03 04 liuy
	KSPROPERTY_CTRL_I2C_ACCESS,   //added 2011 09 02 liuy
	KSPROPERTY_CTRL_GETSNRBER, //added 2011 09 30 liuy
	KSPROPERTY_CTRL_GETCONSTELATION, //added 2011 9 30 liuy
	KSPROPERTY_CTRL_CHANNEL_CHANGE, //added 2011 9 30 liuy Custom property for changing channel
	KSPROPERTY_CTRL_UNICABLE,//*****added 2011 10 19 interface for unicable****/
	KSPROPERTY_CTRL_GET_MEDIAINFO, //added 2011 10 13 5925
	KSPROPERTY_CTRL_INPUTMULTISTREAMID, //added 2011 10 13 5925
	KSPROPERTY_CTRL_UNKNOWN,
	KSPROPERTY_CTRL_TBSACCESS,
	KSPROPERTY_CTRL_PLPINFO,	//added 2012 11 09 liuy DVBT2 PLP interface
	KSPROPERTY_CTRL_PLS,		//added 2013 03 18 liuy Gold and Root Code interface
	KSPROPERTY_CTRL_MODCODES,
} KSPROPERTY_QBOXControl;

typedef struct {
	DWORD	ChannelFrequency;//
	DWORD	ulLNBLOFLowBand;
	DWORD	ulLNBLOFHighBand;
	DWORD	SymbolRate;
	BYTE	Polarity;
	BYTE	LNB_POWER;              // LNB_POWER_ON | LNB_POWER_OFF
    BYTE	HZ_22K;                 // HZ_22K_OFF | HZ_22K_ON
    BYTE	Tone_Data_Burst;        // Data_Burst_ON | Tone_Burst_ON |Tone_Data_Disable
    BYTE	DiSEqC_Port;            // DiSEqC_NULL | DiSEqC_A | DiSEqC_B | DiSEqC_C | DiSEqC_D

	BYTE motor[5];
	BYTE ir_code;
	BYTE lock;
	BYTE strength;
	BYTE quality;
	BYTE FecType;
	BYTE ModuType;
	BYTE reserved[254];
} QBOXDVBSCMD, *PQBOXDVBSCMD;

#define MAX_QBOX_I2C 128
//added 2011 09 02 liuy
typedef struct _i2caccess_cmd
{
	unsigned char chip_add;//chip address
	unsigned char data[MAX_QBOX_I2C];//input command
	unsigned char nbdata;//input command lenth
	unsigned char outbuffer[MAX_QBOX_I2C];//outbuffer
	unsigned char nlenth;  //   outbuffer lenth
} QBOX_I2CACCESS_CMD;

//added 2011 10 19 
typedef enum _TBSUnicableMode {
	TBSUNICABLE_LNBPOWER=0x00,      
	TBSUNICABLE_DISEQC=0x01,            
	TBSUNICABLE_LOCK=0x02                         
}TBSUnicableMode;

typedef struct _tbs_unicableobj_
{

	TBSUnicableMode    modetype;//hv;diseqc;lock
	unsigned long      frequency_khz;	/* slot frequency in kHz                */
	unsigned long      symbol_rate_ksps;/* symbol rate (kSps)                    */
	int                nLNBPower;//1H;2V
	UCHAR              uc_diseqc_send_message[10];
	UCHAR              uc_diseqc_send_message_length;

}TBS_UNICABLEOBJ;
//end 2011 10 19

typedef enum _tbsaccessmode{
	TBSACCESS_LNBPOWER=0x00,
	TBSACCESS_DISEQC  =0x01,
	TBSACCESS_22K     =0x02
}TBSAccessMode;

typedef enum _tbslnbpowermode{
	LNBPWR_OFF  =0x00,
	LNBPWR_ON   =0x03,// this sets automatically to last H/V state  and keeps changing after tune requests
	LNBPWR_13V  =0x02,
	LNBPWR_18V  =0x01,
}TBSLNBPowerMode;

typedef enum _tbs22kmode{
	TBSBURST_OFF         =0x00,
	TBSBURST_ON          =0x01, 
	TBSBURST_UNMODULATED =0x02, //tone burst unmodulated ,satellite A
	TBSBURST_MODULATED   =0x03 ////tone burst modulated, satellite B
}TBS22KMode;

typedef struct _tbs_access
{
	TBSAccessMode	   access_mode;
	TBS22KMode		   tbs22k_mode;
	DWORD			   uc_reservedtemp;
	TBSLNBPowerMode    LNBPower_mode;
	UCHAR			   uc_diseqc_send_message[128];
	DWORD			   uc_diseqc_send_message_length;
	UCHAR			   uc_diseqc_receive_message[128];
	DWORD			   uc_diseqc_receive_message_length;
	UCHAR			   uc_reserved[256];//reserved for future use 
} TBS_ACCESS_STRUCT;

typedef struct _MEDIAINFO
{

	int 	Frequency;				/* Transponder frequency (in KHz)				*/
	int 	SymbolRate; 			/* Transponder symbol rate	(in Mbds)			*/

	int		Standard;				/* Found Standard DVBS1,DVBS2 or DSS	*/
	int 	ModCode;				/* Found Modcode only for DVBS2 		*/	

	int		PunctureRate;			/* Found Puncture rate	For DVBS1		*/
	int 	Modulation; 			/* Found modulation type				*/


	int		StreamType; 		 /* Transport Stream Input or Generic Stream Input (packetized or continuous).*/
	int 	InputStream;		 /* Single Input Stream or Multiple Input Stream. */
	int		CodingModulation;	 /* Constant Coding and Modulation or Adaptive Coding and Modulation (VCM is signalled as ACM).*/
	int		InputStreamSynchro;  /* (Input Stream Synchronization Indicator): If ISSYI = 1 = active, the ISSY field is inserted after UPs */
	int		NullPacketdeletion;  /* Null-packet deletion active/not active. */ 
	int		RollOff;			 /* Transmission Roll-off factor */

	int		StreamIdentify;      /*Multiple stream identify*/

	//added 2010 12 17
	int		MATYPEOne;
	int		MATYPETwo;

	//added 2011 07 26
	int 	Spectrum;				/* IQ specrum swap setting	0 =IQ_NORMAL; 1=IQ_SWAPPED*/
	int 	Pilots; 				/* Pilots on,off only for DVB-S2	0= OFF; 1= ON			*/

	//added 2011 08 03 liuy
	int		FrameLength;
} MEDIAINFO;

typedef struct _PLS_INFO
{ 
	UINT PlsCode;
	UINT PlsMode;//0= ROOT Code; 1=GoldCode
	BYTE reserved[256];
} PLS_INFO;

#endif //QBOXBDA_H
