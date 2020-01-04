/* AltDVB v2.2 device interface, by Diodato */
/* PASCAL calling convention */

#include "stdafx.h"
#include "DvbDeviceControl.h"

CDvbDeviceControl *wrapper = NULL;

class CDllAltBDA2 : public CWinApp
{
public:
	CDllAltBDA2();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

// CDllAltBDA2
BEGIN_MESSAGE_MAP(CDllAltBDA2, CWinApp)
END_MESSAGE_MAP()

// CDllAltBDA2 construction
CDllAltBDA2::CDllAltBDA2()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CDllAltBDA2 theApp;

// CDllAltBDA2 initialization
BOOL CDllAltBDA2::InitInstance()
{
	CWinApp::InitInstance();

	char szAppPath[MAX_PATH];
	GetModuleFileName(AfxGetInstanceHandle(),szAppPath,MAX_PATH);
	free((void*)m_pszExeName);
	m_pszExeName = _strdup(szAppPath);
	char* pszTmp = strrchr(szAppPath,'.');
	if (pszTmp) *pszTmp='\0';

	strcat(szAppPath,".cfg");
	free((void*)m_pszProfileName);
	m_pszProfileName = _strdup(szAppPath);

	wrapper = new CDvbDeviceControl(AfxGetInstanceHandle());
	return wrapper!=NULL;
}

int __declspec(dllexport) __stdcall DvbDeviceControl(int id, char *data)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())
	switch(id)
	{
	case DRIVER_DESCRIPTION: /* get driver description */
		{
			DbgLog((LOG_TRACE,0,TEXT("DRIVER_DESCRIPTION")));
			return wrapper->DriverDescription((struct DRIVER_DATA *)data);
		}
		break;
	case DEVICE_DESCRIPTION: /* get device description */
		{
			DbgLog((LOG_TRACE,0,TEXT("DEVICE_DESCRIPTION")));
			return wrapper->DeviceDescription((struct DEVICE_DATA *)data);
		}
		break;
	case MESSAGE_CALLBACK: /* set message callback */
		{
			DbgLog((LOG_TRACE,0,TEXT("SET_MESSAGE_CALLBACK")));
			return wrapper->MessageCallback((MSG_CB_PROC)data);
		}
		break;
	case OPEN_DEVICE: /* open device */
		{
			DbgLog((LOG_TRACE,0,TEXT("OPEN_DEVICE")));
			return wrapper->OpenDevice((struct OPEN_DEVICE_DATA *)data);
		}
		break;
	case CLOSE_DEVICE: /* close device */
		{
			DbgLog((LOG_TRACE,0,TEXT("CLOSE_DEVICE")));
			return wrapper->CloseDevice((int *)data);
		}
		break;
	case DEVICE_NAME: /* get device name */
		{
			DbgLog((LOG_TRACE,0,TEXT("DEVICE_NAME")));
			return wrapper->DeviceName((struct DEVICE_NAME_DATA *)data);
		}
		break;
	case PID_FILTER: /* set PID filter */
		{
			DbgLog((LOG_TRACE,0,TEXT("PID_FILTER")));
			return wrapper->PidFilter((struct PID_FILTER_DATA *)data);
		}
		break;
	case STREAM_CALLBACK: /* set TS packet callback */
		{
			DbgLog((LOG_TRACE,0,TEXT("STREAM_CALLBACK")));
			return wrapper->StreamCallback((STR_CB_PROC)data);
		}
		break;
	case TUNE: /* perform tuning */
		{
			DbgLog((LOG_TRACE,0,TEXT("TUNE")));
			return wrapper->Tune((struct TUNE_DATA *)data);
		}
		break;
	case SIGNAL_STATISTICS: /* get signal statistics */
		{
			DbgLog((LOG_TRACE,0,TEXT("SIGNAL_STATISTICS")));
			return wrapper->SignalStatistics((struct SIGNAL_STATISTICS_DATA *)data);
		}
		break;
	case DISEQC_COMMAND: /* send DiSEqC command */
		{
			DbgLog((LOG_TRACE,0,TEXT("DISEQC_COMMAND")));
			return wrapper->DiSEqC_Command((struct DISEQC_COMMAND_DATA *)data);
		}
		break;
	case DISEQC_SUPPORT: /* get DiSEqC support */
		{
			DbgLog((LOG_TRACE,0,TEXT("DISEQC_SUPPORT")));
			return wrapper->DiSEqC_Support((struct DISEQC_SUPPORT_DATA *)data);
		}
		break;
	case DEVICE_CONFIG: /* set private device config (GUI) */
		{
			DbgLog((LOG_TRACE,0,TEXT("DEVICE_CONFIG")));
			return wrapper->DeviceConfig();
		}
		break;
	case DEVICE_WHOLE_TRANSP: /* get device's capability to open whole transponder */
		{
			DbgLog((LOG_TRACE,0,TEXT("DEVICE_WHOLE_TRANSP")));
			return wrapper->DeviceOpensWholeTransponder((struct WHOLE_TRANSPONDER_DATA *)data);
		}
		break;
	default: /* shouldn't be called */
		{
			DebugLog("BDA2: DvbDeviceControl: id=0x%8.8x", id);
		}
	}
	return AltxDVB_OK;
}

void DebugLog( const char *format, ... )
{
	char buffer[1024] = { 0 };

	va_list args;
	va_start( args, format );

	vsprintf_s( buffer, _vscprintf( format, args )+1, format, args );

	va_end( format );

	OutputDebugStringA( buffer );
	OutputDebugStringA("\n");
}
