#include "stdafx.h"
#include "BdaGraph.h"

//THBDA Ioctl functions
BOOL CBdaGraph::THBDA_IOControl( DWORD  dwIoControlCode,
									LPVOID lpInBuffer,
									DWORD  nInBufferSize,
									LPVOID lpOutBuffer,
									DWORD  nOutBufferSize,
									LPDWORD lpBytesReturned)
{
    if (!m_pKsTunerPropSet)
        return FALSE;

    KSPROPERTY instance_data;

    ULONG    ulOutBuf = 0;
    ULONG    ulReturnBuf = 0;
    THBDACMD THBDACmd;

    THBDACmd.CmdGUID = GUID_THBDA_CMD;
    THBDACmd.dwIoControlCode = dwIoControlCode;
    THBDACmd.lpInBuffer = lpInBuffer;
    THBDACmd.nInBufferSize = nInBufferSize;
    THBDACmd.lpOutBuffer = lpOutBuffer;
    THBDACmd.nOutBufferSize = nOutBufferSize;
    THBDACmd.lpBytesReturned = lpBytesReturned;

    HRESULT hr = m_pKsTunerPropSet->Set(GUID_THBDA_TUNER, 
                              NULL, 
	  						  &instance_data, sizeof(instance_data),
                              &THBDACmd, sizeof(THBDACmd));

    return SUCCEEDED(hr);
}


//***************************************************************//
//************** Basic IOCTL sets  (must support) ***************//
//***************************************************************//

BOOL CBdaGraph::THBDA_IOCTL_CHECK_INTERFACE_Fun(void)
{
    BOOL bResult = FALSE;
    DWORD nBytes = 0;

    bResult = THBDA_IOControl((DWORD) THBDA_IOCTL_CHECK_INTERFACE,
									NULL,
									0,
									NULL,
									0,
									(LPDWORD)&nBytes);
    if (bResult==FALSE)	{
		OutputDebugString(TEXT("IOCTL Error: THBDA_IOCTL_CHECK_INTERFACE_Fun failed! \n"));
	}
    return bResult;
}

//***************************************************************//
//********************* DVB-S (must support)*********************//
//***************************************************************//

BOOL CBdaGraph::THBDA_IOCTL_SET_LNB_DATA_Fun(LNB_DATA *pLNB_DATA)
{	
	BOOLEAN bResult	= FALSE;
	DWORD   nBytes  = 0;	

	bResult = THBDA_IOControl(	(DWORD)THBDA_IOCTL_SET_LNB_DATA,
							(LPVOID)pLNB_DATA, 
							sizeof(LNB_DATA),     
							NULL, 
							0,                    
							(LPDWORD)&nBytes);
	if (bResult==FALSE)	{
		OutputDebugString(TEXT("IOCTL Error: THBDA_IOCTL_SET_LNB_DATA_Fun failed! \n"));
	}

	return bResult;
}

BOOL CBdaGraph::THBDA_IOCTL_GET_LNB_DATA_Fun(LNB_DATA *pLNB_DATA)
{	
	BOOLEAN bResult	= FALSE;
	DWORD   nBytes  = 0;	

	bResult = THBDA_IOControl(	(DWORD)THBDA_IOCTL_GET_LNB_DATA,							  
							NULL, 
							0,
							(LPVOID)pLNB_DATA, 
							sizeof(LNB_DATA),                       
							(LPDWORD)&nBytes);
	if (bResult==FALSE)	{
		OutputDebugString(TEXT("IOCTL Error: THBDA_IOCTL_GET_LNB_DATA_Fun failed! \n"));
	}

	return bResult;
}

BOOL CBdaGraph::THBDA_IOCTL_SET_DiSEqC_Fun(DiSEqC_DATA *pDiSEqC_DATA)
{	
	BOOLEAN bResult	= FALSE;
	DWORD   nBytes  = 0;	

	bResult = THBDA_IOControl(	(DWORD)THBDA_IOCTL_SET_DiSEqC,
							(LPVOID)pDiSEqC_DATA, 
							sizeof(DiSEqC_DATA),     
							NULL, 
							0,                    
							(LPDWORD)&nBytes);
	if (bResult==FALSE)	{
		OutputDebugString(TEXT("IOCTL Error: THBDA_IOCTL_SET_DiSEqC_Fun failed! \n"));
	}

	return bResult;
}

BOOL CBdaGraph::THBDA_IOCTL_GET_DiSEqC_Fun(DiSEqC_DATA *pDiSEqC_DATA)
{	
	BOOLEAN bResult	= FALSE;
	DWORD   nBytes  = 0;	

	bResult = THBDA_IOControl(	(DWORD)THBDA_IOCTL_GET_DiSEqC,							  
							NULL, 
							0,
							(LPVOID)pDiSEqC_DATA, 
							sizeof(DiSEqC_DATA),                       
							(LPDWORD)&nBytes);
	if (bResult==FALSE)	{
		OutputDebugString(TEXT("IOCTL Error: THBDA_IOCTL_GET_DiSEqC_Fun failed! \n"));
	}

	return bResult;
}
