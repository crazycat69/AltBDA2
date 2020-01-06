#include "stdafx.h"

#include "DvbDeviceControl.h"

int RFScanMode=1;
CDvbDeviceControl::CDvbDeviceControl(HMODULE hModule)
{
	hInstance = hModule;
	stream_callback = NULL;
	selected_device_enum = 0;
	message_callback = NULL;
	conf_params.ConfVer[0] = 0x0;
	conf_params.ConfMod8PSK = DEFAULT_MOD_8PSK;
	conf_params.ConfDiSEqC = DEFAULT_DISEQC;
}

CDvbDeviceControl::~CDvbDeviceControl()
{
}

int CDvbDeviceControl::DriverDescription(struct DRIVER_DATA *d)
{
	sprintf(d->author,"Diodato & CrazyCat");
	sprintf(d->description,"BDA DVB-S/T/C Interface");
	sprintf(d->url,"http://code.google.com/p/altbda2");

	return AltxDVB_OK;
}

int CDvbDeviceControl::DeviceDescription(struct DEVICE_DATA *d)
{
	struct NetworkTuners Tuners;

	BdaGraph.GetNetworkTuners(&Tuners);
	d->dev_cnt = Tuners.Count;

	for(int i=0; i<Tuners.Count; ++i)
	{
		if(IsEqualGUID(Tuners.Tuner[i].Type, CLSID_DVBSNetworkProvider))
			d->device[i].dev_type = DVBS | (Tuners.Tuner[i].Availability ? AVAILABLE : 0x0);
		else
		if(IsEqualGUID(Tuners.Tuner[i].Type, CLSID_DVBTNetworkProvider))
			d->device[i].dev_type = DVBT | (Tuners.Tuner[i].Availability ? AVAILABLE : 0x0);
		else
		if(IsEqualGUID(Tuners.Tuner[i].Type, CLSID_DVBCNetworkProvider))
			d->device[i].dev_type = DVBC | (Tuners.Tuner[i].Availability ? AVAILABLE : 0x0);
		else
			d->device[i].dev_type = ERR;

		strcpy(d->device[i].dev_name, Tuners.Tuner[i].Name);
		sprintf(d->device[i].dev_id, "%d", Tuners.Tuner[i].Id);
	}

	return AltxDVB_OK;
}

int CDvbDeviceControl::MessageCallback(MSG_CB_PROC d)
{
	message_callback = d;
	CConfiguration::MessageCallback(d);
	BdaGraph.MessageCallback(d);
	return AltxDVB_OK;
}

void CDvbDeviceControl::ReportMessage(char *text)
{
	DebugLog(text);
	if(message_callback)
	{
		struct MESSAGE_CALLBACK_DATA d;

		d.h = 0x0;
		d.message = text;

		message_callback(MESSAGE_CALLBACK_ID2, (char *)&d);
	}
}

int CDvbDeviceControl::OpenDevice(struct OPEN_DEVICE_DATA *d)
{
	if(sscanf(d->dev_id,"%d",&selected_device_enum) != 1)
		return AltxDVB_ERR;
	selected_device_type = (DEVICE_TYPE)(d->dev_type);
	Configure(hInstance);
	if(SUCCEEDED(BdaGraph.BuildGraph(selected_device_enum, (enum VENDOR_SPECIFIC *)&(conf_params.VendorSpecific))))
	{
		//char selected_device_name[128];
		//WideCharToMultiByte(CP_ACP, 0, BdaGraph.friendly_name_tuner, -1, selected_device_name, sizeof(selected_device_name), NULL, NULL);
		ConfCaps();
		if(SUCCEEDED(BdaGraph.RunGraph()))
			return AltxDVB_OK;
		else
			BdaGraph.CloseGraph();
	}
	else
		BdaGraph.CloseGraph();

	return AltxDVB_ERR; // non 0x0 quits AltDVB
}

int CDvbDeviceControl::CloseDevice(int *d)
{
	BdaGraph.CloseGraph();
	return AltxDVB_OK;
}

int CDvbDeviceControl::DeviceName(struct DEVICE_NAME_DATA *d)
{
	char txt[121];

	if(selected_device_enum == 0x0)
		return AltxDVB_ERR;

	WideCharToMultiByte(CP_ACP, 0, BdaGraph.friendly_name_tuner, -1, txt, sizeof(txt), NULL, NULL);

	if (conf_params.VendorSpecific==TT_BDA)
		BdaGraph.DVBS_TT_GetProdName(txt,sizeof(txt));

	sprintf(d->description,"BDA2 [%s]",txt);

	return AltxDVB_OK;
}

int CDvbDeviceControl::PidFilter(struct PID_FILTER_DATA *d)
{
	return AltxDVB_OK; // d->pid == 0x2000 means all PIDs
}

int CDvbDeviceControl::StreamCallback(STR_CB_PROC d)
{
	stream_callback = d;
	BdaGraph.SetStreamCallbackProcedure(d);

	return AltxDVB_OK; // 0x0 OK, non zero quits AltDVB
}

static Polarisation CurrPol = BDA_POLARISATION_NOT_SET;
static clock_t lock_time=0; 
static struct TUNE_DATA tune_data;

int CDvbDeviceControl::Tune(struct TUNE_DATA *d)
{
	SpectralInversion SpectrInv;
	ModulationType ModType;
	Polarisation Pol;
	BinaryConvolutionCodeRate Fec;
	Pilot S2Pilot;
	RollOff S2RollOff;

	LONG PosOpt=DEFAULT_POS_OPT;
	BYTE DiSEqC_Port=0;
	BOOL bUseTB=FALSE;

	switch(selected_device_type)
	{
	case DVBS:
		{
		/* LNB power on/off */
		if ( (d->polarity == LNBPOWER_ON) || (d->polarity == LNBPOWER_OFF) )
		{
			switch(conf_params.VendorSpecific)
			{
			case TBS_BDA:
				if FAILED(BdaGraph.DVBS_TBS_LNBPower(d->polarity == LNBPOWER_ON))
					return AltxDVB_ERR;
				break;
			case QBOX_BDA:
				if FAILED(BdaGraph.DVBS_QBOX_LNBPower(d->polarity == LNBPOWER_ON))
					return AltxDVB_ERR;
				break;
			case CRAZY_BDA:
				if FAILED(BdaGraph.DVBS_Crazy_LNBPower(d->polarity == LNBPOWER_ON))
					return AltxDVB_ERR;
				break;
			case TH_BDA:
				if FAILED(BdaGraph.DVBS_TH_LNBPower(d->polarity == LNBPOWER_ON))
					return AltxDVB_ERR;
				break;
			case COMPRO_BDA:
				if FAILED(BdaGraph.DVBS_Compro_LNBPower(d->polarity == LNBPOWER_ON))
					return AltxDVB_ERR;
				break;
			case TV_BDA:
			case BST_BDA:
				if FAILED(BdaGraph.DVBS_SetPolarity(d->polarity == LNBPOWER_OFF ? BDA_POLARISATION_NOT_DEFINED : DEFAULT_POLARISATION))
					return AltxDVB_ERR;
				break;
			default:
				if(FAILED(BdaGraph.DVBS_Tune(
					(ULONG)(DEFAULT_LOW_OSCILLATOR),
					(ULONG)(DEFAULT_HIGH_OSCILLATOR),
					(ULONG)(DEFAULT_LNB_SWITCH),
					(ULONG)(DEFAULT_FREQUENCY_S),
					DEFAULT_INVERSION_S,
					DEFAULT_MODULATION_S,
					(LONG)(DEFAULT_SYMBOLRATE_S),
					(d->polarity == LNBPOWER_OFF) ? BDA_POLARISATION_NOT_DEFINED : DEFAULT_POLARISATION,
					DEFAULT_FEC,
					DEFAULT_POS_OPT)))
					return AltxDVB_ERR;
			}
			return AltxDVB_OK;
		}

		ReportMessage("Tune transponder");
		lock_time = clock();
		tune_data = *d;

		/* not first tuning */
		switch(d->inversion)
		{
			case INV_AUTO:	SpectrInv = BDA_SPECTRAL_INVERSION_AUTOMATIC; break;
			case INV_OFF:	SpectrInv = BDA_SPECTRAL_INVERSION_NORMAL; break;
			case INV_ON:	SpectrInv = BDA_SPECTRAL_INVERSION_INVERTED; break;
			default: SpectrInv = BDA_SPECTRAL_INVERSION_NOT_DEFINED;
		};

		switch(d->polarity)
		{
			case LNB_POWER_OFF:		Pol = BDA_POLARISATION_NOT_DEFINED; break;
			case LNB_POWER_ON:		Pol = BDA_POLARISATION_LINEAR_V; break;
			case LINEAR_HORIZONTAL:	Pol = BDA_POLARISATION_LINEAR_H; break;
			case LINEAR_VERTICAL:	Pol = BDA_POLARISATION_LINEAR_V; break;
			default: Pol = BDA_POLARISATION_NOT_SET;
		};

		switch(d->fec)
		{
		case FEC_1_2:
			Fec = BDA_BCC_RATE_1_2; break;
		case FEC_2_3:
			Fec = BDA_BCC_RATE_2_3; break;
		case FEC_3_4:
			Fec = BDA_BCC_RATE_3_4; break;
		case FEC_4_5:
			Fec = BDA_BCC_RATE_4_5; break;
		case FEC_5_6:
			Fec = BDA_BCC_RATE_5_6; break;
		case FEC_6_7:
			Fec = BDA_BCC_RATE_6_7; break;
		case FEC_7_8:
			Fec = BDA_BCC_RATE_7_8; break;
		case FEC_8_9:
			Fec = BDA_BCC_RATE_8_9; break;
		case FEC_9_10:
			Fec = BDA_BCC_RATE_9_10; break;
		default:
			Fec = BDA_BCC_RATE_NOT_SET;
		};

		switch(d->modulation)
		{
			case MOD_AUTO:	ModType = BDA_MOD_NOT_DEFINED; break;
			case MOD_QPSK:	ModType = BDA_MOD_QPSK; break;
			case MOD_8PSK:	ModType = conf_params.ConfMod8PSK; break;
			case MOD_QPSK2:	ModType = BDA_MOD_NBC_QPSK; break;
			case MOD_QAM_16: ModType = BDA_MOD_16QAM; break;
			case MOD_QAM_32: ModType = BDA_MOD_32QAM; break;
			case MOD_QAM_64: ModType = BDA_MOD_64QAM; break;
			case MOD_QAM_128: ModType = BDA_MOD_128QAM; break;
			case MOD_VSB_8: ModType = BDA_MOD_8VSB; break;
			case MOD_VSB_16: ModType = BDA_MOD_16VSB; break;
			case MOD_16APSK:	ModType = BDA_MOD_16APSK; break;
			case MOD_32APSK:	ModType = BDA_MOD_32APSK; break;
			default: ModType = BDA_MOD_NOT_SET;
		};

		switch(d->pilot)
		{
			case PILOT_OFF:	S2Pilot = BDA_PILOT_OFF; break;
			case PILOT_ON:	S2Pilot = BDA_PILOT_ON; break;
			default: S2Pilot = BDA_PILOT_NOT_SET;
		};

		switch(d->rolloff)
		{
			case ROLLOFF_20:	S2RollOff = BDA_ROLL_OFF_20; break;
			case ROLLOFF_25:	S2RollOff = BDA_ROLL_OFF_25; break;
			case ROLLOFF_35:	S2RollOff = BDA_ROLL_OFF_35; break;
			default: S2RollOff = BDA_ROLL_OFF_NOT_SET;
		};

		switch(d->switches)
		{
		case TONEBURST_A:
		case TONEBURST_B:
			bUseTB=TRUE; DiSEqC_Port=d->switches; break;
		case POS_A_OPT_A:
			PosOpt=0x000; DiSEqC_Port = 1; break;
		case POS_B_OPT_A:
			PosOpt=0x001; DiSEqC_Port = 2; break;
		case POS_A_OPT_B:
			PosOpt=0x100; DiSEqC_Port = 3; break;
		case POS_B_OPT_B:
			PosOpt=0x101; DiSEqC_Port = 4; break;
		}

		if (bUseTB)
		{
			switch(conf_params.VendorSpecific)
			{
			case TT_BDA:
				BdaGraph.DVBS_TT_DiSEqC(0,NULL,DiSEqC_Port);
				break;
			case TH_BDA:
				BdaGraph.DVBS_TH_LNBSource(DiSEqC_Port, DiSEqC_Port==TONEBURST_A ? Tone_Burst_OFF : Tone_Burst_ON);
				break;
			case OMC_BDA:
				BdaGraph.DVBS_OMC2_ToneBurst(DiSEqC_Port==TONEBURST_B);
				break;
			case ANYSEE_BDA:
				BdaGraph.DVBS_AnySee_DiSEqC(0,NULL,DiSEqC_Port);
				break;
			case GNP_BDA:
				{
					BYTE cmd = DiSEqC_Port==TONEBURST_A ? 1 : 0;
					BdaGraph.DVBS_GenpixOld_DiSEqC(1,&cmd);
				}
				break;
			case GENPIX_BDA:
				BdaGraph.DVBS_Genpix_ToneBurst(DiSEqC_Port==TONEBURST_A ? Tone_Burst_OFF : Tone_Burst_ON);
				break;
			case CRAZY_BDA:
				BdaGraph.DVBS_Crazy_ToneBurst(DiSEqC_Port==TONEBURST_A ? Tone_Burst_OFF : Tone_Burst_ON);
				break;
			}
		}
		else if (DiSEqC_Port)
			switch(conf_params.VendorSpecific)
			{
			case TH_BDA:
				BdaGraph.DVBS_TH_LNBSource(DiSEqC_Port,Data_Burst_ON);
				break;
			}

		int pls_mode = 0, pls_code = 1;

		switch (d->pls_mode)
		{
		case PLS_ROOT:
			pls_mode = 0; pls_code = d->pls_code; break;
		case PLS_GOLD:
			pls_mode = 1; pls_code = d->pls_code; break;
		}
		int isi = d->isi & 0x100 ? d->isi & 0xff : 0;
		
		switch(conf_params.VendorSpecific)
		{
		case MS_BDA:
			if(FAILED(BdaGraph.DVBS_MS_Tune(
					(ULONG)(d->lnb_low),
					(ULONG)(d->lnb_high),
					(ULONG)(d->lnb_switch),
					(ULONG)(d->frequency),
					SpectrInv,
					ModType,
					(LONG)(d->symbol_rate),
					Pol,
					Fec,
					S2Pilot,
					S2RollOff,
					DiSEqC_Port,
					bUseTB)))
				return AltxDVB_ERR;
			break;
		case OMC_BDA:
			if(FAILED(BdaGraph.DVBS_OMC_Tune(
					(ULONG)(d->lnb_low),
					(ULONG)(d->lnb_high),
					(ULONG)(d->lnb_switch),
					(ULONG)(d->frequency),
					SpectrInv,
					ModType,
					(LONG)(d->symbol_rate),
					Pol,
					Fec,
					isi, pls_mode, pls_code)))
				return AltxDVB_ERR;
			break;
		case CRAZY_BDA:
			if(FAILED(BdaGraph.DVBS_Crazy_Tune(
					(ULONG)(d->lnb_low),
					(ULONG)(d->lnb_high),
					(ULONG)(d->lnb_switch),
					(ULONG)(d->frequency),
					SpectrInv,
					ModType,
					(LONG)(d->symbol_rate),
					Pol,
					Fec,
					isi, pls_mode, pls_code)))
				return AltxDVB_ERR;
			break;
		case HAUP_BDA:
			if(FAILED(BdaGraph.DVBS_HAUP_Tune(
					(ULONG)(d->lnb_low),
					(ULONG)(d->lnb_high),
					(ULONG)(d->lnb_switch),
					(ULONG)(d->frequency),
					SpectrInv,
					ModType,
					(LONG)(d->symbol_rate),
					Pol,
					Fec,
					S2Pilot,
					S2RollOff,
					PosOpt)))
				return AltxDVB_ERR;
			break;
		case CXT_BDA:
			if(FAILED(BdaGraph.DVBS_CXT_Tune(
					(ULONG)(d->lnb_low),
					(ULONG)(d->lnb_high),
					(ULONG)(d->lnb_switch),
					(ULONG)(d->frequency),
					SpectrInv,
					ModType,
					(LONG)(d->symbol_rate),
					Pol,
					Fec,
					S2Pilot,
					S2RollOff,
					PosOpt)))
				return AltxDVB_ERR;
			break;
		case DW_BDA:
			if(FAILED(BdaGraph.DVBS_DW_Tune(
				(ULONG)(d->lnb_low),
				(ULONG)(d->lnb_high),
				(ULONG)(d->lnb_switch),
				(ULONG)(d->frequency),
				SpectrInv,
				ModType,
				(LONG)(d->symbol_rate),
				Pol,
				Fec,
				DiSEqC_Port,
				d->switches==TONEBURST_B)))
				return AltxDVB_ERR;
			break;
		case TBS_BDA:
		case QBOX_BDA:
		case TBS_NXP_BDA:
			BdaGraph.DVBS_TBS_SetMIS(isi);
			BdaGraph.DVBS_TBS_SetPLS(pls_mode, pls_code);
		default:
			{
				int rc = FALSE;
				return SUCCEEDED(BdaGraph.DVBS_Tune(
					(ULONG)(d->lnb_low),
					(ULONG)(d->lnb_high),
					(ULONG)(d->lnb_switch),
					(ULONG)(d->frequency),
					SpectrInv,
					ModType,
					(LONG)(d->symbol_rate),
					Pol,
					Fec,
					PosOpt)) ? AltxDVB_OK : AltxDVB_ERR;
			}
		}
		break;
		}
	case DVBT:
		if(FAILED(BdaGraph.DVBT_Tune(
				(ULONG)(d->frequency),
				(ULONG)(d->bandwidth))))
			return AltxDVB_ERR;
		break;
	case DVBC:
		switch(d->modulation)
		{
			case MOD_AUTO:		ModType = BDA_MOD_NOT_SET; break;
			case MOD_QAM_16:	ModType = BDA_MOD_16QAM; break;
			case MOD_QAM_32:	ModType = BDA_MOD_32QAM; break;
			case MOD_QAM_64:	ModType = BDA_MOD_64QAM; break;
			case MOD_QAM_128:	ModType = BDA_MOD_128QAM; break;
			case MOD_QAM_256:	ModType = BDA_MOD_256QAM; break;
			default: ModType = BDA_MOD_NOT_SET;
		};
		if(FAILED(BdaGraph.DVBC_Tune(
				(ULONG)(d->frequency),
				(LONG)(d->symbol_rate),
				ModType)))
			return AltxDVB_ERR;
		break;
	}
	return AltxDVB_OK;
}

int CDvbDeviceControl::SignalStatistics(struct SIGNAL_STATISTICS_DATA *d)
{
	HRESULT hr = S_OK;
	BOOLEAN present=FALSE,locked=FALSE;
	LONG strength=0, quality=0;

	d->quality = 0;
	d->strength = 0;
	d->flags = 0x0;

	switch(conf_params.VendorSpecific)
	{
	default:
		hr = BdaGraph.DVBS_GetSignalsStat(&present,&locked,&strength,&quality);
	}

	if(SUCCEEDED(hr))
	{
		if (conf_params.RelockTimeout && lock_time && (!locked || !quality))
		{
			clock_t curr_time = clock();			
			if ((curr_time-lock_time) >= (conf_params.RelockTimeout*CLOCKS_PER_SEC))
			{
				curr_time = curr_time-lock_time;
				DebugLog( "Relock attempt after %d sec - %s", curr_time/CLOCKS_PER_SEC,
							  Tune(&tune_data) ? "failed !" : "success." );
			}
			else
				DebugLog("Wait signal lock %d sec", (curr_time-lock_time)/CLOCKS_PER_SEC);
		}
		else
			lock_time = clock();

		d->flags = (locked? 0x10 : 0x00) | (present? 0x0F : 0x00);
		d->strength = present ? (int)strength : 0;
		d->quality = locked ? (int)quality : 0;
		return AltxDVB_OK;
	}

	return AltxDVB_ERR;
}

int CDvbDeviceControl::DiSEqC_Command(struct DISEQC_COMMAND_DATA *d)
{
	char txt[256];
	sprintf(txt,"DiSEqC len=%d",d->len);
	DebugLog(txt);
	for(int i=0;i<d->len;++i)
	{
	sprintf(txt,"DiSEqC %2.2x",d->DiSEqC_Command[i]);
	DebugLog(txt);
	}
	switch(conf_params.VendorSpecific)
	{
	case PURE_BDA: return AltxDVB_ERR;
	case MS_BDA:
		if(BdaGraph.DVBS_MS_DiSEqC(d->len, d->DiSEqC_Command,0)!=S_OK)
			return AltxDVB_ERR;
		break;
	case TT_BDA:
		if(BdaGraph.DVBS_TT_DiSEqC(d->len, d->DiSEqC_Command,0)!=S_OK)
			return AltxDVB_ERR;
		break;
	case HAUP_BDA:
		if(BdaGraph.DVBS_HAUP_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case CXT_BDA:
		if (BdaGraph.DVBS_CXT_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case BST_BDA:
		if (BdaGraph.DVBS_BST_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case TBS_BDA:
		if (BdaGraph.DVBS_TBS_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case TBS_NXP_BDA:
		if (BdaGraph.DVBS_TBS_NXP_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case QBOX_BDA:
		if (BdaGraph.DVBS_QBOX_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case TV_BDA:
		if (BdaGraph.DVBS_TeVii_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case TH_BDA:
		if (BdaGraph.DVBS_TH_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case DW_BDA:
		if (BdaGraph.DVBS_DW_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case OMC_BDA:
		//BdaGraph.DVBS_OMC2_Set22Khz(TRUE); Sleep(50);
		if (BdaGraph.DVBS_OMC2_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case COMPRO_BDA:
		BdaGraph.DVBS_Compro_Set22Khz(TRUE);
		Sleep(50);
		if (BdaGraph.DVBS_Compro_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case ANYSEE_BDA:
		if (BdaGraph.DVBS_AnySee_DiSEqC(d->len, d->DiSEqC_Command,0)!=S_OK)
			return AltxDVB_ERR;
		break;
	case GNP_BDA:
		if (BdaGraph.DVBS_GenpixOld_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case GENPIX_BDA:
		if (BdaGraph.DVBS_Genpix_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case NETUP_BDA:
		if (BdaGraph.DVBS_Netup_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	case CRAZY_BDA:
		if (BdaGraph.DVBS_Crazy_DiSEqC(d->len, d->DiSEqC_Command)!=S_OK)
			return AltxDVB_ERR;
		break;
	}
	return AltxDVB_OK;
}

int CDvbDeviceControl::DiSEqC_Support(struct DISEQC_SUPPORT_DATA *d)
{
	d->DiSEqC_Support = (enum DISEQC_TYPE)conf_params.ConfDiSEqC;

	return AltxDVB_OK;
}

int CDvbDeviceControl::DeviceConfig(void)
{
	if(DoConfigurationDialog())
		return AltxDVB_OK;
	else
		return AltxDVB_ERR;
}

int CDvbDeviceControl::DeviceOpensWholeTransponder(struct WHOLE_TRANSPONDER_DATA *d)
{
	d->OpensWholeTransponder = 0x1; // 0x1 - device can open whle transponder, 0x0 - no it cannot
	return AltxDVB_OK;
}
