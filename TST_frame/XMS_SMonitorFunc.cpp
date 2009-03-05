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
	pdlg->m_RawInfo.InsertColumn(1, "USR_LK_MSG",LVCFMT_LEFT, 80);
	pdlg->m_RawInfo.InsertColumn(2, "Unit_ID", LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(3, "Port_ID", LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(4, "TS_ID",   LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(5, "Raw_Len", LVCFMT_LEFT, 60);
	pdlg->m_RawInfo.InsertColumn(6, "Raw_Data",LVCFMT_LEFT,400);
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

	/////////////////////////////RAW frame process part ///////////////////////
	case XMS_EVT_SS7RawFrame:
		if (g_StartWorkFlag)
		{
			Show_Data(pAcsEvt);
		}
		break;	

	case XMS_EVT_SS7SendRawFrame:
		{
			DeviceID_t *pDev = &pAcsEvt->m_DeviceID;			
			
			int index = Get_DeviceIndex(pDev);
			if (index == -1)
			{
				break;
			}
			
			int iPos  = g_pDevRes[index].iPos;			
			pdlg->m_DeviceInfo.SetItemText(iPos, 5, "Sent frame to flow, success");
		}
		break;

	case XMS_EVT_DEVICESTATE:
		ACK_DeviceState(pAcsEvt);
		break;
	//
	
	default:
		break;
	}
	
	return;
}


//*****************************************************************************
//Name        : IsLinkDevice
//Author      : None
//Description : Check device is Link or not
//Input       : idx - position/index of array g_pDevRes
//Output      : None
//Return      : 1 - Yes
//              0 - No
//*****************************************************************************
DJ_S32  IsLinkDevice(DJ_S32 idx)
{
	if (DT_Main(idx) == XMS_DEVMAIN_INTERFACE_CH) // m_s16DeviceMain
	{	
		switch (DT_Sub(idx)) // m_s16DeviceSub
		{
		case XMS_DEVSUB_SS7_LINK:
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
		if (IsLinkDevice(i))
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
		if (IsLinkDevice(i))
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
			case XMS_DEVSUB_SS7_LINK:
				strcpy(sztmp, "SS7_Link");
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


DJ_S32  ACK_DeviceState(Acs_Evt_t* pAcsEvt)
{
	DeviceID_t *pDev = &pAcsEvt->m_DeviceID;
	int index = Get_DeviceIndex(pDev);
	if (index==-1)
	{
		return -1;
	}
	
	
	Acs_GeneralProc_Data *pData = (Acs_GeneralProc_Data*)FetchEventData(pAcsEvt);
	char sztmp[128] = {0};
	
	switch (pData->m_s32DeviceState)
	{
	case DES_SS7_LST_CONNECTING:
		strcpy(sztmp, "Connecting");
		break;

	case DES_SS7_LST_ACTIVE:
		strcpy(sztmp, "Active");
		break;

	case DES_SS7_LST_FAILED:
		strcpy(sztmp, "Failed");
		break;

	case DES_SS7_LST_WAITCON:
		strcpy(sztmp, "WaitConn");
		break;

	case DES_SS7_LST_SUSPEND:
		strcpy(sztmp, "Suspend");
		break;

	default:
		sprintf(sztmp, "State=0x%x, unknown", pData->m_s32DeviceState);
		break;
	}
	
	pdlg->m_DeviceInfo.SetItemText(g_pDevRes[index].iPos,2,sztmp);
	return true;
}


//*****************************************************************************
//Name        : Show_Data
//Author      : None
//Description : Display raw data information if it exist
//Input       : pAcsEvt
//Output      : None
//Return      : None
//*****************************************************************************
DJ_Void Show_Data(Acs_Evt_t* pAcsEvt)
{
	Acs_SS7RawFrame_Data* pData=(Acs_SS7RawFrame_Data*)FetchEventData(pAcsEvt);
	static int no =0;
	char sztmp[1024];
	int  i=0;
	int  k=0;
	
	///////////////////////////////////////////////////////////////////////////	
	int  cnt = pdlg->m_RawInfo.GetItemCount();
	int  j=cnt;
	
	if (pData->m_u8InfoLen == 0)
	{
		return;
	}
	//		
	
	///////////////////////////////////////////////////////////////////////////
	sprintf(sztmp, "%09d", ++no);             // 0, Number
	pdlg->m_RawInfo.InsertItem(j, sztmp);

	sprintf(sztmp, "%02d_%02d_%02d", pData->m_u8UserType, pData->m_u8LinkType, 
		    pData->m_u8InfoType
			);
	pdlg->m_RawInfo.SetItemText(j, 1, sztmp);
	
	itoa(pData->m_u8Rfu1[0], sztmp, 10);          // 1, data source: unit_id
	pdlg->m_RawInfo.SetItemText(j, 2, sztmp);
	
	itoa(pData->m_u8Rfu1[1], sztmp, 10);          // 2, data source: port_id
	pdlg->m_RawInfo.SetItemText(j, 3, sztmp); 
	
	itoa(pData->m_u8Rfu1[2], sztmp, 10);          // 3, data source:timeslot
	pdlg->m_RawInfo.SetItemText(j, 4, sztmp);
	
	itoa(pData->m_u8InfoLen, sztmp, 10);          // 4, raw data len
	pdlg->m_RawInfo.SetItemText(j, 5, sztmp);
	
	for (i=0; i < pData->m_u8InfoLen; i++)        // 5, raw data
	{	
		sprintf(&sztmp[k], "%02X ", pData->m_u8Info[i]);
		k += 3;
	}
	pdlg->m_RawInfo.SetItemText(j, 6, sztmp);
	pdlg->m_RawInfo.EnsureVisible(j, TRUE);
	//
	
	return;
}


//*****************************************************************************
//Name        : SplitStringToByte
//Author      : None
//Description : Split string to byte
//Input       : psz - string , Value - to store byte
//Output      : None
//Return      : byte count
//*****************************************************************************
DJ_S32  SplitStringToByte(DJ_S8 *psz, DJ_U8 Value[])
{
	char *token = strtok(psz, " ");
	int  iCount = 0;
	
	while (token)
	{
		sscanf(token, "%x", &Value[iCount]);
		
		token = strtok(NULL, " ");
		
		iCount ++;
	}
	
	return iCount;
}


//*****************************************************************************
//Name        : Send_Frame
//Author      : None
//Description : Send raw frame
//Input       : None
//Output      : None
//Return      : byte count
//*****************************************************************************
DJ_S32  Send_Frame(void)
{	
	int iPos = pdlg->m_DeviceInfo.GetSelectionMark();
	if (!g_StartWorkFlag || iPos == -1)
	{
		MessageBox(NULL,"Start and select device first","Error",MB_OK);
		return 0;
	}
	
	int index = pdlg->m_DeviceInfo.GetItemData(iPos);
	DeviceID_t *pDev= &g_pDevRes[index].DevID;
	//
	
	///////////////////////////////////////////////////////////////////////////
	BYTE    Byte_val[512] = {0};
	int     Byte_cnt=0;
	CString string_hex;
	
	pdlg->m_Data.GetWindowText(string_hex);
	Byte_cnt = SplitStringToByte(string_hex.GetBuffer(0), Byte_val);
	//
	
	if (Byte_cnt < 1+3+3+2+1+1) //sio,dpc,opc,cic,h1h0,data//
	{
		return -1;
	}
	
	
	// Send raw data from SIO, input hex data: 85 01 02 03 ...... /////////////
	Acs_SS7RawFrame_Data   raw_data = {0};
	
	raw_data.m_u8LinkType = XMS_SS7_LINK_TYPE_CHINA;
	
	raw_data.m_u8UserType =XMS_SS7_USER_TYPE_SCCP;
	raw_data.m_u8InfoType  = XMS_SS7_MSG_DATA; // Must set to data // 
	
	raw_data.m_u8InfoLen  = Byte_cnt;
	memcpy(raw_data.m_u8Info, Byte_val, 
		   Byte_cnt
		   );
	//
	
	RetCode_t r = XMS_ctsSendRawFrame(g_acsHandle, pDev, 
		          // + 8 : see define, Acs_SS7RawFrame_Data
				  raw_data.m_u8InfoLen + 8,
				  &raw_data
		          );
	
//	typedef struct 
//	{
//		DJ_U8 m_u8UserType; // 8 byte start
//		DJ_U8 m_u8LinkType;
//		DJ_U8 m_u8MsgType;
//		DJ_U8 m_u8InfoLen;
//		
//		DJ_U8 m_u8UnitId;
//		DJ_U8 m_u8E1Port;
//		DJ_U8 m_u8Ts;
//		DJ_U8 m_u8Reserved1; // 8 byte end
//		
//		DJ_U8 m_u8Info[255]; // the information length is m_u8InfoLen
//		DJ_U8 Reserved2;
//	}Acs_SS7RawFrame_Data;
	
	if (r < 0)
	{
		MessageBox(NULL, "send err", "err", MB_OK);
	}
	
	return r;
}
