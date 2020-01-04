#ifndef CMPBDA_H
#define CMPBDA_H

// GUID of DiSEqC Property
// {0c12bf87-5bc0-4dda-9d07-21e5c2f3b9ae}
static const GUID VAMP_DVBS_DISEQC_ACCESS_PROPERTY = 
{ 0x0c12bf87, 0x5bc0, 0x4dda, { 0x9d, 0x07, 0x21, 0xe5, 0xc2, 0xf3, 0xb9, 0xae } };

typedef enum
{
	VAMP_DVBS_DISEQC_PROP_ID_COMMAND = 0,
	VAMP_DVBS_DISEQC_PROP_ID_RAW_COMMAND,
	VAMP_DVBS_DISEQC_PROP_ID_LNB_CTRL
} _VAMP_DVBS_DISEQC_ACCESS_PROPERTY;

// Enable / Disable 22kHz Tone
typedef enum
{
	L22KHZ_TONE_OFF = 0,
	L22KHZ_TONE_ON,
	POWER_OFF,
	POWER_ON
} LNB_CTRL;

#endif CMPBDA_H
