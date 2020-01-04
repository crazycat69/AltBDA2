#ifndef DWBDA_H_INCLUDED_
#define DWBDA_H_INCLUDED_

#include <Streams.h>

#include <winioctl.h>
#include <ks.h>
#include <ksmedia.h>
#include <ksproxy.h>

#define PCI_TUNER_NAME TEXT("PCI BDA DVB-S Tuner")
#define PCI_CAPTURE_NAME TEXT("PCI BDA DVB-S Capture")
#define PCIE_2005_TUNER_NAME TEXT("PCIE DW2005 DVB-S2 Tuner")
#define PCIE_2005_CAPTURE_NAME TEXT("PCIE DW2005 DVB-S2 Capture")
#define PCIE_2006_TUNER_NAME TEXT("PCIE PCIE_2006 DVB-S2 Tuner")
#define PCIE_2006_CAPTURE_NAME TEXT("PCIE PCIE_2006 DVB-S2 Capture")
#define USB_2102_TUNER_NAME TEXT("USB 2102 DVB-S Tuner")
#define USB_2102_CAPTURE_NAME TEXT("USB 2102 DVB-S Tuner")
#define USB_2104_TUNER_NAME TEXT("USB 2104 DVB-S2 Tuner")
#define USB_2104_CAPTURE_NAME TEXT("USB 2104 DVB-S2 Tuner")
#define USB_2901_TUNER_NAME TEXT("USB 2901 DVB-T Tuner")
#define USB_2901_CAPTURE_NAME TEXT("USB 2901 DVB-T Capture")
#define USB_3101_TUNER_NAME TEXT("USB 3101 DVB-C Tuner")
#define USB_3101_CAPTURE_NAME TEXT("USB 3101 DVB-C Capture")


#define PCI_TUNER_GUID TEXT("{4c807f36-2db7-44ce-9582-e1344782cb85}")		//DW2002/4 PCI DVBS(2)
#define PCIE_2005_TUNER_GUID TEXT("{ede18552-45e6-469f-93b5-27e94296de38}")	//DW2005 PCI-E   DVBS2
#define USB_TUNER_GUID TEXT("{5a714cad-60f9-4124-b922-8a0557b8840e}")		//DW210X,DW2901,DW3101 USB2.0 DVBS(2), DVBT, DVBC

#define PCI_CAPTURE_GUID TEXT("{0a468d83-a2e8-45a9-b09e-fe8404c2d13a}")		 //DW2002/4 PCI DVBS(2)
#define PCIE_2005_CAPTURE_GUID TEXT("{76310c7b-e712-4564-94cc-dd69d6608989}")//DW2005 PCI-E DVBS2
#define USB_CAPTURE_GUID TEXT("{eaad090c-92ee-4dc9-a3e9-1c196bf95068}")		 //DW210X,DW2901,DW3101 USB2.0 DVBS(2), DVBT, DVBC

static const GUID IID_IKsObject =	//IKsObject interface GUID
{ 0x423c13a2, 0x2070, 0x11d0, { 0x9e, 0xf7, 0x00, 0xaa, 0x00, 0xa2, 0x16, 0xa1 } };

static const GUID GUID_READ_IDS =	//Read IDs
{ 0x9b56175, 0x4423, 0x47ba, { 0x82, 0x85, 0xdd, 0x6a, 0x70, 0xbf, 0x8, 0x4b } };

static const GUID GUID_DiseqcCmdSend =	//DiseqcCmd interface GUID
{ 0x61ae2cdf, 0x87e8, 0x445c, { 0x8a, 0x7, 0x35, 0x6e, 0xd2, 0x28, 0xfb, 0x4e } };

static const GUID GUID_TUNER_S_LOCK =	//Tuner Lock
{ 0x8bed860a, 0xa7b4, 0x4e90, { 0x9d, 0xf4, 0x13, 0x20, 0xc9, 0x49, 0x22, 0x61 } };

static const GUID GUID_SIGNAL_STATUS =	//Signal Status
{ 0xb2a693d0, 0x1295, 0x4b8e, { 0x8f, 0x3, 0x6, 0x27, 0xb0, 0x76, 0x98, 0xab } };

static const GUID GUID_HID_INTERFACE =	//HID
{ 0xdc0a8dca, 0x2c9c, 0x45d5, { 0x81, 0xf9, 0xdd, 0xb3, 0xf2, 0xba, 0x7e, 0xa6 } };

static const GUID GUID_READ_MAC =		//READ MAC
{ 0x334d58a, 0xc4f, 0x40aa, { 0xab, 0x21, 0x44, 0x59, 0xc3, 0xf1, 0x65, 0x5a } };

static const GUID GUID_I2CREADWRITE = 
{ 0x88b1e4fb, 0x6363, 0x41d9, { 0xa8, 0x6a, 0x65, 0x1a, 0x32, 0x9f, 0x58, 0x5b } };

static const GUID GUID_GET_CARDINFORMATION =
{ 0x4f52320c, 0x7ff7, 0x422b, { 0xbe, 0xfd, 0xdf, 0x96, 0x7b, 0x6c, 0x91, 0x39 } };

static const GUID GUID_ACCESS_SCREADER =
{ 0x68c75aeb, 0x7dc0, 0x43f6, { 0x94, 0x87, 0xed, 0xbe, 0x76, 0x15, 0xe1, 0x5 } };

static const GUID GUID_IPFILTER_CREATE =
{ 0x20736f9d, 0xbc78, 0x457a, { 0xaa, 0xb, 0x8c, 0x4c, 0x36, 0x7e, 0x13, 0xa3 } };

static const GUID GUID_IPFILTER_RELEASE =
{ 0x5177ad69, 0xdb7a, 0x4249, { 0x80, 0x36, 0xc1, 0x3e, 0x6f, 0x5f, 0x9c, 0x88 } };

static const GUID GUID_IPFILTER_START =
{ 0xa53c0956, 0xa3e4, 0x4a6e, { 0xa3, 0xba, 0x3, 0x66, 0x9c, 0x1b, 0x5f, 0x97 } };

static const GUID GUID_IPFILTER_STOP =
{ 0x11317d06, 0x38bd, 0x48bf, { 0x91, 0x6f, 0xb8, 0xd3, 0x1b, 0x8e, 0xe5, 0x7e } };

static const GUID GUID_GET_FILTERMAC =
{ 0x638a0eaf, 0x7bd2, 0x4464, { 0x90, 0xc0, 0x91, 0x36, 0xa1, 0xaf, 0x9d, 0x1 } };

static const GUID GUID_SET_FILTERMAC =
{ 0x4d9558ff, 0x8d55, 0x48c5, { 0xbb, 0x97, 0x0, 0xaa, 0xc0, 0xb0, 0xbf, 0x32 } };

static const GUID GUID_IPFILTER_ADDPID =
{ 0x68a55eea, 0xf819, 0x4ea6, { 0xa8, 0x2f, 0xa0, 0x74, 0x9d, 0x5b, 0xe2, 0xb8 } };

static const GUID GUID_IPFILTER_DELALLPID =
{ 0x5a362857, 0xbb45, 0x4d2d, { 0x94, 0xef, 0x8a, 0xba, 0x4c, 0x10, 0xde, 0xbe } };

static const GUID GUID_IPFILTER_GETPIDMAC =
{ 0x9aaedae1, 0xa15a, 0x4492, { 0x93, 0x48, 0x10, 0x73, 0x4e, 0xa5, 0x44, 0x8b } };

static const GUID GUID_IPFILTER_GETSTATISTIC =
{ 0xcfcdc5f6, 0x941b, 0x40b8, { 0xa5, 0x17, 0xa0, 0xe7, 0x7a, 0x36, 0xc4, 0xd1 } };


enum BUS_DW{
	BUS_UNKNOWN,
	BUS_USB1,
	BUS_USB2,
	BUS_USB3,
	BUS_PCI	= 0x11,
	BUS_PCIE = 0x12
};

enum FE_DW{
	FE_UNKNOWN,
	FE_DVBS,
	FE_DVBS2
};

typedef struct _DW_ID {
	DWORD VID;
	DWORD PID;
	BYTE FEType;	// 1:DVB-S; 2:DVB-S2;Others:Reserved.
	BYTE BusType;	// 1:USB1.0; 2:USB2.0;3:USB3.0,0x11:PCI2.0;0x12:PCIE
} DW_ID, *PDW_ID;

typedef struct _DWSignalStatus {
	int nStrength;	//0-100
	int nQuality;	//0-100
	int nLock;		//0:unlocked , >0:locked
	int reserved;
} DWSignalStatus, *PDWSignalStatus;

typedef struct _I2CReadWrite {
	//Write bytes then read.
	GUID GUID_ID;
	BYTE I2CDeviceNumber;
	//For DW2005 PCIE device,0:Internal;1:External I2C1;2:External I2C2.
	//For others, no use.The driver will use the default I2C on board.
	BYTE ReadBytesNumber;//MAX:256
	BYTE WriteBytesNumber;//MAX:256
	BYTE SubDeviceAddressNumber;//MAX:3//Please set it to 0.
	BYTE DeviceAddress;
	BYTE SubDeviceAddress[3];//No use.
	BYTE ReadBuffer[256];
	BYTE WriteBuffer[256];
}I2CReadWrite,*PI2CReadWrite;

typedef struct _CardInfo
{
	//Each hardware device own its SN. It is deferent from other boards.
	BYTE reserved1;
	BYTE SN[5];
	BYTE CIExtended;
	// CIExtended>3: CI module supported and with smart card reader interface.
	// Others: no CI module supported and smart card reader interface.
	BYTE reserved3;
}CardInfo,*PCardInfo;

HRESULT dwBDADeviceIoCtlProcess (HANDLE handle, PVOID pInBuffer, size_t InSize, PVOID pOutBuffer, size_t OutSize);

HRESULT dwBDAGetDeviceID(HANDLE handle, PDW_ID pDW_ID);
HRESULT dwBDAGetCardInfo(HANDLE handle, PCardInfo pCardInfo);
HRESULT dwBDAGetMAC(HANDLE handle, BYTE *pMAC);
HRESULT dwBDAReadEEPROM(HANDLE handle, BYTE* pBuffer, BYTE Index, BYTE Len);

HRESULT dwBDADiseqSendCommand(HANDLE handle, BYTE *pbCmd, ULONG ulLen);

HRESULT dwBDATune(HANDLE handle, ULONG lFrequency, ULONG lSymbolRate, ULONG lLnbFreq, UINT Pol, BOOL Force22KHz, UINT Fec, UINT mod,
				  UINT DiSeqCPort, UINT ToneBurst);

HRESULT dwBDAGetSignalStatus(HANDLE handle, PDWSignalStatus pDWSignalStatus);

LONG	dwBDAReadIRKey(HANDLE handle);

#endif // DW_H_INCLUDED_
