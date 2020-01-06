#include "BdaGraph.h"

class CDvbDeviceControl: public CConfiguration
{
public:
	CDvbDeviceControl(HMODULE);
	~CDvbDeviceControl();

	int DriverDescription(struct DRIVER_DATA *);
	int DeviceDescription(struct DEVICE_DATA *);
	int MessageCallback(MSG_CB_PROC);
	void ReportMessage(char *text);
	int OpenDevice(struct OPEN_DEVICE_DATA *);
	int CloseDevice(int *);
	int DeviceName(struct DEVICE_NAME_DATA *);
	int PidFilter(struct PID_FILTER_DATA *);
	int StreamCallback(STR_CB_PROC);
	int Tune(struct TUNE_DATA *);
	int SignalStatistics(struct SIGNAL_STATISTICS_DATA *);
	int DeviceConfig(void);
	int DiSEqC_Command(struct DISEQC_COMMAND_DATA *);
	int DiSEqC_Support(struct DISEQC_SUPPORT_DATA *);
	int DeviceOpensWholeTransponder(struct WHOLE_TRANSPONDER_DATA *);

private:
	int selected_device_enum;
	DEVICE_TYPE selected_device_type;

	HINSTANCE hInstance;

	MSG_CB_PROC message_callback;
	STR_CB_PROC stream_callback;

	CBdaGraph BdaGraph;
};

