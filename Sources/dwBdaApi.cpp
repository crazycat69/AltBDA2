#include "stdafx.h"
#include "dwBdaApi.h"

HRESULT dwBDADeviceIoCtlProcess (HANDLE handle, PVOID pInBuffer, size_t InSize, PVOID pOutBuffer, size_t OutSize)
{
	//We always treat the InBuffer beginning with a valid GUID!
	//The InBuffer size must > sizeof(GUID)!
	//When the InBuffer includes one GUID only,pls init the InBuffer as
	//KSPROPERTY!
	CheckPointer(handle,E_POINTER);
	if(InSize <= sizeof(GUID))
		return E_INVALIDARG ;
	ULONG ret = 0;
	return KsSynchronousDeviceControl(
		handle,
		IOCTL_KS_PROPERTY,
		pInBuffer,
		InSize,
		pOutBuffer,
		OutSize,
		&ret);
}

HRESULT dwBDAGetDeviceID(HANDLE handle, PDW_ID pDW_ID)
{
	CheckPointer(handle,E_POINTER);
	CheckPointer(pDW_ID,E_POINTER);

	KSPROPERTY Property;
	ZeroMemory(&Property,sizeof(KSPROPERTY));
	Property.Set = GUID_READ_IDS;
	return dwBDADeviceIoCtlProcess(handle, &Property, sizeof(KSPROPERTY), pDW_ID, sizeof(DW_ID));
}

HRESULT dwBDAGetCardInfo(HANDLE handle, PCardInfo pCardInfo)
{
	CheckPointer(handle,E_POINTER);
	CheckPointer(pCardInfo,E_POINTER);

	KSPROPERTY Property;
	ZeroMemory(&Property,sizeof(KSPROPERTY));
	Property.Set = GUID_GET_CARDINFORMATION;
	return dwBDADeviceIoCtlProcess(handle, &Property, sizeof(KSPROPERTY), pCardInfo, sizeof(CardInfo));
}

HRESULT dwBDAGetMAC(HANDLE handle, BYTE* pMAC)
{
	CheckPointer(handle,E_POINTER);
	CheckPointer(pMAC,E_POINTER);

	KSPROPERTY Property;
	ZeroMemory(&Property,sizeof(KSPROPERTY));
	Property.Set = GUID_READ_MAC;
	return dwBDADeviceIoCtlProcess(handle, &Property, sizeof(KSPROPERTY), pMAC, 6);
}

HRESULT dwBDADiseqSendCommand(HANDLE handle, BYTE *pbCmd, ULONG ulLen)
{
	CheckPointer(handle,E_POINTER);
	CheckPointer(pbCmd,E_POINTER);

	if ( (ulLen==0) || (ulLen>256) )
		return E_INVALIDARG;

	struct DiseqcCmd
	{
		GUID GUID_ID;
		unsigned char cmd[256];
		unsigned long cmd_len;
	};
	DiseqcCmd tempCmd;
	ZeroMemory(&tempCmd,sizeof(DiseqcCmd));
	tempCmd.GUID_ID = GUID_DiseqcCmdSend;
	memcpy(tempCmd.cmd,pbCmd,ulLen);
	tempCmd.cmd_len = ulLen;
	return dwBDADeviceIoCtlProcess(handle, &tempCmd, sizeof(DiseqcCmd), NULL, 0 );
}

HRESULT dwBDATune(HANDLE handle, ULONG lFrequency, ULONG lSymbolRate, ULONG lLnbFreq, UINT Pol, BOOL Force22KHz, UINT Fec, UINT mod, UINT DiSeqCPort, UINT ToneBurst )
{
	CheckPointer(handle,E_POINTER);

	struct Tuner_S_Param2
	{
		GUID GUID_ID;
		ULONG symbol_rate;//ksps
		ULONG frequency;//kHz
		ULONG lnb;//Khz
		UINT hv; //1:H.0:V.
		BOOL b22k;
		UINT diseqcPort;//1-4.
		UINT FEC; //Refer to “3.More FEC types and Modulation types”
		UINT Modulation;
		//1:DVB-S1,QPSK
		//2:DVB-S2,QPSK
		//3:DVB-S2,8PSK
		UINT Burst;//0:Undefined burst;1:Burst A;2:Burst B.
	};
	Tuner_S_Param2 TunerParam;
	TunerParam.GUID_ID = GUID_TUNER_S_LOCK;
	TunerParam.frequency = lFrequency;
	TunerParam.symbol_rate = lSymbolRate;
	TunerParam.hv = Pol;
	TunerParam.lnb = lLnbFreq;
	TunerParam.b22k = Force22KHz;
	TunerParam.Burst = ToneBurst;
	TunerParam.diseqcPort = DiSeqCPort;
	TunerParam.FEC = Fec;
	//Lock Channel
	return dwBDADeviceIoCtlProcess(handle, &TunerParam, sizeof(TunerParam), NULL, 0 );
}

HRESULT dwBDAGetSignalStatus(HANDLE handle, PDWSignalStatus pDWSignalStatus)
{
	CheckPointer(handle,E_POINTER);
	CheckPointer(pDWSignalStatus,E_POINTER);

	KSPROPERTY Property;
	ZeroMemory(&Property,sizeof(KSPROPERTY));
	Property.Set = GUID_SIGNAL_STATUS;
	return dwBDADeviceIoCtlProcess(handle, &Property, sizeof(KSPROPERTY), pDWSignalStatus, sizeof(DWSignalStatus));
}

HRESULT dwBDAReadEEPROM(HANDLE handle, BYTE* pBuffer, BYTE Index, BYTE Len)
{
	CheckPointer(handle,E_POINTER);
	CheckPointer(pBuffer,E_POINTER);
	
	Sleep(5);
	I2CReadWrite InBuffer;
	I2CReadWrite OutBuffer;
	ZeroMemory(&InBuffer,sizeof(I2CReadWrite));
	ZeroMemory(&OutBuffer,sizeof(I2CReadWrite));
	InBuffer.GUID_ID = GUID_I2CREADWRITE;
	InBuffer.DeviceAddress = 0xA0;//Default EEPROM device address.
	InBuffer.I2CDeviceNumber = 1; //Use DW2005 CX23885 first external I2C.
	InBuffer.ReadBytesNumber = Len;
	InBuffer. SubDeviceAddressNumber = 0;
	InBuffer.WriteBuffer[0] = Index;
	InBuffer.WriteBytesNumber = 1;//Write the index only.
	HRESULT hr = dwBDADeviceIoCtlProcess(handle, &InBuffer, sizeof(I2CReadWrite), &OutBuffer, sizeof(I2CReadWrite));
	if SUCCEEDED(hr)
		CopyMemory(pBuffer,OutBuffer.ReadBuffer,Len);
	return hr;
}

LONG dwBDAReadIRKey(HANDLE handle)
{
	CheckPointer(handle,E_POINTER);

	KSPROPERTY Property;
	ZeroMemory(&Property,sizeof(KSPROPERTY));
	Property.Set = GUID_HID_INTERFACE;
	LONG key = -1;
	if SUCCEEDED(dwBDADeviceIoCtlProcess(handle, &Property, sizeof(KSPROPERTY), &key, sizeof(key)))
		return key;
	return -1;
}
