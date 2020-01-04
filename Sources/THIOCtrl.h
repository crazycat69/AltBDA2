#ifndef THBDA_IOCTL_H
#define THBDA_IOCTL_H


#define FILE_ANY_ACCESS     0
#define METHOD_BUFFERED     0

// {255E0082-2017-4b03-90F8-856A62CB3D67}
static const GUID GUID_THBDA_CMD = 
{ 0x255e0082, 0x2017, 0x4b03, { 0x90, 0xf8, 0x85, 0x6a, 0x62, 0xcb, 0x3d, 0x67 } };


//{E5644CC4-17A1-4eed-BD90-74FDA1D65423}
static GUID GUID_THBDA_TUNER = 
{ 0xE5644CC4, 0x17A1, 0x4eed, { 0xBD, 0x90, 0x74, 0xFD, 0xA1, 0xD6, 0x54, 0x23 } };

#define CTL_CODE( DeviceType, Function, Method, Access ) (                 \
    ((DeviceType) << 16) | ((Access) << 14) | ((Function) << 2) | (Method) \
)

#define THBDA_IO_INDEX    0xAA00


//*******************************************************************************************************
//Functionality : Set turner power
//InBuffer      : Tuner_Power_ON | Tuner_Power_OFF
//InBufferSize  : 1 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_SET_TUNER_POWER             CTL_CODE(THBDA_IO_INDEX, 100, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get turner power status
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : Tuner_Power_ON | Tuner_Power_OFF
//OutBufferSize : 1 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_TUNER_POWER             CTL_CODE(THBDA_IO_INDEX, 101, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//Obsolete now
//*******************************************************************************************************
//Functionality : Set LNB configuration
//InBuffer      : struct LNB_DATA
//InBufferSize  : sizeof(LNB_DATA) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
//#define THBDA_IOCTL_SET_LNB                     CTL_CODE(THBDA_IO_INDEX, 102, METHOD_BUFFERED, FILE_ANY_ACCESS) 
//
//Obsolete now
//*******************************************************************************************************
//Functionality : Get LNB configuration
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct LNB_DATA
//OutBufferSize : sizeof(LNB_DATA) bytes
//*******************************************************************************************************
//#define THBDA_IOCTL_GET_LNB                     CTL_CODE(THBDA_IO_INDEX, 103, METHOD_BUFFERED, FILE_ANY_ACCESS) 



//*******************************************************************************************************
//Functionality : Set LNB parameters
//InBuffer      : struct LNB_DATA
//InBufferSize  : sizeof(LNB_DATA) bytes
//OutBuffer     : 0
//OutBufferSize : 0
//*******************************************************************************************************
#define THBDA_IOCTL_SET_LNB_DATA            CTL_CODE(THBDA_IO_INDEX, 128, METHOD_BUFFERED, FILE_ANY_ACCESS) 

//*******************************************************************************************************
//Functionality : GET LNB parameters
//InBuffer      : NULL
//InBufferSize  : 0
//OutBuffer     : struct LNB_DATA
//OutBufferSize : sizeof(LNB_DATA) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_LNB_DATA            CTL_CODE(THBDA_IO_INDEX, 129, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Send DiSEqC command
//InBuffer      : struct DiSEqC_DATA
//InBufferSize  : sizeof(DiSEqC_DATA) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_SET_DiSEqC                  CTL_CODE(THBDA_IO_INDEX, 104, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get DiSEqC command
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct DiSEqC_DATA
//OutBufferSize : sizeof(DiSEqC_DATA) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_DiSEqC                  CTL_CODE(THBDA_IO_INDEX, 105, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Set turner frequency and symbol rate
//InBuffer      : struct TURNER_VALUE
//InBufferSize  : sizeof(TURNER_VALUE) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_LOCK_TUNER                  CTL_CODE(THBDA_IO_INDEX, 106, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get turner frequency and symbol rate
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct TURNER_VALUE
//OutBufferSize : sizeof(TURNER_VALUE) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_TUNER_VALUE            CTL_CODE(THBDA_IO_INDEX, 107, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get signal quality & strength
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct SIGNAL_DATA
//OutBufferSize : sizeof(SIGNAL_DATA) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_SIGNAL_Q_S              CTL_CODE(THBDA_IO_INDEX, 108, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : START TS capture (from Tuner to driver Ring buffer)
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_START_CAPTURE               CTL_CODE(THBDA_IO_INDEX, 109, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Stop TS capture
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_STOP_CAPTURE                CTL_CODE(THBDA_IO_INDEX, 110, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get Driver ring buffer status
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct RING_BUF_STATUS 
//OutBufferSize : sizeof(RING_BUF_STATUS) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_RINGBUFFER_STATUS      CTL_CODE(THBDA_IO_INDEX, 111, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get TS from driver's ring buffer to local  buffer 
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct CAPTURE_DATA
//OutBufferSize : sizeof(CAPTURE_DATA) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_CAPTURE_DATA            CTL_CODE(THBDA_IO_INDEX, 112, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Set PID filter mode and Pids to PID filter
//InBuffer      : struct PID_FILTER_INFO
//InBufferSize  : sizeof(PID_FILTER_INFO) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_SET_PID_FILTER_INFO         CTL_CODE(THBDA_IO_INDEX, 113, METHOD_BUFFERED, FILE_ANY_ACCESS)  


//*******************************************************************************************************
//Functionality : Get Pids, PLD mode and available max number Pids
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct PID_FILTER_INFO
//OutBufferSize : sizeof(PID_FILTER_INFO) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_PID_FILTER_INFO         CTL_CODE(THBDA_IO_INDEX, 114, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Start RC(Remote Controller receiving) thread
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_START_REMOTE_CONTROL        CTL_CODE(THBDA_IO_INDEX, 115, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Stop RC thread, and remove all RC event
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_STOP_REMOTE_CONTROL         CTL_CODE(THBDA_IO_INDEX, 116, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Add RC_Event to driver
//InBuffer      : REMOTE_EVENT
//InBufferSize  : sizeof(REMOTE_EVENT) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_ADD_RC_EVENT                CTL_CODE(THBDA_IO_INDEX, 117, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Remove RC_Event 
//InBuffer      : REMOTE_EVENT
//InBufferSize  : sizeof(REMOTE_EVENT) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_REMOVE_RC_EVENT             CTL_CODE(THBDA_IO_INDEX, 118, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get Remote Controller key
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : BYTE
//OutBufferSize : 1 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_REMOTE_CONTROL_VALUE    CTL_CODE(THBDA_IO_INDEX, 119, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Set Remote control,HID function enable or disable
//InBuffer      : 1 0 for OFF,others for ON.
//InBufferSize  : 1 bytes
//OutBuffer     : 0 registers value
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_HID_RC_ENABLE             CTL_CODE(THBDA_IO_INDEX, 152, METHOD_BUFFERED, FILE_ANY_ACCESS)



//*******************************************************************************************************
//Functionality : Reset USB or PCI controller
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_RESET_DEVICE                CTL_CODE(THBDA_IO_INDEX, 120, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Check BDA driver if support IOCTL interface
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CHECK_INTERFACE             CTL_CODE(THBDA_IO_INDEX, 121, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Set Twinhan BDA driver configuration
//InBuffer      : struct THBDAREGPARAMS
//InBufferSize  : sizeof(THBDAREGPARAMS) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_SET_REG_PARAMS              CTL_CODE(THBDA_IO_INDEX, 122, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Get Twinhan BDA driver configuration
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct THBDAREGPARAMS
//OutBufferSize : struct THBDAREGPARAMS
//*******************************************************************************************************
#define THBDA_IOCTL_GET_REG_PARAMS              CTL_CODE(THBDA_IO_INDEX, 123, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Get device info
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct DEVICE_INFO
//OutBufferSize : sizeof(DEVICE_INFO) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_DEVICE_INFO             CTL_CODE(THBDA_IO_INDEX, 124, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Get driver info
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct DriverInfo
//OutBufferSize : sizeof(DriverInfo) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_DRIVER_INFO             CTL_CODE(THBDA_IO_INDEX, 125, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Write EEPROM value
//InBuffer      : struct EE_IO_DATA
//InBufferSize  : sizeof(EE_IO_DATA) bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_SET_EE_VAL                  CTL_CODE(THBDA_IO_INDEX, 126, METHOD_BUFFERED, FILE_ANY_ACCESS) 
                  

//*******************************************************************************************************
//Functionality : Read EEPROM value      
//InBuffer      : struct EE_IO_DATA
//InBufferSize  : sizeof(EE_IO_DATA) bytes
//OutBuffer     : struct EE_IO_DATA
//OutBufferSize : sizeof(EE_IO_DATA) bytes
//*******************************************************************************************************                          
#define THBDA_IOCTL_GET_EE_VAL                  CTL_CODE(THBDA_IO_INDEX, 127, METHOD_BUFFERED, FILE_ANY_ACCESS) 

//*******************************************************************************************************
//Functionality : Enable virtual DVBT interface for DVB-S card
//InBuffer      : 1 0 for OFF,others for ON.
//InBufferSize  : 1 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_ENABLE_VIRTUAL_DVBT         CTL_CODE(THBDA_IO_INDEX, 300, METHOD_BUFFERED, FILE_ANY_ACCESS)

//*******************************************************************************************************
//Functionality : Reset (Clear) DVB-S Transponder mapping table entry for virtual DVB-T interface
//InBuffer      : NULL
//InBufferSize  : 0
//OutBuffer     : NULL
//OutBufferSize : 0
//*******************************************************************************************************
#define THBDA_IOCTL_RESET_T2S_MAPPING            CTL_CODE(THBDA_IO_INDEX, 301, METHOD_BUFFERED, FILE_ANY_ACCESS) 

//*******************************************************************************************************
//Functionality : Set DVB-S Transponder mapping table entry for virtual DVB-T interface
//InBuffer      : struct DVB-T2S_MAPPING_ENTRY
//InBufferSize  : sizeof(struct DVB-T2S_MAPPING_ENTRY) bytes
//OutBuffer     : NULL
//OutBufferSize : 0
//*******************************************************************************************************
#define THBDA_IOCTL_SET_T2S_MAPPING            CTL_CODE(THBDA_IO_INDEX, 302, METHOD_BUFFERED, FILE_ANY_ACCESS) 

//*******************************************************************************************************
//Functionality : GET DVB-S Transponder mapping table entry
//InBuffer      : &(Table_Index)
//InBufferSize  : sizeof(ULONG)
//OutBuffer     : struct DVB-T2S_MAPPING_ENTRY
//OutBufferSize : sizeof(struct DVB-T2S_MAPPING_ENTRY) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_GET_T2S_MAPPING            CTL_CODE(THBDA_IO_INDEX, 303, METHOD_BUFFERED, FILE_ANY_ACCESS) 


//*******************************************************************************************************
//Functionality : Download tuner firmware, 704C
//InBuffer      : 1 byte buffer,  0:Downlaod analog TV firmware, 1:download DVB-T firmware
//InBufferSize  : 1:byte
//OutBuffer     :1 byte buffer,  0-99: download percentage, 100:download complete, 255:Fail 
//OutBufferSize : 1 bytes
//*******************************************************************************************************

#define THBDA_IOCTL_DOWNLOAD_TUNER_FIRMWARE    CTL_CODE(THBDA_IO_INDEX, 400, METHOD_BUFFERED, FILE_ANY_ACCESS) 
//*******************************************************************************************************
//Functionality : Get tuner firmware download progress
//InBuffer      : NULL
//InBufferSize  : 0:byte
//OutBuffer     :1 byte buffer,  0-99: download percentage, 100:download complete, 255:Fail 
//OutBufferSize : 1 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_DOWNLOAD_TUNER_FIRMWARE_STAUS CTL_CODE(THBDA_IO_INDEX, 401, METHOD_BUFFERED, FILE_ANY_ACCESS)



//*******************************************************************************************************
//Functionality : Get CI state
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct THCIState
//OutBufferSize : sizeof(THCIState) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_GET_STATE                CTL_CODE(THBDA_IO_INDEX, 200, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Get APP info.
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct THAppInfo
//OutBufferSize : sizeof(THAppInfo) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_GET_APP_INFO                CTL_CODE(THBDA_IO_INDEX, 201, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Init MMI
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_INIT_MMI                    CTL_CODE(THBDA_IO_INDEX, 202, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Get MMI
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct THMMIInfo
//OutBufferSize : sizeof(THMMIInfo) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_GET_MMI                     CTL_CODE(THBDA_IO_INDEX, 203, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Answer
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : struct THMMIInfo
//OutBufferSize : sizeof(THMMIInfo) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_ANSWER                      CTL_CODE(THBDA_IO_INDEX, 204, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Close MMI
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_CLOSE_MMI                   CTL_CODE(THBDA_IO_INDEX, 205, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Send PMT
//InBuffer      : PMT data buffer
//InBufferSize  : PMT data buffer size bytes
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//Comment       : CA_PMT data format
                    //1: ca pmt list management;(8 bit);
                    //2: program number (16 bit);
                    //3: reserved (2 bit);
                    //4: version number (5 bit);
                    //5: current next indicator (I bit);
                    //6: reserved (4 bit);
                    //7: program information length (12 bit);
                    //8: if (7!=0)
                    //	    ca pmt command id (program level); (8 bit);
                    //	    ca descriptor at program level; (n * 8bit);
                    //9:  stream type (8 bit);
                    //10: reserved (3 bit);
                    //11: elementary stream PID (bit 13);
                    //12: reserved (4 bit);
                    //13: ES information length (12 bit);
                    //14: if (ES information length ! =0)
                    //       ca pmt command id ( elementary stream level) (8 bit);
                    //	     ca descriptor at elementary stream level; ( n * 8bit)
                    //* more detail, please refer to EN 50221 (8,4,3,4 CA_PMT); 
//*******************************************************************************************************
#define THBDA_IOCTL_CI_SEND_PMT                    CTL_CODE(THBDA_IO_INDEX, 206, METHOD_BUFFERED, FILE_ANY_ACCESS)


//*******************************************************************************************************
//Functionality : Create CI event
//InBuffer      : hEventHandle (The event handle that is created by AP)
//InBufferSize  : sizeof(HANDLE)
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_EVENT_CREATE             CTL_CODE(THBDA_IO_INDEX, 208, METHOD_BUFFERED, FILE_ANY_ACCESS)

//*******************************************************************************************************
//Functionality : Close CI event
//InBuffer      : hEventHandle (The event handle that is sended by create CI event)
//InBufferSize  : sizeof(HANDLE)
//OutBuffer     : NULL
//OutBufferSize : 0 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_EVENT_CLOSE              CTL_CODE(THBDA_IO_INDEX, 209, METHOD_BUFFERED, FILE_ANY_ACCESS)

//*******************************************************************************************************
//Functionality : Get PMT Reply
//InBuffer      : NULL
//InBufferSize  : 0 bytes
//OutBuffer     : PMT Reply Buffer
//OutBufferSize : sizeof(PMT Reply Buffer) bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_GET_PMT_REPLY            CTL_CODE(THBDA_IO_INDEX, 210, METHOD_BUFFERED, FILE_ANY_ACCESS)

//*******************************************************************************************************
//Functionality : Send CI raw command
//InBuffer      : RAW_CMD_INFO
//InBufferSize  : sizeof(RAW_CMD_INFO) bytes
//OutBuffer     : NULL
//OutBufferSize : 0
//*******************************************************************************************************
#define THBDA_IOCTL_CI_SEND_RAW_CMD            CTL_CODE(THBDA_IO_INDEX, 211, METHOD_BUFFERED, FILE_ANY_ACCESS)

//*******************************************************************************************************
//Functionality : Get CI raw command data
//InBuffer      : NULL
//InBufferSize  : 0
//OutBuffer     : Raw command data buffer
//OutBufferSize : Max 1024 bytes
//*******************************************************************************************************
#define THBDA_IOCTL_CI_GET_RAW_CMD_DATA        CTL_CODE(THBDA_IO_INDEX, 212, METHOD_BUFFERED, FILE_ANY_ACCESS)


#define MAX_PMT_REPLY_SIZE	1024

#define CA_PMT_CMDID_Decrambleing 1
#define CA_PMT_CMDID_MMI          2
#define CA_PMT_CMDID_Query        3 
#define CA_PMT_CMDID_NSelected    4

#define CA_PMT_LIST_MGT_MORE   0
#define CA_PMT_LIST_MGT_FIRST  1
#define CA_PMT_LIST_MGT_LAST   2
#define CA_PMT_LIST_MGT_ONLY   3
#define CA_PMT_LIST_MGT_ADD    4
#define CA_PMT_LIST_MGT_UPDATE 5

typedef enum _CAM_TYPE_ENUM
{
    CAM_DEFAULT = 1, //Viaccess
    TH_CAM_ASTON = 2,
    TH_CAM_CONAX = 3,
    TH_CAM_CRYPTOWORKS = 4    
}  CAM_TYPE_ENUM;




#define RC_NO_DATA                          0x44

#define Tuner_Power_ON                      1
#define Tuner_Power_OFF                     0


#define LNB_POWER_OFF                       0
#define LNB_POWER_ON                        1

#define LNB_TYPE_NORMAL                     0
#define LNB_TYPE_UNIVERSAL                  1
#define LNB_TYPE_CUSTOM                     2

#define Data_Burst_OFF                      0
#define Data_Burst_ON                       1

#define Tone_Burst_OFF                      0
#define Tone_Burst_ON                       1

#define HZ_22K_OFF                          0
#define HZ_22K_ON                           1

#define POLARITY_H                          0
#define POLARITY_V                          1

#define DiSEqC_NULL                         0
#define DiSEqC_A                            1
#define DiSEqC_B                            2
#define DiSEqC_C                            3
#define DiSEqC_D                            4

#define HighBand_22K_Output_Enable          1
#define HighBand_22K_Output_Disable         0

typedef struct {
    GUID    CmdGUID;            // Private Command GUID
    DWORD   dwIoControlCode;    // operation
    LPVOID  lpInBuffer;         // input data buffer
    DWORD   nInBufferSize;      // size of input data buffer
    LPVOID  lpOutBuffer;        // output data buffer
    DWORD   nOutBufferSize;     // size of output data buffer
    LPDWORD lpBytesReturned;    // byte count
} THBDACMD, *PTHBDACMD;

//Obsolete now
//typedef struct _LNB_DATA
//{
//    UCHAR           LNB_POWER;              // LNB_POWER_ON | LNB_POWER_OFF
//    UCHAR           POLARITY;               // POLARITY_H | POLARITY_V
//    UCHAR           HZ_22K;                 // HZ_22K_OFF | HZ_22K_ON
//    UCHAR           Tone_Data_Burst;        // Data_Burst_ON | Tone_Burst_ON |Tone_Data_Disable
//
//    ULONG           ulLNBLOFLowBand;        // LOF Low Band, in KHz
//    ULONG           ulLNBLOFHighBand;       // LOF High Band, in KHz
//    ULONG           ulLNBLOFHiLoSW;         // LOF High/Low Band Switch Freq, in KHz
//
//    UCHAR           HighBand_22K_Output;    // HighBand_22K_Output_Enable: output 22K if HighBand Freq, no output 22K if LowBand Freq
//                                            // HighBand_22K_Output_Disable: output 22K according to "HZ_22K" setting
//
//    UCHAR           DiSEqC_Port;            // DiSEqC_NULL | DiSEqC_A | DiSEqC_B | DiSEqC_C | DiSEqC_D
//} LNB_DATA, *P_LNB_DATA;


#define MAX_T2S_TABLE_SIZE 180
typedef struct _DVB_T2S_MAPPING_ENTRY
{
    ULONG       ulIndex;                // Mapping Table index from 0-MAX_T2S_TABLE_SIZE
    ULONG       Frequency;              // DVB-S in Khz, Original Frequency, (not offseted by LOF)
    ULONG       SymbolRate;		// in Ksps  
    ULONG       Polarity;		// POLARITY_H|POLARITY_V
} DVB_T2S_MAPPING_ENTRY, *P_DVB_T2S_MAPPING_ENTRY;


typedef struct _LNB_DATA
{
    UCHAR           LNB_POWER;              // LNB_POWER_ON | LNB_POWER_OFF

    UCHAR           Tone_Data_Burst;        // Tone_Data_OFF | Tone_Burst_ON | Data_Burst_ON 

    ULONG           ulLNBLOFLowBand;        // LOF Low Band, in KHz
    ULONG           ulLNBLOFHighBand;       // LOF High Band, in KHz
    ULONG           ulLNBLOFHiLoSW;         // LOF High/Low Band Switch Freq, in KHz

    UCHAR           f22K_Output;		    // 22KHz tone Control: F22K_Output_HiLo, F22K_Output_Off, F22K_Output_On

    UCHAR           DiSEqC_Port;            // DiSEqC_NULL | DiSEqC_A | DiSEqC_B | DiSEqC_C | DiSEqC_D
} LNB_DATA, *P_LNB_DATA;



typedef struct _DiSEqC_DATA
{
    INT             command_len;           // 3, 4, 5
    UCHAR           command[12];           // DiSEqC command 3, 4, 5
} DiSEqC_DATA, *P_DiSEqC_DATA;
    

#define LOCK_NOT_WAIT_RESULT                0
#define LOCK_WAIT_RESULT                    1

typedef struct _TURNER_VALUE
{
	ULONG           Frequency;              // DVB-S in Khz, Original Frequency, (not offseted by LOF)
	                                        // DVB-T, DVB-C 50000-860000Khz
	union {
        ULONG       SymbolRate;		        // in Ksps  
        ULONG       Bandwidth;			// 6000/7000/8000 in KHz  
    };
    ULONG           QAMSize;                // 0/8/16/32/64/128/256
	UCHAR           WaitStatus;             // LOCK_NOT_WAIT_RESULT | LOCK_WAIT_RESULT
} TURNER_VALUE, *P_TURNER_VALUE;


#define Tuner_Lock                          1
#define Tuner_UnLock                        0

typedef struct _SIGNAL_DATA
{
    UCHAR           Quality;                // 0-99
    UCHAR           Strength;               // 0-99
    UCHAR           Lock_Status;            // Tuner_Lock | Tuner_UnLock
    UCHAR           Lock_Status_2;          // reserved, by products
}   SIGNAL_DATA, *P_SIGNAL_DATA;

typedef struct _RING_BUF_STATUS
{
    ULONG           Ring_Buf_Size;          // The TS Ring Buffer size in driver 
    ULONG           Ring_Buf_Head;          // The TS Ring Buffer Head, 0-(Ring_Buf_Size-1)
    ULONG           Ring_Buf_Tail;          // The TS Ring Buffer End,  0-(Ring_Buf_Size-1)
    ULONG           OverFlowCnt;            // # of times for buffer overflow 
    ULONG           OverFlowSize;           // TS data discarded
    UCHAR           Reset_Flag;             // 1:Reset overflowcnt/size after call, 0:no reset
} RING_BUF_STATUS, *P_RING_BUF_STATUS;

typedef struct _CAPTURE_DATA
{
    PUCHAR          TS_Buf;                 // in User space
    ULONG           TS_Buf_Size;            // 
    ULONG           Return_TS_Size;         // Actually TS data returned from driver to user 
} CAPTURE_DATA, *P_CAPTURE_DATA;



typedef struct _REMOTE_EVENT
{
    HANDLE          hEvent;
} REMOTE_EVENT, *PREMOTE_EVENT;


typedef struct _THBDAREGPARAMS
{
    ULONG           ulreserved1;
    ULONG           ulreserved2; 
    ULONG           ulDisableOffFreqScan;   // 0:Normal, 
                                            // 1:Disable off center-frequncy scan (+-167KHz/+-125KHz)
    ULONG           ulRelockMonitor;        // Relock Monitor enable flag
    ULONG           ulreserved3; 
    ULONG           ulreserved4;
    ULONG           ulreserved5; 
    ULONG           ulreserved6; 
    ULONG           ulreserved7;
    ULONG           ulreserved8;          
    ULONG           ulATSCFreqShift;        // ATSC frequency shift
} THBDAREGPARAMS, *PTHBDAREGPARAMS;


typedef struct _EE_IO_DATA
{
    DWORD           Address;
    DWORD           Value;
} EE_IO_DATA, *P_EE_IO_DATA;


typedef struct _DriverInfo 
{
    UCHAR           Version_Major;           // in BCD Ex., 3.2    =====> 0x32
    UCHAR           Version_Minor;           // 2.1    =====> 0x21
    UCHAR           FW_Version_Major;        // in BCD Ex., 10
    UCHAR           FW_Version_Minor;        //             05   =====> 1.0b05
    CHAR            Date_Time[22];           // Ex.,"2004-12-20 18:30:00" or  "DEC 20 2004 10:22:10"  with compiler __DATE__ and __TIME__  definition s
    CHAR            Company[8];              // Ex.,"TWINHAN" 
    CHAR            SupportHWInfo[32];       // Ex.,"PCI DVB CX-878 with MCU series", "PCI ATSC CX-878 with MCU series", "7020/7021 USB-Sat", , "7045/7046 USB-Ter",.....................
    CHAR            CI_MMI_Flag;             //Bit 0:  0:No event mode support 1:Event mode supported
    CHAR            Reserved[189];
} DriverInfo, *P_DriverInfo;


#define DEVICE_TYPE_DVBS                    0x00000001
#define DEVICE_TYPE_DVBT                    0x00000002
#define DEVICE_TYPE_DVBC                    0x00000004
#define DEVICE_TYPE_ATSC                    0x00000008
#define DEVICE_TYPE_ANNEX_C                 0x00000010       //US OpenCable
#define DEVICE_TYPE_ISDB_T                  0x00000020
#define DEVICE_TYPE_ISDB_S                  0x00000040

#define DEVICE_TYPE_PAL                     0x00000100
#define DEVICE_TYPE_NTSC                    0x00000200
#define DEVICE_TYPE_SECAM                   0x00000400
#define DEVICE_TYPE_SVIDEO                  0x00000800
#define DEVICE_TYPE_COMPOSITE               0x00001000
#define DEVICE_TYPE_FM                      0x00002000


#define Device_Speed_PCI                    0xff
#define Device_Speed_LOW                    0   //USB 1.1 low
#define Device_Speed_FULL                   1   //USB 1.1 full
#define Device_Speed_HIGH                   2   //USB 2.0 high

#define UNSUPPORT_CI                        0
#define SUPPORT_CI_V1                       1
#define SUPPORT_CI_V2                       2

typedef struct _DEVICE_INFO
{
    CHAR            Device_Name[32];        // Ex., VP1020, VP3020C, VP7045...
    ULONG           Device_TYPE;            // DEVICE_TYPE_DVBS, DEVICE_TYPE_DVBT, DEVICE_TYPE_DVBC...
    UCHAR           Device_Speed;           // Device_Speed_PCI, Device_Speed_FULL, Device_Speed_HIGH
    UCHAR           MAC_ADDRESS[6];
    UCHAR           CI_Support;             // 0:UNSUPPORT_CI, 1:SUPPORT_CI_V1, 2:SUPPORT_CI_V2
    INT             TS_Packet_Len;          // 188 | 204    
    CHAR            PID_Filter;             //0:No pid filter   1:Pidfilter
    CHAR            PID_Filter_Bypass;      //0:No Bypass mode  1:Bypass mode supported
    CHAR            Reserved[188];
} DEVICE_INFO, *P_DEVICE_INFO;

#define MAX_PIDS_TABLE_NUM 32
enum {
    PID_FILTER_MODE_PASS=0,                 // only set PIDs pass through
    PID_FILTER_MODE_DISABLE,                // Disable PLD, let all TS pass through
    PID_FILTER_MODE_FILTER                  // only set PIDs can't pass through
};

typedef struct _PID_FILTER_INFO 
{
    BYTE            PIDFilterMode;          // PID_FILTER_MODE_PASS | PID_FILTER_MODE_DISABLE | PID_FILTER_MODE_FILTER
    BYTE            MaxPidsNum;             // Pid max number that HW & Fw afford!
    ULONG           CurPidValidMap;         // Pid valid index, ex CurPidValidMap = 0x00000003 means Pid value index 0, 1 are valid
    WORD            PidValue[MAX_PIDS_TABLE_NUM];     // Pid value table
} PID_FILTER_INFO, *P_PID_FILTER_INFO;

enum CIMessage
{
// Old CI messages
	CI_STATUS_EMPTY_OLD = 0,		// NON_CI_INFO      0
	CI_STATUS_CAM_OK1_OLD,			// ME0				1
	CI_STATUS_CAM_OK2_OLD,			// ME1				2
	
	MMI_STATUS_GET_MENU_OK1_OLD,	// MMI0				3
	MMI_STATUS_GET_MENU_OK2_OLD,	// MMI1				4
	MMI_STATUS_GET_MENU_CLOSE1_OLD,	// MMI0_ClOSE		5
	MMI_STATUS_GET_MENU_CLOSE2_OLD,	// MMI1_ClOSE		6

// New CI messages
	CI_STATUS_EMPTY = 10,		// No CAM inserted
	CI_STATUS_INSERTED,			// CAM is inserted
	CI_STATUS_CAM_OK,			// Initila CAM OK
	CI_STATUS_CAM_UNKNOW,		// Unkonw CAM type

	MMI_STATUS_ANSWER_SEND,		// Communicating with CAM 
	MMI_STATUS_GET_MENU_OK,		// Get information from CAM
	MMI_STATUS_GET_MENU_FAIL,	// Fail to get information from CAM
	MMI_STATUS_GET_MENU_INIT,   // Init MMI
	MMI_STATUS_GET_MENU_CLOSE,  // Close MMI
	MMI_STATUS_GET_MENU_CLOSED, // MMI Closed

// Raw command messages
    RAW_CMD_STATUS_SEND = 30,
	RAW_CMD_STATUS_GET_DATA_OK,
};

// Get CI info. IOCTL cmd structure
typedef struct
{
	ULONG ulCIState;		// CI/CAM status
    ULONG ulMMIState;		// MMI status
	ULONG ulPMTState;
	ULONG ulEventMessage;	// Current event status
    ULONG ulRawCmdState;
	ULONG Reserved[7];
} THCIState, *PTHCIState;

// Get Old CI info. IOCTL cmd structure
typedef struct
{
	ULONG ulCIState;		// CI/CAM status
    ULONG ulMMIState;		// MMI status
} THCIStateOld, *PTHCIStateOld;

typedef struct AppInfoStruct
{
	unsigned int app_type;
	unsigned int application_manufacture;
	unsigned int manufacture_code;
	char application_info[64];
} THAppInfo, *PTHAppInfo;

typedef struct MMIInfoStruct
{
	char Header[256];
	char SubHeader[256];
	char ButtomLine[256];
	char MenuItem[9][42];
	int  ItemCount;

	BOOL EnqFlag;

	BOOL Blind_Answer;
	int  Answer_Text_Length;
	char Prompt[256];

	int  Answer;
	char AnswerStr[256];

    int  Type;
} THMMIInfo, *PTHMMIInfo;

enum RAWCMDSSTYPE
{
    SESSION_TYPE_PROFILE = 0,   // Profile session command
    SESSION_TYPE_APP,           // Application session command
    SESSION_TYPE_CA,            // CA session command
    SESSION_TYPE_MMI            // MMI sesstion command
};

typedef struct _RAW_CMD_INFO
{
	DWORD   dwSessionType;      // Raw command session type
    PVOID   pRawCmdBuff;        // Raw command buffer
    DWORD   dwRawCmdBuffSize;   // Raw command buffer size
    DWORD   Reserved[5];
} RAW_CMD_INFO, *PRAW_CMD_INFO;

#endif //THBDA_IOCTL_H
