// DialogGTDSet.cpp : implementation file
//

#include "stdafx.h"
#include "XMSApi_Test.h"
#include "DialogGTDSet.h"
#include "DJAcsDataDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MAX_PARAMETER_TYPE 3
int iComboEnvelopParam = 0;
char ParamEnvelopStr[MAX_PARAMETER_TYPE][32]=
{
	"Mode_Any",
	"Mode_One",
	"Mode_Two"
};

extern CmdParamData_GtdFreq_t      g_GtdFreq;
extern CmdParamData_GtdProtoType_t g_GtdProto;
//------GTD set_freq-------
extern char cfg_freq_index1[20];
extern char cfg_freq_coef1[20];
extern char cfg_freq_index2[20];
extern char cfg_freq_coef2[20];
extern char cfg_freq_index3[20];
extern char cfg_freq_coef3[20];
extern char cfg_freq_index4[20];
extern char cfg_freq_coef4[20];

//------GTD Set Tone-------
extern char cfg_gtdid1[20];
extern char cfg_freqmask1[20];
extern char cfg_threshold1[20];
extern char cfg_envelop1[20];
extern char cfg_repeatcount1[20];
extern char cfg_minontime11[20];
extern char cfg_maxontime11[20];
extern char cfg_minofftime11[20];
extern char cfg_maxofftime11[20];
extern char cfg_minontime21[20];
extern char cfg_maxontime21[20];
extern char cfg_minofftime21[20];
extern char cfg_maxofftime21[20];

extern char cfg_gtdid2[20];
extern char cfg_freqmask2[20];
extern char cfg_threshold2[20];
extern char cfg_envelop2[20];
extern char cfg_repeatcount2[20];
extern char cfg_minontime12[20];
extern char cfg_maxontime12[20];
extern char cfg_minofftime12[20];
extern char cfg_maxofftime12[20];
extern char cfg_minontime22[20];
extern char cfg_maxontime22[20];
extern char cfg_minofftime22[20];
extern char cfg_maxofftime22[20];

extern char cfg_gtdid3[20];
extern char cfg_freqmask3[20];
extern char cfg_threshold3[20];
extern char cfg_envelop3[20];
extern char cfg_repeatcount3[20];
extern char cfg_minontime13[20];
extern char cfg_maxontime13[20];
extern char cfg_minofftime13[20];
extern char cfg_maxofftime13[20];
extern char cfg_minontime23[20];
extern char cfg_maxontime23[20];
extern char cfg_minofftime23[20];
extern char cfg_maxofftime23[20];

extern char cfg_gtdid4[20];
extern char cfg_freqmask4[20];
extern char cfg_threshold4[20];
extern char cfg_envelop4[20];
extern char cfg_repeatcount4[20];
extern char cfg_minontime14[20];
extern char cfg_maxontime14[20];
extern char cfg_minofftime14[20];
extern char cfg_maxofftime14[20];
extern char cfg_minontime24[20];
extern char cfg_maxontime24[20];
extern char cfg_minofftime24[20];
extern char cfg_maxofftime24[20];

extern char cfg_gtdid5[20];
extern char cfg_freqmask5[20];
extern char cfg_threshold5[20];
extern char cfg_envelop5[20];
extern char cfg_repeatcount5[20];
extern char cfg_minontime15[20];
extern char cfg_maxontime15[20];
extern char cfg_minofftime15[20];
extern char cfg_maxofftime15[20];
extern char cfg_minontime25[20];
extern char cfg_maxontime25[20];
extern char cfg_minofftime25[20];
extern char cfg_maxofftime25[20];

extern char cfg_gtdid6[20];
extern char cfg_freqmask6[20];
extern char cfg_threshold6[20];
extern char cfg_envelop6[20];
extern char cfg_repeatcount6[20];
extern char cfg_minontime16[20];
extern char cfg_maxontime16[20];
extern char cfg_minofftime16[20];
extern char cfg_maxofftime16[20];
extern char cfg_minontime26[20];
extern char cfg_maxontime26[20];
extern char cfg_minofftime26[20];
extern char cfg_maxofftime26[20];
/////////////////////////////////////////////////////////////////////////////
// CDialogGTDSet dialog



CDialogGTDSet::CDialogGTDSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGTDSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGTDSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialogGTDSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGTDSet)
	DDX_Control(pDX, IDC_COMBO_ENVELOP, m_ComboEnvelop);
	DDX_Control(pDX, IDC_TAB_GTD_TONE, m_tab_gtd_tone);
	DDX_Control(pDX, IDC_TAB_GTD_FREQ, m_tab_gtd_freq);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGTDSet, CDialog)
	//{{AFX_MSG_MAP(CDialogGTDSet)
	ON_NOTIFY(NM_CLICK, IDC_TAB_GTD_FREQ, OnClickTabGtdFreq)
	ON_NOTIFY(NM_CLICK, IDC_TAB_GTD_TONE, OnClickTabGtdTone)
	ON_CBN_SELCHANGE(IDC_COMBO_ENVELOP, OnSelchangeComboEnvelop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGTDSet message handlers

BOOL CDialogGTDSet::OnInitDialog() 
{
	char	TmpStr[256];
	int		iTmpVal;
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tab_gtd_freq.InsertItem( 0, _T("Set Freq1") );
	m_tab_gtd_freq.InsertItem( 1, _T("Set Freq2") );
	m_tab_gtd_freq.InsertItem( 2, _T("Set Freq3") );
	m_tab_gtd_freq.InsertItem( 3, _T("Set Freq4") );

	//default Setup
	//SetDlgItemText(IDC_EDIT_FREQ_INDEX,"0");
	//SetDlgItemText(IDC_EDIT_FREQ_COEF,"450");

	SetDlgItemText(IDC_EDIT_FREQ_INDEX,cfg_freq_index1);
	SetDlgItemText(IDC_EDIT_FREQ_COEF,cfg_freq_coef1);

	m_tab_gtd_tone.InsertItem( 0, _T("Set Dial") );
	m_tab_gtd_tone.InsertItem( 1, _T("Set Ring") );
	m_tab_gtd_tone.InsertItem( 2, _T("Set BusyI") );
	m_tab_gtd_tone.InsertItem( 3, _T("Set BusyJ") );
	m_tab_gtd_tone.InsertItem( 4, _T("Set BusyK") );
	m_tab_gtd_tone.InsertItem( 5, _T("Set FaxTone") );

	// init m_ComboParam
	for ( int i = 0; i < MAX_PARAMETER_TYPE ; i ++ )
	{
		sprintf ( TmpStr, "%2d - %s", i, ParamEnvelopStr[i] );
		m_ComboEnvelop.AddString ( TmpStr );
	}
	//iComboEnveopParam = cfg_iComboParam;
	iComboEnvelopParam = 0;
	m_ComboEnvelop.SetCurSel ( iComboEnvelopParam );


	//Init the SetTone Parameter
	SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid1);
	SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask1);
	SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold1);
	
	sscanf(cfg_envelop1,"%d",&iTmpVal);
	m_ComboEnvelop.SetCurSel ( iTmpVal);

	SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount1);
	SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime11);
	SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime11);
	SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime11);
	SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime11);
	SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime21);
	SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime21);
	SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime21);
	SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime21);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogGTDSet::OnClickTabGtdFreq(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	char TmpStr[20];
	int iTmpVal;

	int id = m_tab_gtd_freq.GetCurSel();
	//IDC_EDIT_FREQ_INDEX
	sprintf(TmpStr,"%d",id);

	SetDlgItemText(IDC_EDIT_FREQ_INDEX,TmpStr);
	switch(id)
	{
	case 0:
		SetDlgItemText(IDC_EDIT_FREQ_COEF,cfg_freq_coef1);
		break;
	case 1:
		SetDlgItemText(IDC_EDIT_FREQ_COEF,cfg_freq_coef2);
		break;
	case 2:
		SetDlgItemText(IDC_EDIT_FREQ_COEF,cfg_freq_coef3);
		break;
	case 3:
		SetDlgItemText(IDC_EDIT_FREQ_COEF,cfg_freq_coef4);
		break;

	}

	GetDlgItem ( IDC_EDIT_FREQ_INDEX )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );

	GetDlgItem ( IDC_EDIT_FREQ_COEF )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &iTmpVal );
	//g_FskSet.rxFilterWindow = iTmpVal;

	*pResult = 0;
}

void CDialogGTDSet::OnOK() 
{
	// TODO: Add extra validation here

	char TmpStr[20];
	//int	 iTmpVal;

	//Get GTDFreq Parameters
	


	int id = m_tab_gtd_freq.GetCurSel();

	switch(id)
	{
	case 0:
		GetDlgItem ( IDC_EDIT_FREQ_INDEX )->GetWindowText ( cfg_freq_index1, 20 );
		GetDlgItem ( IDC_EDIT_FREQ_COEF )->GetWindowText ( cfg_freq_coef1, 20 );
		sscanf ( cfg_freq_index1, "%d", &g_GtdFreq.m_u16Freq_Index );
		sscanf ( cfg_freq_coef1, "%d",  &g_GtdFreq.m_u16Freq_Coef );
		break;
	case 1:
		GetDlgItem ( IDC_EDIT_FREQ_INDEX )->GetWindowText ( cfg_freq_index2, 20 );
		GetDlgItem ( IDC_EDIT_FREQ_COEF )->GetWindowText ( cfg_freq_coef2, 20 );
		sscanf ( cfg_freq_index2, "%d", &g_GtdFreq.m_u16Freq_Index );
		sscanf ( cfg_freq_coef2, "%d",  &g_GtdFreq.m_u16Freq_Coef );
		break;
	case 2:
		GetDlgItem ( IDC_EDIT_FREQ_INDEX )->GetWindowText ( cfg_freq_index3, 20 );
		GetDlgItem ( IDC_EDIT_FREQ_COEF )->GetWindowText ( cfg_freq_coef3, 20 );
		sscanf ( cfg_freq_index3, "%d", &g_GtdFreq.m_u16Freq_Index );
		sscanf ( cfg_freq_coef3, "%d",  &g_GtdFreq.m_u16Freq_Coef );
		break;
	case 3:
		GetDlgItem ( IDC_EDIT_FREQ_INDEX )->GetWindowText ( cfg_freq_index4, 20 );
		GetDlgItem ( IDC_EDIT_FREQ_COEF )->GetWindowText ( cfg_freq_coef4, 20 );
		sscanf ( cfg_freq_index4, "%d", &g_GtdFreq.m_u16Freq_Index );
		sscanf ( cfg_freq_coef4, "%d",  &g_GtdFreq.m_u16Freq_Coef );
		break;

	}
	



	//Get GTD_Tone Parameters
	GetDlgItem ( IDC_EDIT_GTDID )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16GtdID );

	GetDlgItem ( IDC_EDIT_FREQ_MASK )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Freq_Mask );

	GetDlgItem ( IDC_EDIT_THRESHOLD )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Amp_Threshold );

	g_GtdProto.m_u16Envelope_Mode = m_ComboEnvelop.GetCurSel();

	GetDlgItem ( IDC_EDIT_REPEATCOUNT )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Repeat_Count );

	GetDlgItem ( IDC_EDIT_MINONTIME1 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Min_On_Time1 );

	GetDlgItem ( IDC_EDIT_MAXONTIME1 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Max_On_Time1 );

	GetDlgItem ( IDC_EDIT_MINOFFTIME1 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Min_Off_Time1 );

	GetDlgItem ( IDC_EDIT_MAXOFFTIME1 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Max_Off_Time1 );

	GetDlgItem ( IDC_EDIT_MINONTIME2 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Min_On_Time2 );

	GetDlgItem ( IDC_EDIT_MAXONTIME2 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Max_On_Time2 );

	GetDlgItem ( IDC_EDIT_MINOFFTIME2 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Min_Off_Time2 );

	GetDlgItem ( IDC_EDIT_MAXOFFTIME2 )->GetWindowText ( TmpStr, 20 );
	sscanf ( TmpStr, "%d", &g_GtdProto.m_u16Max_Off_Time2 );


	id = m_tab_gtd_tone.GetCurSel();

	switch(id)
	{
	case 0:
		sprintf(cfg_gtdid1,      "%d",g_GtdProto.m_u16GtdID);
		sprintf(cfg_freqmask1,   "%d",g_GtdProto.m_u16Freq_Mask);
		sprintf(cfg_threshold1,  "%d",g_GtdProto.m_u16Amp_Threshold);
		sprintf(cfg_envelop1,    "%d",g_GtdProto.m_u16Envelope_Mode);
		sprintf(cfg_repeatcount1,"%d",g_GtdProto.m_u16Repeat_Count);
		sprintf(cfg_minontime11, "%d",g_GtdProto.m_u16Min_On_Time1);
		sprintf(cfg_maxontime11, "%d",g_GtdProto.m_u16Max_On_Time1);
		sprintf(cfg_minofftime11,"%d",g_GtdProto.m_u16Min_Off_Time1);
		sprintf(cfg_maxofftime11,"%d",g_GtdProto.m_u16Max_Off_Time1);
		sprintf(cfg_minontime21, "%d",g_GtdProto.m_u16Min_On_Time2);
		sprintf(cfg_maxontime21, "%d",g_GtdProto.m_u16Max_On_Time2);
		sprintf(cfg_minofftime21,"%d",g_GtdProto.m_u16Min_Off_Time2);
		sprintf(cfg_maxofftime21,"%d",g_GtdProto.m_u16Max_Off_Time2);
		break;
	case 1:
		sprintf(cfg_gtdid2,      "%d",g_GtdProto.m_u16GtdID);
		sprintf(cfg_freqmask2,   "%d",g_GtdProto.m_u16Freq_Mask);
		sprintf(cfg_threshold2,  "%d",g_GtdProto.m_u16Amp_Threshold);
		sprintf(cfg_envelop2,    "%d",g_GtdProto.m_u16Envelope_Mode);
		sprintf(cfg_repeatcount2,"%d",g_GtdProto.m_u16Repeat_Count);
		sprintf(cfg_minontime12, "%d",g_GtdProto.m_u16Min_On_Time1);
		sprintf(cfg_maxontime12, "%d",g_GtdProto.m_u16Max_On_Time1);
		sprintf(cfg_minofftime12,"%d",g_GtdProto.m_u16Min_Off_Time1);
		sprintf(cfg_maxofftime12,"%d",g_GtdProto.m_u16Max_Off_Time1);//from 11 t0 12
		sprintf(cfg_minontime22, "%d",g_GtdProto.m_u16Min_On_Time2);
		sprintf(cfg_maxontime22, "%d",g_GtdProto.m_u16Max_On_Time2);
		sprintf(cfg_minofftime22,"%d",g_GtdProto.m_u16Min_Off_Time2);
		sprintf(cfg_maxofftime22,"%d",g_GtdProto.m_u16Max_Off_Time2);
		break;
	case 2:
		sprintf(cfg_gtdid3,      "%d",g_GtdProto.m_u16GtdID);
		sprintf(cfg_freqmask3,   "%d",g_GtdProto.m_u16Freq_Mask);
		sprintf(cfg_threshold3,  "%d",g_GtdProto.m_u16Amp_Threshold);
		sprintf(cfg_envelop3,    "%d",g_GtdProto.m_u16Envelope_Mode);
		sprintf(cfg_repeatcount3,"%d",g_GtdProto.m_u16Repeat_Count);
		sprintf(cfg_minontime13, "%d",g_GtdProto.m_u16Min_On_Time1);
		sprintf(cfg_maxontime13, "%d",g_GtdProto.m_u16Max_On_Time1);
		sprintf(cfg_minofftime13,"%d",g_GtdProto.m_u16Min_Off_Time1);
		sprintf(cfg_maxofftime13,"%d",g_GtdProto.m_u16Max_Off_Time1);
		sprintf(cfg_minontime23, "%d",g_GtdProto.m_u16Min_On_Time2);
		sprintf(cfg_maxontime23, "%d",g_GtdProto.m_u16Max_On_Time2);
		sprintf(cfg_minofftime23,"%d",g_GtdProto.m_u16Min_Off_Time2);
		sprintf(cfg_maxofftime23,"%d",g_GtdProto.m_u16Max_Off_Time2);
		break;
	case 3:
		sprintf(cfg_gtdid4,      "%d",g_GtdProto.m_u16GtdID);
		sprintf(cfg_freqmask4,   "%d",g_GtdProto.m_u16Freq_Mask);
		sprintf(cfg_threshold4,  "%d",g_GtdProto.m_u16Amp_Threshold);
		sprintf(cfg_envelop4,    "%d",g_GtdProto.m_u16Envelope_Mode);
		sprintf(cfg_repeatcount4,"%d",g_GtdProto.m_u16Repeat_Count);
		sprintf(cfg_minontime14, "%d",g_GtdProto.m_u16Min_On_Time1);
		sprintf(cfg_maxontime14, "%d",g_GtdProto.m_u16Max_On_Time1);
		sprintf(cfg_minofftime14,"%d",g_GtdProto.m_u16Min_Off_Time1);
		sprintf(cfg_maxofftime14,"%d",g_GtdProto.m_u16Max_Off_Time1);
		sprintf(cfg_minontime24, "%d",g_GtdProto.m_u16Min_On_Time2);
		sprintf(cfg_maxontime24, "%d",g_GtdProto.m_u16Max_On_Time2);
		sprintf(cfg_minofftime24,"%d",g_GtdProto.m_u16Min_Off_Time2);
		sprintf(cfg_maxofftime24,"%d",g_GtdProto.m_u16Max_Off_Time2);
		break;
	case 4:
		sprintf(cfg_gtdid5,      "%d",g_GtdProto.m_u16GtdID);
		sprintf(cfg_freqmask5,   "%d",g_GtdProto.m_u16Freq_Mask);
		sprintf(cfg_threshold5,  "%d",g_GtdProto.m_u16Amp_Threshold);
		sprintf(cfg_envelop5,    "%d",g_GtdProto.m_u16Envelope_Mode);
		sprintf(cfg_repeatcount5,"%d",g_GtdProto.m_u16Repeat_Count);
		sprintf(cfg_minontime15, "%d",g_GtdProto.m_u16Min_On_Time1);
		sprintf(cfg_maxontime15, "%d",g_GtdProto.m_u16Max_On_Time1);
		sprintf(cfg_minofftime15,"%d",g_GtdProto.m_u16Min_Off_Time1);
		sprintf(cfg_maxofftime15,"%d",g_GtdProto.m_u16Max_Off_Time1);
		sprintf(cfg_minontime25, "%d",g_GtdProto.m_u16Min_On_Time2);
		sprintf(cfg_maxontime25, "%d",g_GtdProto.m_u16Max_On_Time2);
		sprintf(cfg_minofftime25,"%d",g_GtdProto.m_u16Min_Off_Time2);
		sprintf(cfg_maxofftime25,"%d",g_GtdProto.m_u16Max_Off_Time2);
		break;
	case 5:
		sprintf(cfg_gtdid6,      "%d",g_GtdProto.m_u16GtdID);
		sprintf(cfg_freqmask6,   "%d",g_GtdProto.m_u16Freq_Mask);
		sprintf(cfg_threshold6,  "%d",g_GtdProto.m_u16Amp_Threshold);
		sprintf(cfg_envelop6,    "%d",g_GtdProto.m_u16Envelope_Mode);
		sprintf(cfg_repeatcount6,"%d",g_GtdProto.m_u16Repeat_Count);
		sprintf(cfg_minontime16, "%d",g_GtdProto.m_u16Min_On_Time1);
		sprintf(cfg_maxontime16, "%d",g_GtdProto.m_u16Max_On_Time1);
		sprintf(cfg_minofftime16,"%d",g_GtdProto.m_u16Min_Off_Time1);
		sprintf(cfg_maxofftime16,"%d",g_GtdProto.m_u16Max_Off_Time1);
		sprintf(cfg_minontime26, "%d",g_GtdProto.m_u16Min_On_Time2);
		sprintf(cfg_maxontime26, "%d",g_GtdProto.m_u16Max_On_Time2);
		sprintf(cfg_minofftime26,"%d",g_GtdProto.m_u16Min_Off_Time2);
		sprintf(cfg_maxofftime26,"%d",g_GtdProto.m_u16Max_Off_Time2);
		break;
	
	}

	CDialog::OnOK();
}

void CDialogGTDSet::OnClickTabGtdTone(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//char TmpStr[20];
	int iTmpVal;

	int id = m_tab_gtd_tone.GetCurSel();
	//IDC_EDIT_FREQ_INDEX
	//sprintf(TmpStr,"%d",id);

	//SetDlgItemText(IDC_EDIT_FREQ_TONE,TmpStr);
	switch(id)
	{
	case 0:
		SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid1);
		SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask1);
		SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold1);

		sscanf(cfg_envelop1,"%d",&iTmpVal);
		m_ComboEnvelop.SetCurSel ( iTmpVal);

		SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount1);
		SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime11);
		SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime11);
		SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime11);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime11);
		SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime21);
		SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime21);
		SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime21);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime21);

		break;
	case 1:
		SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid2);
		SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask2);
		SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold2);

		sscanf(cfg_envelop2,"%d",&iTmpVal);
		m_ComboEnvelop.SetCurSel ( iTmpVal);

		SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount2);
		SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime12);
		SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime12);
		SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime12);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime12);
		SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime22);
		SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime22);
		SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime22);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime22);
		break;
	case 2:
		SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid3);
		SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask3);
		SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold3);

		sscanf(cfg_envelop3,"%d",&iTmpVal);
		m_ComboEnvelop.SetCurSel ( iTmpVal);

		SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount3);
		SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime13);
		SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime13);
		SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime13);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime13);
		SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime23);
		SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime23);
		SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime23);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime23);
		break;
	case 3:
		SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid4);
		SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask4);
		SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold4);

		sscanf(cfg_envelop4,"%d",&iTmpVal);
		m_ComboEnvelop.SetCurSel ( iTmpVal);

		SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount4);
		SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime14);
		SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime14);
		SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime14);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime14);
		SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime24);
		SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime24);
		SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime24);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime24);
		break;
	case 4:
		SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid5);
		SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask5);
		SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold5);

		sscanf(cfg_envelop5,"%d",&iTmpVal);
		m_ComboEnvelop.SetCurSel ( iTmpVal);

		SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount5);
		SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime15);
		SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime15);
		SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime15);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime15);
		SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime25);
		SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime25);
		SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime25);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime25);
		break;
	case 5:
		SetDlgItemText(IDC_EDIT_GTDID,cfg_gtdid6);
		SetDlgItemText(IDC_EDIT_FREQ_MASK,cfg_freqmask6);
		SetDlgItemText(IDC_EDIT_THRESHOLD,cfg_threshold6);

		sscanf(cfg_envelop6,"%d",&iTmpVal);
		m_ComboEnvelop.SetCurSel ( iTmpVal);

		SetDlgItemText(IDC_EDIT_REPEATCOUNT,cfg_repeatcount6);
		SetDlgItemText(IDC_EDIT_MINONTIME1,cfg_minontime16);
		SetDlgItemText(IDC_EDIT_MAXONTIME1,cfg_maxontime16);
		SetDlgItemText(IDC_EDIT_MINOFFTIME1,cfg_minofftime16);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME1,cfg_maxofftime16);
		SetDlgItemText(IDC_EDIT_MINONTIME2,cfg_minontime26);
		SetDlgItemText(IDC_EDIT_MAXONTIME2,cfg_maxontime26);
		SetDlgItemText(IDC_EDIT_MINOFFTIME2,cfg_minofftime26);
		SetDlgItemText(IDC_EDIT_MAXOFFTIME2,cfg_maxofftime26);
		break;

	}

	*pResult = 0;
}

void CDialogGTDSet::OnSelchangeComboEnvelop() 
{
	// TODO: Add your control notification handler code here
	iComboEnvelopParam = m_ComboEnvelop.GetCurSel ( ); 
}
