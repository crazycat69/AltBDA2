#ifndef ASBDA_H
#define ASBDA_H

const GUID GUID_ANYSEE_CAPTURE_FILTER_PROPERTY = 
{ 0xb8e78938, 0x899d, 0x41bd, { 0xb5, 0xb4, 0x62, 0x69, 0xf2, 0x80, 0x18, 0x99 } };

#define PROPERTY_SEND_DiSEqC_DATA   24
 
typedef enum _EnumDiSEqCToneBurst_
{
	No_DiSEqCToneBurst = 0,
	SA_DiSEqCToneBurst,
	SB_DiSEqCToneBurst
}EnumDiSEqCToneBurst;
 
typedef struct _USERPROPERTY_DiSEqC_S_
{
	KSPROPERTY Property; 
	DWORD dwLength;       // DiSEqC Data(include command) Length
	BYTE Data[16];       // DiSEqC Data
	BYTE ToneBurst;      // EnumDiSEqCToneBurst
}PROPERTY_DiSEqC_S, *PPROPERTY_DiSEqC_S;

#endif ASBDA_H

