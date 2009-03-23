#include "stdafx.h"

#include "XMS_SMonitor.h"
#include "XMS_SMonitorDlg.h"

#include "DJAcsDataDef.h"
#include "DJAcsAPIDef.h"
#include "DJAcsDevState.h"
#include "ITPDataDefine.h"

#include "XMS_SMonitorFunc.h"

#include "DJAcsSignalMonitor.h"

/////////////////////////////////// Global define /////////////////////////////
extern CXMS_SMonitorDlg* pdlg;

TYPE_MY_DEVICE_STRUCT	*g_pDevRes = NULL;
CFG_INFO g_CFGInfo;

int      g_TotalDevRes  = 0;
int      XMStest_AppID  = 0;
int      g_StartWorkFlag= 0;
int      g_Pause = 0;

ACSHandle_t  g_acsHandle= 0;
ServerID_t   cfg_ServerID;
//


//*****************************************************************************
//Name        : InitSystem
//Author      : None
//Description : Initialize system, list control display process
//Input       : None
//Output      : None
//Return      : 0 - Success 
//             -1 - Failure
//*****************************************************************************
DJ_S32 InitSystem()
{
	///////////////////// Display information initialize //////////////////////
	pdlg->m_DeviceInfo.SetExtendedStyle(pdlg->m_DeviceInfo.GetExtendedStyle() | 
		                                LVS_EX_FULLROWSELECT
										);
	pdlg->m_DeviceInfo.InsertColumn(0, "CHN_info", LVCFMT_LEFT, 60 );
	pdlg->m_DeviceInfo.InsertColumn(1, "Type",     LVCFMT_LEFT, 120);
	pdlg->m_DeviceInfo.InsertColumn(2, "State/Event",LVCFMT_LEFT,120);
	pdlg->m_DeviceInfo.InsertColumn(3, "Caller",   LVCFMT_LEFT, 80);
	pdlg->m_DeviceInfo.InsertColumn(4, "Called",   LVCFMT_LEFT, 80);	
	pdlg->m_DeviceInfo.InsertColumn(5, "Information",LVCFMT_LEFT, 320);
	//

	pdlg->m_RawInfo.SetExtendedStyle(pdlg->m_DeviceInfo.GetExtendedStyle() | 
		                                LVS_EX_FULLROWSELECT
										);
	
	pdlg->m_RawInfo.InsertColumn(0, "No.",     LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(1, "Unit_ID", LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(2, "Port_ID", LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(3, "TS_ID",   LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(4, "Raw_Len", LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(5, "Raw_Data",LVCFMT_LEFT,480);
	//
	
	pdlg->m_RawLine.AddString("50000");
	pdlg->m_RawLine.AddString("10000");
	pdlg->m_RawLine.AddString("5000");
	pdlg->m_RawLine.AddString("1000");	
	pdlg->m_RawLine.AddString("100");
	pdlg->m_RawLine.SetItemData(0,50000);
	pdlg->m_RawLine.SetItemData(1,10000);
	pdlg->m_RawLine.SetItemData(2,5000);
	pdlg->m_RawLine.SetItemData(3,1000);
	pdlg->m_RawLine.SetItemData(4,100);
	pdlg->m_RawLine.SetCurSel(0);
	//
	
	memset(&g_CFGInfo, 0, sizeof(g_CFGInfo));
	g_TotalDevRes = 0;
	g_pDevRes = NULL;
	g_StartWorkFlag=0;
	
	Get_ConfigInformation();
	//
	
	///////////////////// XMS acs api system initialize ///////////////////////
	memset(&cfg_ServerID, 0, sizeof(&cfg_ServerID));
	strcpy(cfg_ServerID.m_s8ServerIp, 
		   g_CFGInfo.IPAddress
		   );
	
	//
	strcpy(cfg_ServerID.m_s8UserName, g_CFGInfo.Username);
	strcpy(cfg_ServerID.m_s8UserPwd,  g_CFGInfo.Password);
	
	cfg_ServerID.m_u32ServerPort = g_CFGInfo.Port;
	XMStest_AppID = g_CFGInfo.AppID;	
	
	//
	RetCode_t r = XMS_acsOpenStream(&g_acsHandle, 
		                            &cfg_ServerID,
						            XMStest_AppID,
						            32,
						            32,
						            0,
						            NULL
									);
	if (r < 0)
	{
		MessageBox(NULL, "XMS_acsOpenStream failed!", "Error", MB_OK);
		return -1;
	}
	
	//
	r = XMS_acsSetESR(g_acsHandle, (EsrFunc)EVT_Handler, 0, 1);
	if (r < 0)
	{
		MessageBox(NULL, "XMS_acsSetESR failed!", "Error", MB_OK);
		return -1;
	}
	
	//
	XMS_acsGetDeviceList(g_acsHandle, NULL);
	
	return 0;
}


//*****************************************************************************
//Name        : InitSystem
//Author      : None
//Description : Exit system and release resource
//Input       : None
//Output      : None
//Return      : 0 - Success 
//             -1 - Failure
//*****************************************************************************
DJ_S32  ExitSystem(void)
{	
	CloseDevice();
	
	XMS_acsCloseStream(g_acsHandle, NULL);	
	g_acsHandle = 0;
	
	if (g_pDevRes) free(g_pDevRes);
	g_pDevRes = NULL;
	
	return 0;
}


//*****************************************************************************
//Name        : Get_ConfigInformation
//Author      : None
//Description : Read config information from file XMS_mon.ini
//Input       : None
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void Get_ConfigInformation()
{
	GetPrivateProfileString("ConfigInformation", "IPAddress", "127.0.0.1", 
							 g_CFGInfo.IPAddress, 
							 32, 
							 ".\\XMS_mon.ini"
							 );
	
	g_CFGInfo.Port     = GetPrivateProfileInt("ConfigInformation", "Port", 
		                                      0, 
											  ".\\XMS_mon.ini"
											  );
	
	GetPrivateProfileString("ConfigInformation", "Username", "", 
							 g_CFGInfo.Username, 
							 32, 
							 ".\\XMS_mon.ini"
							 );
	
	GetPrivateProfileString("ConfigInformation", "Password", "", 
							 g_CFGInfo.Password, 
							 32, 
							 ".\\XMS_mon.ini"
							 );
	
	g_CFGInfo.AppID    = GetPrivateProfileInt("ConfigInformation", "AppID", 
		                                      0,
											  ".\\XMS_mon.ini"
											  );
	
	pdlg->m_IPAddress.SetWindowText(g_CFGInfo.IPAddress);
	char sztmp[32];
	
	itoa(g_CFGInfo.Port,   sztmp, 10);
	pdlg->m_Port.SetWindowText(sztmp);
	
	pdlg->UpdateData(FALSE);
	
	return;
}


//*****************************************************************************
//Name        : Set_ConfigInformation
//Author      : None
//Description : Write config information to file XMS_mon.ini
//Input       : None
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void Set_ConfigInformation()
{
	pdlg->UpdateData(TRUE);
	char sztmp[128];
	
	pdlg->m_IPAddress.GetWindowText(sztmp, 32);
	strcpy(g_CFGInfo.IPAddress, sztmp);
	WritePrivateProfileString("ConfigInformation", "IPAddress", 
		                      sztmp, 
							  ".\\XMS_mon.ini"
							  );
	
	pdlg->m_Port.GetWindowText(sztmp, 32);
	g_CFGInfo.Port = atoi(sztmp);	
	WritePrivateProfileString("ConfigInformation", "Port", 
		                      sztmp, 
							  ".\\XMS_mon.ini"
							  );
	
	return;
}


//*****************************************************************************
//Name        : EVT_Handler
//Author      : None
//Description : Event handle for XMS system
//Input       : esrParam - parameter
//Output      : None
//Return      : 1 - Success 
//              0 - Failure
//*****************************************************************************
DJ_Void EVT_Handler(DJ_U32 esrParam)
{
	Acs_Dev_List_Head_t* pAcsDevList = NULL;
	Acs_Evt_t*			 pAcsEvt = NULL;
	
	pAcsEvt = (Acs_Evt_t *)esrParam;
	
	switch(pAcsEvt->m_s32EventType)
	{
	case XMS_EVT_QUERY_DEVICE:
		if (!g_StartWorkFlag)
		{				
			pAcsDevList = (Acs_Dev_List_Head_t *)FetchEventData(pAcsEvt);
			switch (pAcsDevList->m_s32DeviceMain)
			{
			case XMS_DEVMAIN_INTERFACE_CH:		
				AddDeviceRes(pAcsDevList);
				break;
			}
		}
		break;

	case XMS_EVT_QUERY_DEVICE_END:
		if (!g_StartWorkFlag)
		{	
			pdlg->m_BtnStart.EnableWindow();
			InitDevice();
		}
		break;
	
	case XMS_EVT_OPEN_DEVICE:
		Ack_OpenDevice(pAcsEvt);
		break;

	case XMS_EVT_SIGMON:
		if (g_StartWorkFlag)
		{
		Ack_SigMon(pAcsEvt);
		}
		break;

	default:
		break;
	}
	
	return;
}


//*****************************************************************************
//Name        : IsHizDevice
//Author      : None
//Description : Check device is HIZ or not
//Input       : idx - position/index of array g_pDevRes
//Output      : None
//Return      : 1 - Yes
//              0 - No
//*****************************************************************************
DJ_S32  IsHizDevice(DJ_S32 idx)
{
	if (DT_Main(idx) == XMS_DEVMAIN_INTERFACE_CH) // m_s16DeviceMain
	{	
		switch (DT_Sub(idx)) // m_s16DeviceSub
		{
		case XMS_DEVSUB_HIZ_HDLC_N64K_LINK:
		case XMS_DEVSUB_HIZ_SS7_64K_LINK:
		case XMS_DEVSUB_HIZ_SS7_2M_LINK:
		case XMS_DEVSUB_HIZ_SS7:
		
		case XMS_DEVSUB_HIZ_PRI_LINK:
		case XMS_DEVSUB_HIZ_PRI:
			return 1;
			break;
		}
	}
	
	return 0;
}


//*****************************************************************************
//Name        : OpenDevice
//Author      : None
//Description : Open all HiZ device, close these device in function:CloseDevice
//Input       : None
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void  OpenDevice()
{
	for (int i = 0; i < g_TotalDevRes; i++)
	{
		if (IsHizDevice(i))
		{
			XMS_ctsOpenDevice(g_acsHandle, &g_pDevRes[i].DevID, NULL);
		}
	}
}


//*****************************************************************************
//Name        : CloseDevice
//Author      : None
//Description : Close all opened device use function: OpenDevice
//Input       : None
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void  CloseDevice()
{
	for (int i = 0; i < g_TotalDevRes; i++)
	{
		if (g_pDevRes[i].bOpenFlag)
		{			
			XMS_ctsCloseDevice(g_acsHandle, &g_pDevRes[i].DevID, NULL);
		}
	}
}


//*****************************************************************************
//Name        : AddDeviceRes
//Author      : None
//Description : Record device information
//Input       : None
//Output      : None
//Return      : 0 - Success 
//             -1 - Failure
//*****************************************************************************
DJ_S32 AddDeviceRes(Acs_Dev_List_Head_t *pAcsDevList)
{	
	DeviceID_t	*pDev;
	void	*p;
	int		i;
	
	if (g_pDevRes == NULL)
	{
		g_pDevRes = (TYPE_MY_DEVICE_STRUCT *)malloc(pAcsDevList->m_s32DeviceNum * sizeof(TYPE_MY_DEVICE_STRUCT));
		if (g_pDevRes == NULL)
		{	
			return -1;
		}
	}
	else
	{
		p = realloc(g_pDevRes, (g_TotalDevRes + pAcsDevList->m_s32DeviceNum) * sizeof(TYPE_MY_DEVICE_STRUCT));
		if (p == NULL)
		{	
			return -1;
		}
		
		g_pDevRes = (TYPE_MY_DEVICE_STRUCT *)p;
	}
	
	pDev = (DeviceID_t *) ((DJ_S8 *)pAcsDevList + sizeof(Acs_Dev_List_Head_t));
	for (i = 0; i < pAcsDevList->m_s32DeviceNum; i++)
	{
		g_pDevRes[g_TotalDevRes + i].DevID = pDev[i];				
		
		g_pDevRes[g_TotalDevRes + i].bOpenFlag = 0;
		g_pDevRes[g_TotalDevRes + i].iState = 0;
		g_pDevRes[g_TotalDevRes + i].iEvent = 0;
		g_pDevRes[g_TotalDevRes + i].iPos = -1;
		
		memset(g_pDevRes[g_TotalDevRes + i].Caller, 
			   0, 
			   sizeof(g_pDevRes[g_TotalDevRes + i].Caller)
			   );
		
		memset(g_pDevRes[g_TotalDevRes + i].Called, 
			   0, 
			   sizeof(g_pDevRes[g_TotalDevRes + i].Called)
			   );
	}
	
	g_TotalDevRes += pAcsDevList->m_s32DeviceNum;
	
	return 0;
}


//*****************************************************************************
//Name        : InitDevice
//Author      : None
//Description : Initialize device information and fill info to list control
//Input       : None
//Output      : None
//Return      : 0 - Success 
//             -1 - Failure
//*****************************************************************************
DJ_S32  InitDevice()
{   
	pdlg->m_DeviceInfo.DeleteAllItems();
	
	char sztmp[128];
	int  type = 0;
	int  i = 0;
	int  j = 0;
	
	while (i < g_TotalDevRes)
	{	
		if (IsHizDevice(i))
		{
			//
			sprintf(sztmp, "[%d,%3d]", g_pDevRes[i].DevID.m_s8ModuleID,
				                       g_pDevRes[i].DevID.m_s16ChannelID
									   );
			pdlg->m_DeviceInfo.InsertItem(j, sztmp);
			
			//
			type=g_pDevRes[i].DevID.m_s16DeviceSub;
			switch(type)
			{
			case XMS_DEVSUB_HIZ_HDLC_N64K_LINK:
				strcpy(sztmp, "HIZ_HDLC_N64k_Link");
				break;

			case XMS_DEVSUB_HIZ_SS7_64K_LINK:
				strcpy(sztmp, "HIZ_SS7_64k_Link");
				break;

			case XMS_DEVSUB_HIZ_SS7_2M_LINK:
				strcpy(sztmp, "HIZ_SS7_2M_Link");
				break;

			case XMS_DEVSUB_HIZ_SS7:
				strcpy(sztmp, "HIZ_SS7");
				break;
				
			case XMS_DEVSUB_HIZ_PRI_LINK:
				strcpy(sztmp, "HIZ_PRI_Link");
				break;

			case XMS_DEVSUB_HIZ_PRI:
				strcpy(sztmp, "HIZ_PRI");
				break;

			default:
				sztmp[0] = 0;
				break;
			}
			pdlg->m_DeviceInfo.SetItemText(j, 1, sztmp);
			
			//
			pdlg->m_DeviceInfo.SetItemData(j, i);
			g_pDevRes[i].iPos = j;
			
			//
			j++;
		}
		
		i++;
	}
	
	return 0;
}


//*****************************************************************************
//Name        : Get_DeviceIndex
//Author      : None
//Description : Initialize device information and fill info to list control
//Input       : pDev - Pointer to DeviceID_t struct
//Output      : None
//Return      : Device index of array g_pDevRes (>=0)
//             -1 - Failure
//*****************************************************************************
DJ_S32 Get_DeviceIndex(DeviceID_t *pDev)
{	
	DeviceID_t *Dev= 0;
	
	for (int i = 0; i < g_TotalDevRes; i++)
	{
		Dev = &g_pDevRes[i].DevID;
		
		if ((Dev->m_s16DeviceMain == pDev->m_s16DeviceMain)
			&&
			(Dev->m_s16DeviceSub == pDev->m_s16DeviceSub)			
			&&
			(Dev->m_s8ModuleID == pDev->m_s8ModuleID)
			&&
			(Dev->m_s16ChannelID == pDev->m_s16ChannelID)
			)
		{
			return i;
			break;
		}
	}
	
	return -1;
}


//*****************************************************************************
//Name        : Ack_OpenDevice
//Author      : None
//Description : Event return because of open device success
//Input       : pAcsEvt
//Output      : None
//Return      : 0 - Success
//             -1 - Failure
//*****************************************************************************
DJ_S32  Ack_OpenDevice(Acs_Evt_t* pAcsEvt)
{
	DeviceID_t *pDev = &pAcsEvt->m_DeviceID;
	int index = Get_DeviceIndex(pDev);
	if (index==-1)
	{
		return -1;
	}
		
	///////////////////////////// Open device ok //////////////////////////////
	pdlg->m_DeviceInfo.SetItemText(g_pDevRes[index].iPos,2,"Opened,waiting evt");
	g_pDevRes[index].iState = ST_OPEN;
	g_pDevRes[index].DevID = *pDev;
	g_pDevRes[index].bOpenFlag = 1;
	//
	
	return 0;
}


//*****************************************************************************
//Name        : Ack_SigMon
//Author      : None
//Description : Signalling monitor event process
//Input       : pAcsEvt
//Output      : None
//Return      : 0 - Success
//             -1 - Failure
//*****************************************************************************
DJ_S32  Ack_SigMon(Acs_Evt_t* pAcsEvt)
{
	DeviceID_t *pDev = &pAcsEvt->m_DeviceID;
	PSMON_EVENT SMevt= NULL;
	
	int index = Get_DeviceIndex(pDev);
	if (index == -1)
	{
		return -1;
	}	
	
	SMevt = (PSMON_EVENT)FetchEventData(pAcsEvt);
	int iPos  = g_pDevRes[index].iPos;
	
	Show_Information(iPos, SMevt);
	Show_Data(SMevt);
	
	switch (SMevt->EventType)
	{
	case SMON_EVT_Call_Generate:
		pdlg->m_DeviceInfo.SetItemText(iPos, 2, "Call_Generate");
		pdlg->m_DeviceInfo.SetItemText(iPos, 3, SMevt->Caller_ID);
		pdlg->m_DeviceInfo.SetItemText(iPos, 4, SMevt->Called_ID);
		break;

	case SMON_EVT_Call_Connect:
		pdlg->m_DeviceInfo.SetItemText(iPos, 2, "Call_Connect");
		break;

	case SMON_EVT_Call_Disconnect:
		pdlg->m_DeviceInfo.SetItemText(iPos, 2, "Call_Disconnect");
		pdlg->m_DeviceInfo.SetItemText(iPos, 3, "");
		pdlg->m_DeviceInfo.SetItemText(iPos, 4, "");
		break;

	case SMON_EVT_Call_Info:
		pdlg->m_DeviceInfo.SetItemText(iPos, 2, "Call_Info");
		break;

	case SMON_Evt_Gen_Signal_Info:
		pdlg->m_DeviceInfo.SetItemText(iPos, 2, "Gen_SignalInfo");
		break;

	case SMON_Evt_Raw_Signal_Info:
		pdlg->m_DeviceInfo.SetItemText(iPos, 2, "Raw_SignalInfo");
		break;

	default:
		break;
	}
	
	return 0;
}


//*****************************************************************************
//Name        : Show_Information
//Author      : None
//Description : Display some information into list control item
//Input       : iPos  - List control item position(line index)
//              pData - Pointer to PSMON_EVENT struct
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void Show_Information(DJ_S32 iPos, DJ_Void* pData)
{
	PSMON_EVENT SMevt = (PSMON_EVENT)pData;
	char sztmp[128];
	
	///////////////////////////////////////////////////////////////////////////
	sprintf(sztmp, "MSG_type=0x%02x, Unit=%d, Port=%d, ts=%d, Raw_len=%d", 
			    SMevt->MsgType,
				SMevt->Unit_ID,
				SMevt->Port_ID,
				SMevt->TS,
				SMevt->DataLen
			    );
	//
	
	pdlg->m_DeviceInfo.SetItemText(iPos, 5, sztmp);
	
	return;
}


//*****************************************************************************
//Name        : Show_Data
//Author      : None
//Description : Display raw data information if it exist
//Input       : pData - Pointer to PSMON_EVENT struct
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void Show_Data(DJ_Void* pData)
{
	PSMON_EVENT SMevt = (PSMON_EVENT)pData;
	static int no =0;
	char sztmp[1024];
	int  i=0;
	int  k=0;
	
	///////////////////////////////////////////////////////////////////////////
	int  line= pdlg->m_RawLine.GetItemData(pdlg->m_RawLine.GetCurSel());
	int  cnt = pdlg->m_RawInfo.GetItemCount();
	int  j=cnt;
	
	if (SMevt->DataLen == 0)
	{
		return;
	}
	//
	
	if (cnt > line)
	{
Label_Delete_More_Line:
		pdlg->m_RawInfo.DeleteItem(0);
		j--;
		
		if (pdlg->m_RawInfo.GetItemCount() > line)
		{
		//	goto Label_Delete_More_Line;
		}
	}
	//
	
	///////////////////////////////////////////////////////////////////////////
	sprintf(sztmp, "%09d", ++no);             // 0, Number
	pdlg->m_RawInfo.InsertItem(j, sztmp);
	
	itoa(SMevt->Unit_ID, sztmp, 10);          // 1, data source: unit_id
	pdlg->m_RawInfo.SetItemText(j, 1, sztmp); 	
	
	itoa(SMevt->Port_ID, sztmp, 10);          // 2, data source: port_id
	pdlg->m_RawInfo.SetItemText(j, 2, sztmp); 
	
	itoa(SMevt->TS, sztmp, 10);               // 3, data source:timeslot
	pdlg->m_RawInfo.SetItemText(j, 3, sztmp);
	
	itoa(SMevt->DataLen, sztmp, 10);          // 4, raw data len
	pdlg->m_RawInfo.SetItemText(j, 4, sztmp);
	
	for (i=0; i < SMevt->DataLen; i++)        // 5, raw data
	{	
		sprintf(&sztmp[k], "%02X ", SMevt->RAWdata[i]);
		k += 3;
	}
	pdlg->m_RawInfo.SetItemText(j, 5, sztmp);
	pdlg->m_RawInfo.EnsureVisible(j, TRUE);
	//
	
	return;
}
