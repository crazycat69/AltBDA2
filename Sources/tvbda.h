#ifndef TVBDA_H
#define TVBDA_H

const GUID KSPROPSETID_BdaTunerExtensionPropertiesTeViiS420 = {0xfaa8f3e5, 0x9022, 0xd420, 0x88, 0xef, 0xd9, 0xeb, 0x71, 0x6f, 0x6e, 0xc9};
const GUID KSPROPSETID_BdaTunerExtensionPropertiesTeViiS460 = {0x16FAAC60, 0x9022, 0xd420, 0x15, 0x20, 0xd9, 0xeb, 0x71, 0x6f, 0x6e, 0xc9};

typedef enum 
{
	KSPROPERTY_BDA_1 =0,
	KSPROPERTY_BDA_2,
	KSPROPERTY_BDA_DISEQC,
	KSPROPERTY_BDA_IR_GET,
	KSPROPERTY_BDA_IR_SET
} KSPROPERTY_BDA_TUNER_EXTENSION2;

const BYTE CX_DISEQC_TX_BUFFER_SIZE = 150;

typedef struct _PRIVATE_DISEQC
{
    UCHAR		message[CX_DISEQC_TX_BUFFER_SIZE+1];
    UCHAR		length;
	BOOL		b_last_message;
	int			power;
} PRIVATE_DISEQC, *PPRIVATE_DISEQC;

#endif TVBDA_H