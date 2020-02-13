#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#pragma warning( disable : 4995 4996 ) // no depreciated warnings
#include "NetworkProvider.h"
#include "Dll.h"

#include <stdio.h>
#include <WinVer.h>

#define DEFAULT_MOD_8PSK BDA_MOD_8VSB
#define DEFAULT_DISEQC DISEQC_COMMITED

enum VENDOR_SPECIFIC { PURE_BDA,
					   MS_BDA,
					   TT_BDA,
					   HAUP_BDA,
					   CXT_BDA,
					   TBS_BDA,
					   QBOX_BDA,
					   TBS_NXP_BDA,
					   TH_BDA,
					   DW_BDA,
					   TV_BDA,
					   BST_BDA,
					   OMC_BDA,
					   COMPRO_BDA,
					   ANYSEE_BDA,
					   GNP_BDA,
					   GENPIX_BDA,
					   NETUP_BDA,
					   CRAZY_BDA,
					   AMT_BDA};

enum LEVELS_TYPE { LEVELS_BDA, LEVELS_TT, LEVELS_STB };

#define MAX_TIMEOUT 10

typedef struct CONF_PARAMS
{
	char ConfVer[32];
	int ConfDiSEqC; 
	ModulationType ConfMod8PSK;
	int VendorSpecific;
	clock_t RelockTimeout;
} CONF_PARAMS;

class CConfiguration
{
public:
	CConfiguration();
	~CConfiguration();

	void MessageCallback(MSG_CB_PROC message_callback);
	void ReportMessage(char *text);

	void Configure(HINSTANCE);
	void ConfCaps();
	BOOLEAN ReadConfigurationFile();
	BOOLEAN CreateConfigurationFile();

	BOOLEAN DoConfigurationDialog();
	CONF_PARAMS conf_params;

private:
	MSG_CB_PROC message_callback;
};

#endif /* CONFIGURATION_H */
