// MyDlgBar.cpp : implementation file
//

#include "stdafx.h"
#include "KGEventView.h"
#include "MyDlgBar.h"
#include "commontool.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CHECK_INFO  m_CheckInfo;
/////////////////////////////////////////////////////////////////////////////
// CMyDlgBar dialog

CMyDlgBar::CMyDlgBar()
{
	//{{AFX_DATA_INIT(CMyDlgBar)
	//}}AFX_DATA_INIT
}

void CMyDlgBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDlgBar)
//		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_COMBO_APPID, m_appid);
	DDX_Control(pDX, IDC_COMBO_API, m_api);
 	DDX_Control(pDX, IDC_COMBO_EVT, m_evt);
	DDX_Control(pDX, IDC_COMBO_DEVTYPE, m_devtype);
	DDX_Control(pDX, IDC_COMBO_DSPID, m_dspid);
 	DDX_Control(pDX, IDC_COMBO_CHNLID, m_chnlid);

	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMyDlgBar, CDialogBar)
	//{{AFX_MSG_MAP(CMyDlgBar)
		// NOTE: the ClassWizard will add message map macros here
	ON_BN_CLICKED(IDC_BUTTON_SELECT,OnBtnClk)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_INITDIALOG, OnInitDialog )
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CMyDlgBar, CDialogBar)
	//{{AFX_DISPATCH_MAP(CMyDlgBar)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMyDlgBar to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D21A3A6B-3B99-4414-9BF0-238E4098C45D}
static const IID IID_IMyDlgBar =
{ 0xd21a3a6b, 0x3b99, 0x4414, { 0x9b, 0xf0, 0x23, 0x8e, 0x40, 0x98, 0xc4, 0x5d } };

BEGIN_INTERFACE_MAP(CMyDlgBar, CDialogBar)
	INTERFACE_PART(CMyDlgBar, IID_IMyDlgBar, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDlgBar message handlers
LONG CMyDlgBar::OnInitDialog ( UINT wParam, LONG lParam)
{

            BOOL bRet = HandleInitDialog(wParam, lParam);

            if (!UpdateData(FALSE))
            {
               TRACE0("Warning: UpdateData failed during dialog init.\n");
            }
            initdevtype();
            initapi();
            initevt();
			initdspid();
			initchnlid();
			InitSelectInfo();
            return bRet;
}


BOOL CMyDlgBar::Create(CWnd* pParent, UINT nIDTemplate, UINT nStyle, UINT nID) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	BOOL bReturn = CDialogBar::Create(pParent, nIDTemplate, nStyle, nID);
	if(bReturn)
	{
		m_subButton.AutoLoad(IDC_BUTTON_SELECT, this);
	}
	return bReturn;
	//return true;
}

void CMyDlgBar::OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHandler)
{
    bDisableIfNoHandler = FALSE;
    CDialogBar::OnUpdateCmdUI(pTarget, FALSE);
}
void CMyDlgBar::initappid()
{

}

void CMyDlgBar::initdevtype()
{
	m_devtype.AddString ( "0x00 - All" );
	m_devtype.AddString ( "0x01 - VOICE" );		
	m_devtype.AddString ( "0x02 - FAX" );			
	m_devtype.AddString ( "0x03 - INTERFACE_CH" );
	m_devtype.AddString ( "0x04 - BOARD" );		
	m_devtype.AddString ( "0x05 - VOIP" );			
	m_devtype.AddString ( "0x06 - CONFERENCE" );
	m_devtype.AddString ( "0x07 - CTX" );

	m_devtype.SetCheck(0, TRUE);
    m_devtype.SelectAll(true);
	m_devtype.SetDroppedWidth(m_devtype.GetDroppedWidth()+30);
	m_CheckInfo.iDEVcheckNum=7;
}
void CMyDlgBar::initapi()
{
	m_api.AddString ( "000 - All" );
	m_api.AddString("001 -   XMS_acs_GetDevList");         
	m_api.AddString("002 -   XMS_acs_OpenDevice");         
	m_api.AddString("003 -   XMS_acs_CloseDevice");        
	m_api.AddString("004 -   XMS_acs_RestDevice");        
	m_api.AddString("005 -   XMS_acs_GetDeviceState");     
	m_api.AddString("006 -   XMS_cts_SetDevGroup");	      
	m_api.AddString("007 -   XMS_cts_SetDevParam");	      
	m_api.AddString("008 -   XMS_cts_GetDevParam");	      
	m_api.AddString("009 -   XMS_cts_MakeCallOut");        
	m_api.AddString("010 -   XMS_cts_AlertCall");          
	
	m_api.AddString("011 -  XMS_cts_AnswerCallIn");	      
	m_api.AddString("012 -  XMS_cts_LinkDev");             
	m_api.AddString("013 -  XMS_cts_UnLinkDev");	          
	m_api.AddString("014 -  XMS_cts_ClearCall");           
	m_api.AddString("015 -  XMS_cts_JoinTOConf");          
	m_api.AddString("016 -  XMS_cts_LeaveFromConf");       
	m_api.AddString("017 -  XMS_cts_ClearConf");           
	m_api.AddString("018 -  XMS_cts_PlayFile");            
	m_api.AddString("019 -  XMS_cts_IinitPlayIndex");       
	m_api.AddString("020 -  XMS_cts_BuildPlayIndex");      
	
	m_api.AddString("021 -  XMS_cts_ControlPlay");         
	m_api.AddString("022 -  XMS_cts_RecordFile");          
	m_api.AddString("023 -  XMS_cts_ControlRecord");       
	m_api.AddString("024 -  XMS_cts_SendFax");             
	m_api.AddString("025 -  XMS_cts_StopSendFax");         
	m_api.AddString("026 -  XMS_cts_RecvFax");             
	m_api.AddString("027 -  XMS_cts_StopRecvFax");         
	m_api.AddString("028 -  XMS_cts_ChangeMonitorFilter");	
	m_api.AddString("029 -  XMS_cts_SendIO");	            
	m_api.AddString("030 -  XMS_cts_SendSigMSG");          
	
	m_api.AddString("031 -  XMS_cts_RecordDCSP");	          
	m_api.AddString("032 -  XMS_cts_DbgON");               
	m_api.AddString("033 -  XMS_acs_DbgOFF");              
	m_api.AddString("034 -  XMS_acs_Play_3GPP");           
	m_api.AddString("035 -  XMS_cts_Control_3GPP_PLAY");   
	m_api.AddString("036 -  XMS_cts_Record_3GPP");         
	m_api.AddString("037 -  XMS_cts_Control_3GPP_Record"); 
	m_api.AddString("038 -  XMS_cts_PlayCSP");             
	m_api.AddString("039 -  XMS_cts_SendPlayCSP");         
	m_api.AddString("040 -  XMS_cts_QUERYLIC");            
	
	m_api.AddString("041 -  XMS_CTX_Reg");              
	m_api.AddString("042 -  XMS_CTX_Link");             
	m_api.AddString("043 -  XMS_CTX_SendAPPData");      
	m_api.AddString("044 -  XMS_cts_Init3GPPIndex");       
    m_api.AddString("045 -  XMS_acs_Build3GPPIndex"); 	

	m_api.SetCheck(0, TRUE);
    m_api.SelectAll(true);
	m_api.SetDroppedWidth(m_api.GetDroppedWidth()+140);
	m_CheckInfo.iAPIcheckNum=45;
}
void CMyDlgBar::initevt()
{
	m_evt.AddString ("0x00 - All");
	m_evt.AddString("0X01- XMS_EVT_OPEN_STREAM");              	
	m_evt.AddString("0X02- XMS_EVT_QUERY_DEVICE");              	
	m_evt.AddString("0X03- XMS_EVT_QUERY_DEVICE_END");              
	m_evt.AddString("0X04- XMS_EVT_OPEN_DEVICE");              
	m_evt.AddString("0X05- XMS_EVT_CLOSE_DEVICE");              
	m_evt.AddString("0X06- XMS_EVT_RESET_DEVICE");              
	m_evt.AddString("0X07- XMS_EVT_DEVICESTATE");              
	m_evt.AddString("0X08- XMS_EVT_SETDEV_GROUP");              
	m_evt.AddString("0X09- XMS_EVT_SETPARAM");              
	m_evt.AddString("0X10- XMS_EVT_GETPARAM");              
	m_evt.AddString("0X11- XMS_EVT_QUERY_ONE_DSP_START");            
	m_evt.AddString("0X12- XMS_EVT_QUERY_ONE_DSP_END");            
	m_evt.AddString("0X13- XMS_EVT_QUERY_REMOVE_ONE_DSP_START");      
	m_evt.AddString("0X14- XMS_EVT_QUERY_REMOVE_ONE_DSP_END");        
	m_evt.AddString("0X15- XMS_EVT_OPEN_STREAM_EXT");                	
	m_evt.AddString("0X16- XMS_EVT_LICENSE_QUERY");                	
	m_evt.AddString("0X17- XMS_EVT_ACS_TIMER");                

	m_evt.AddString("0X18- XMS_EVT_CALLOUT");		
	m_evt.AddString("0X19- XMS_EVT_CALLIN");		
	m_evt.AddString("0X20- XMS_EVT_ALERTCALL");    
	m_evt.AddString("0X21- XMS_EVT_ANSWERCALL");    
	m_evt.AddString("0X22- XMS_EVT_LINKDEVICE");	  
	m_evt.AddString("0X23- XMS_EVT_UNLINKDEVICE");		
	m_evt.AddString("0X24- XMS_EVT_CLEARCALL");		
	m_evt.AddString("0X25- XMS_EVT_ANALOG_INTERFACE");    
	m_evt.AddString("0X26- XMS_EVT_CAS_MFC_START");		
	m_evt.AddString("0X27- XMS_EVT_CAS_MFC_END");		
	m_evt.AddString("0X28- XMS_EVT_SENDSIGMSG");		
	m_evt.AddString("0X29- XMS_EVT_SIGMON");		
	m_evt.AddString("0X30- XMS_EVT_3G324M");		
	m_evt.AddString("0X31- XMS_EVT_CTX_REG");    
	m_evt.AddString("0X32- XMS_EVT_CTX_LINK");    
	m_evt.AddString("0X33- XMS_EVT_CTX_APPDATA");    

	m_evt.AddString("0X34- XMS_EVT_JOINTOCONF");		
	m_evt.AddString("0X35- XMS_EVT_LEAVEFROMCONF");		
	m_evt.AddString("0X36- XMS_EVT_CLEARCONF");		

	m_evt.AddString("0X37- XMS_EVT_PLAY");
	m_evt.AddString("0X38- XMS_EVT_INITINDEX");
	m_evt.AddString("0X39- XMS_EVT_BUILDINDEX");
	m_evt.AddString("0X40- XMS_EVT_CONTROLPLAY");
	m_evt.AddString("0X41- XMS_EVT_RECORD");
	m_evt.AddString("0X42- XMS_EVT_CONTROLRECORD");
	m_evt.AddString("0X43- XMS_EVT_RECORDCSP");
	m_evt.AddString("0X44- XMS_EVT_CONTROLRECORDCSP");
	m_evt.AddString("0X45- XMS_EVT_3GPP_PLAY");
	m_evt.AddString("0X46- XMS_EVT_3GPP_CONTROLPLAY");
	m_evt.AddString("0X47- XMS_EVT_3GPP_RECORD");
	m_evt.AddString("0X48- XMS_EVT_3GPP_CONTROLRECORD");
	m_evt.AddString("0X49- XMS_EVT_PLAYCSPREQ");
	m_evt.AddString("0X50- XMS_EVT_3GPP_INIT_INDEX");
	m_evt.AddString("0X51- XMS_EVT_3GPP_BUILD_INDEX");

	m_evt.AddString("0X52- XMS_EVT_SENDFAX"); 
	m_evt.AddString("0X53- XMS_EVT_RECVFAX"); 

	m_evt.AddString("0X54- XMS_EVT_SENDIODATA");
	m_evt.AddString("0X55- XMS_EVT_RECVIODATA");

	m_evt.AddString("0X56- XMS_EVT_CHGMONITORFILTER");
	m_evt.AddString("0X57- XMS_EVT_DEV_TIMER");

	m_evt.AddString("0X58- XMS_EVT_UNIFAILURE");

	m_evt.SetCheck(0, TRUE);
    m_evt.SelectAll(true);
    m_CheckInfo.iEVTcheckNum=58;
	m_evt.SetDroppedWidth(m_evt.GetDroppedWidth()+140);
}
void CMyDlgBar::initdspid()
{
    CString     tmp;
	int         i;
	m_dspid.AddString ( "0x000 -All" );
	for (i=1;i<17;i++) 
	{
	    tmp.Format("0x%03d - DSP %03d",i,i);
		m_dspid.AddString((LPCTSTR)tmp);
	}
	m_dspid.SetCheck(0, TRUE);
    m_dspid.SelectAll(true);
	m_CheckInfo.iDSPcheckNum=16;
	m_dspid.SetDroppedWidth(m_dspid.GetDroppedWidth()+70);
}
void CMyDlgBar::initchnlid()
{
    CString     tmp;
	int         i;
	m_chnlid.AddString ( "0x000 -All" );
	for (i=0;i<128;i++) 
	{
	    tmp.Format("0x%03d - Chnl %03d",i+1,i);
		m_chnlid.AddString((LPCTSTR)tmp);
	}
	m_chnlid.SetCheck(0, TRUE);
    m_chnlid.SelectAll(true);
	m_CheckInfo.iCHNLcheckNum=128;
	m_chnlid.SetDroppedWidth(m_chnlid.GetDroppedWidth()+70);
}

void  CMyDlgBar::OnBtnClk()
{
    GetAPISelectInfo();
	GetEVTSelectInfo();
	GetDspSelectInfo();
	GetChnlSelectInfo();
	GetDevSelectInfo();
}

void CMyDlgBar::GetAPISelectInfo()
{
	int  i;
	for(i=0;i<m_CheckInfo.iAPIcheckNum+1;i++)
	{
        m_CheckInfo.API[i].IsCheck=m_api.GetCheck(i);
	}
}

void CMyDlgBar::GetEVTSelectInfo()
{
	int  i;
	for(i=0;i<m_CheckInfo.iEVTcheckNum+1;i++)
	{
        m_CheckInfo.EVT[i].IsCheck=m_evt.GetCheck(i);
	}
}

void CMyDlgBar::GetDevSelectInfo()
{
	int  i;
	for(i=0;i<m_CheckInfo.iDEVcheckNum+1;i++)
	{
        m_CheckInfo.DEV[i].IsCheck=m_devtype.GetCheck(i);
	}
}

void CMyDlgBar::GetDspSelectInfo()
{
	int  i;
	for(i=0;i<m_CheckInfo.iDSPcheckNum+1;i++)
	{
        m_CheckInfo.DSP[i].IsCheck=m_dspid.GetCheck(i);
	}
}

void CMyDlgBar::GetChnlSelectInfo()
{
	int  i;
	for(i=0;i<m_CheckInfo.iCHNLcheckNum+1;i++)
	{
        m_CheckInfo.CHNL[i].IsCheck=m_chnlid.GetCheck(i);
	}
}

void CMyDlgBar::InitSelectInfo()
{
    GetAPISelectInfo();
	GetEVTSelectInfo();
	GetDspSelectInfo();
	GetChnlSelectInfo();
	GetDevSelectInfo();
}
