#ifndef DLL_H
#define DLL_H
/* AltDVB v2.3 device interface, by Diodato */
/* PASCAL calling convention */
/* int is 4B, short is 2B, char is 1B, enum is 4B */

/* functions */
#define DRIVER_DESCRIPTION	0x10000001
#define DEVICE_DESCRIPTION	0x10000002
#define MESSAGE_CALLBACK	0x10000003
#define OPEN_DEVICE			0x10000004
#define CLOSE_DEVICE		0x10000005
#define DEVICE_NAME			0x10000006
#define PID_FILTER			0x10000007
#define STREAM_CALLBACK		0x10000008
#define TUNE				0x10000009
#define SIGNAL_STATISTICS	0x1000000A
#define DEVICE_CONFIG		0x1000000B
#define DISEQC_COMMAND		0x1000000C
#define DISEQC_SUPPORT		0x1000000D
#define _E_ID 0x1000000E /* not used by AltDVB */
#define DEVICE_WHOLE_TRANSP	0x1000000F

/* function return values */
#define AltxDVB_OK 0x0
#define AltxDVB_ERR 0x1

/* single function to be exported by the Dev_*.int */
int __declspec(dllexport) __stdcall DvbDeviceControl(int,char *);

/* message display callback */
typedef int (__stdcall *MSG_CB_PROC)(int,char *);
/* stream single TS packet callback */
typedef void (__stdcall *STR_CB_PROC)(BYTE *);

struct DRIVER_DATA // length 0x184 == 388B
{
	char author[128];
	char url[128];
	char description[128];
	int extra;
};

/* ain't using bitfields, they are too compiler dependant */
enum DEVICE_TYPE { DVBS=0x1, DVBC=0x2, DVBT=0x3, ATSC=0x4, ERR=0x9, AVAILABLE=0x00010000};

#define MAX_DEVICES 64
struct DEVICE_DATA // length 0x824 == 2084B
{
	int dev_cnt; // up to 64 devices
	struct
	{
		DWORD dev_type;
		char dev_name[128];
		char dev_id[128];
	} device[MAX_DEVICES];
};

#define MESSAGE_CALLBACK_ID1 0x20000001 // they don't show-up inthe initial popup
#define MESSAGE_CALLBACK_ID2 0x20000002 // they show in initial popup
#define MESSAGE_CALLBACK_ID3 0x20000003 // they don't show-up inthe initial popup
#define MESSAGE_CALLBACK_ID4 0x20000004 // they don't show-up inthe initial popup
struct MESSAGE_CALLBACK_DATA
{
	int h;
	char *message;
};

struct OPEN_DEVICE_DATA // length 0x82 == 130B
{
	short int dev_type;
	char dev_id[128];
};

struct DEVICE_NAME_DATA // length 0x80 == 128B
{
	char description[128];
};

enum PID_OPERATION { PID_DISABLE=0x0001, PID_ENABLE=0x0002 };
struct PID_FILTER_DATA
{
	short int operation;
	short int pid; // 0x2000 - all PIDs
};

enum INVERSION { INV_AUTO=0, INV_OFF, INV_ON };

enum MODULATION { MOD_AUTO=0, MOD_QPSK, MOD_8PSK, MOD_QPSK2,
	MOD_QAM_16, MOD_QAM_32, MOD_QAM_64, MOD_QAM_128, MOD_QAM_256,
	MOD_COFDM, MOD_VSB_8, MOD_VSB_16,
	MOD_TQPSK, MOD_T8PSK, MOD_T16QAM, MOD_BPSK,
	MOD_8APSK, MOD_16APSK, MOD_32APSK, MOD_64APSK, MOD_128APSK, MOD_256APSK };

enum BANDWIDTH { BANDW_AUTO = 0, BANDW_6=6, BANDW_7=7, BANDW_8=8, BANDW_10=10 };

enum FEC { FEC_AUTO=0, FEC_1_2=0x12, FEC_2_3=0x23, FEC_3_4=0x34, FEC_4_5=0x45,
	FEC_5_6=0x56, FEC_6_7=0x67, FEC_7_8=0x78, FEC_8_9=0x89, FEC_9_10=0x9a };

enum POLARITY { POL_NONE=0x0, LINEAR_VERTICAL=0x0D, LINEAR_HORIZONTAL=0x12,
	LNBPOWER_OFF=0x80, LNBPOWER_ON=0x81, CIRCULAR_RIGHT=0x8D, CIRCULAR_LEFT=0x92};

enum SWITCHES { SW_NONE=0x0, TONEBURST_A=0x1, TONEBURST_B=0x2,
	POS_A_OPT_A=0x3, POS_B_OPT_A=0x4, POS_A_OPT_B=0x5, POS_B_OPT_B=0x6 };

enum PILOT { PILOT_AUTO=0, PILOT_OFF, PILOT_ON };

enum ROLLOFF { ROLLOFF_AUTO=0, ROLLOFF_05, ROLLOFF_10, ROLLOFF_15, ROLLOFF_20, ROLLOFF_25, ROLLOFF_35 };

enum PLS_MODE { PLS_NONE=0, PLS_ROOT, PLS_GOLD };

enum DELSYS { DELSYS_NONE = 0,
			  DELSYS_DVBS = 0x10,
			  DELSYS_DVBS2 = 0x11,
			  DELSYS_DVBCA = 0x20,
			  DELSYS_DVBCB = 0x21,
			  DELSYS_DVBCC = 0x22,
			  DELSYS_DVBC2 = 0x23,
			  DELSYS_DVBT = 0x30,
			  DELSYS_DVBT2 = 0x31,
			  DELSYS_ATSC = 0x40,
			  DELSYS_ATSCMH = 0x41,
			  DELSYS_DVBH = 0x50,
			  DELSYS_DAB = 0x51};

struct TUNE_DATA
{
	int frequency; // multiplier is 1000
	int inversion;
	int modulation;
	int symbol_rate;
	int bandwidth;
	int fec;
	int polarity;
	int switches;
	int lnb_low;
	int lnb_high;
	int lnb_switch;
	int rolloff;
	int pilot;
	int pls_mode;
	int pls_code;
	int isi;
	int delsys;
	int par17;
	int par18;
};

struct SIGNAL_STATISTICS_DATA
{
	short int strength; // <0-100> == <0x00-0x64>
	short int quality; // <0-100> == <0x00-0x64>
	short int flags; // <0-31> == <0x00-0x1F>; bit 0x10 is locked; bits 0x0f are signal present
};

struct DISEQC_COMMAND_DATA
{
	BYTE len;
	BYTE DiSEqC_Command[256];
};

enum DISEQC_TYPE { DISEQC_NONE=0x0, DISEQC_TONEBURST=0x1, DISEQC_COMMITED=0x2, DISEQC_RAW=0x8 };
struct DISEQC_SUPPORT_DATA
{
	enum DISEQC_TYPE DiSEqC_Support;
};

struct WHOLE_TRANSPONDER_DATA
{
	int OpensWholeTransponder; // 0x1 - device can open whole transponder, 0x0 - no it cannot
};

#endif /* DLL_H */

