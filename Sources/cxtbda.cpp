#include "stdafx.h"

#include "BdaGraph.h"
#include "cxtbda.h"

HRESULT CBdaGraph::DVBS_HAUP_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	HRESULT hr;
	char text[256];

	DISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
	CopyMemory(diseqc_msg.uc_diseqc_send_message, DiSEqC_Command, len);
	diseqc_msg.ul_diseqc_send_message_length = len;
	diseqc_msg.ul_diseqc_receive_message_length = 0;
	diseqc_msg.ul_amplitude_attenuation = 3;
	diseqc_msg.b_tone_burst_modulated = TONE_BURST_MODULATED;
	diseqc_msg.diseqc_version = DISEQC_VER_1X;
	diseqc_msg.receive_mode = RXMODE_NOREPLY;
	diseqc_msg.b_last_message = TRUE; // last_message

	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionPropertiesHaup,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&diseqc_msg, sizeof(diseqc_msg), &diseqc_msg, sizeof(diseqc_msg));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_HAUP_DiSEqC: failed sending DISEQC_MESSAGE_PARAMS (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_HAUP_DiSEqC: sent DISEQC_MESSAGE_PARAMS");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_CXT_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	HRESULT hr;
	char text[256];

	DISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
	CopyMemory(diseqc_msg.uc_diseqc_send_message, DiSEqC_Command, len);
	diseqc_msg.ul_diseqc_send_message_length = len;
	diseqc_msg.ul_diseqc_receive_message_length = 0;
	diseqc_msg.ul_amplitude_attenuation = 3;
	diseqc_msg.b_tone_burst_modulated = TONE_BURST_MODULATED;
	diseqc_msg.diseqc_version = DISEQC_VER_1X;
	diseqc_msg.receive_mode = RXMODE_NOREPLY;
	diseqc_msg.b_last_message = TRUE;

	KSPROPERTY instance_data;
	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&instance_data, sizeof(instance_data), &diseqc_msg, sizeof(diseqc_msg));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_CXT_DiSEqC: failed sending DISEQC_MESSAGE_PARAMS (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_CXT_DiSEqC: sent DISEQC_MESSAGE_PARAMS");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_BST_DiSEqC(BYTE len, BYTE *DiSEqC_Command)
{
	CheckPointer(DiSEqC_Command,E_POINTER);
	if ((len==0) || (len>6))
		return E_INVALIDARG;

	HRESULT hr;
	char text[256];

	DISEQC_MESSAGE_PARAMS diseqc_msg;
	Z(diseqc_msg);
	CopyMemory(diseqc_msg.uc_diseqc_send_message, DiSEqC_Command, len);
	diseqc_msg.ul_diseqc_send_message_length = len;
	diseqc_msg.ul_diseqc_receive_message_length = 0;
	diseqc_msg.ul_amplitude_attenuation = 3;
	diseqc_msg.b_tone_burst_modulated = TONE_BURST_MODULATED;
	diseqc_msg.diseqc_version = DISEQC_VER_1X;
	diseqc_msg.receive_mode = RXMODE_NOREPLY;
	diseqc_msg.b_last_message = TRUE;

	KSPROPERTY instance_data;
	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionPropertiesBst,
		KSPROPERTY_BDA_DISEQC_MESSAGE,
		&instance_data, sizeof(instance_data), &diseqc_msg, sizeof(diseqc_msg));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_BST_DiSEqC: failed sending DISEQC_MESSAGE_PARAMS (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_BST_DiSEqC: sent DISEQC_MESSAGE_PARAMS");
	ReportMessage(text);

	return S_OK;
}

HRESULT CBdaGraph::DVBS_CXT_LNBPower(BOOL bPower)
{
	HRESULT hr;
	char text[256];
	KSPROPERTY instance_data;

	hr = m_pKsTunerPropSet->Set(KSPROPSETID_BdaTunerExtensionProperties,
		KSPROPERTY_BDA_LNB_POWER,
		&instance_data, sizeof(instance_data), &bPower, sizeof(bPower));
	if(FAILED(hr))
	{
		sprintf(text,"BDA2: DVBS_CXT_LNBPower: failed set LNB Power state (0x%8.8x)", hr);
		ReportMessage(text);
		return E_FAIL;
	}
	sprintf(text,"BDA2: DVBS_CXT_LNBPower: set LNB Power state");
	ReportMessage(text);

	return S_OK;
}
