/********************************************************************************************************
 * @file    zcl_appCb.c
 *
 * @brief   This is the source file for zcl_appCb
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

/**********************************************************************
 * INCLUDES
 */
#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"


/**********************************************************************
 * LOCAL CONSTANTS
 */


/**********************************************************************
 * TYPEDEFS
 */


/**********************************************************************
 * LOCAL FUNCTIONS
 */
#ifdef ZCL_READ
static void app_zclReadRspCmd(uint16_t clusterId, zclReadRspCmd_t *pReadRspCmd);
#endif
#ifdef ZCL_WRITE
static void app_zclWriteRspCmd(uint16_t clusterId, zclWriteRspCmd_t *pWriteRspCmd);
static void app_zclWriteReqCmd(uint8_t endPoint, uint16_t clusterId, zclWriteCmd_t *pWriteReqCmd);
#endif
#ifdef ZCL_REPORT
static void app_zclCfgReportCmd(uint8_t endPoint, uint16_t clusterId, zclCfgReportCmd_t *pCfgReportCmd);
static void app_zclCfgReportRspCmd(uint16_t clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd);
static void app_zclReportCmd(uint16_t clusterId, zclReportCmd_t *pReportCmd);
#endif
static void app_zclDfltRspCmd(uint16_t clusterId, zclDefaultRspCmd_t *pDftRspCmd);


/**********************************************************************
 * GLOBAL VARIABLES
 */


/**********************************************************************
 * LOCAL VARIABLES
 */
#ifdef ZCL_IDENTIFY
static ev_timer_event_t *identifyTimerEvt = NULL;
#endif

/**********************************************************************
 * FUNCTIONS
 */

/*********************************************************************
 * @fn      app_zclProcessIncomingMsg
 *
 * @brief   Process ZCL Foundation incoming message.
 *
 * @param   pInMsg - pointer to the received message
 *
 * @return  None
 */
void app_zclProcessIncomingMsg(zclIncoming_t *pInHdlrMsg)
{
	//printf("app_zclProcessIncomingMsg\n");

	uint16_t cluster = pInHdlrMsg->msg->indInfo.cluster_id;
	uint8_t endPoint = pInHdlrMsg->msg->indInfo.dst_ep;
	switch(pInHdlrMsg->hdr.cmd)
	{
#ifdef ZCL_READ
		case ZCL_CMD_READ_RSP:
			app_zclReadRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_WRITE
		case ZCL_CMD_WRITE_RSP:
			app_zclWriteRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_WRITE:
		    app_zclWriteReqCmd(endPoint, cluster, pInHdlrMsg->attrCmd);
			break;
#endif
#ifdef ZCL_REPORT
		case ZCL_CMD_CONFIG_REPORT:
			app_zclCfgReportCmd(endPoint, cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_CONFIG_REPORT_RSP:
			app_zclCfgReportRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		case ZCL_CMD_REPORT:
			app_zclReportCmd(cluster, pInHdlrMsg->attrCmd);
			break;
#endif
		case ZCL_CMD_DEFAULT_RSP:
			app_zclDfltRspCmd(cluster, pInHdlrMsg->attrCmd);
			break;
		default:
			break;
	}
}

#ifdef ZCL_READ
/*********************************************************************
 * @fn      app_zclReadRspCmd
 *
 * @brief   Handler for ZCL Read Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclReadRspCmd(uint16_t clusterId, zclReadRspCmd_t *pReadRspCmd)
{
    //printf("app_zclReadRspCmd\n");

}
#endif	/* ZCL_READ */

#ifdef ZCL_WRITE
/*********************************************************************
 * @fn      app_zclWriteRspCmd
 *
 * @brief   Handler for ZCL Write Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclWriteRspCmd(uint16_t clusterId, zclWriteRspCmd_t *pWriteRspCmd)
{
    //printf("app_zclWriteRspCmd\n");

}

/*********************************************************************
 * @fn      app_zclWriteReqCmd
 *
 * @brief   Handler for ZCL Write Request command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclWriteReqCmd(uint8_t endPoint, uint16_t clusterId, zclWriteCmd_t *pWriteReqCmd) {
    uint8_t numAttr = pWriteReqCmd->numAttr;
    zclWriteRec_t *attr = pWriteReqCmd->attrList;
    bool save = false;

//    printf("app_zclWriteReqCmd\r\n");

    uint8_t idx = endPoint - 1;
    zcl_onOffCfgAttr_t *pOnOffCfg = zcl_onOffCfgAttrsGet();
    pOnOffCfg += idx;

    if (clusterId == ZCL_CLUSTER_GEN_ON_OFF_SWITCH_CONFIG) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_CUSTOM_ATTRID_SWITCH_TYPE) {
                uint8_t type = attr[i].attrData[0];
                printf("type: 0x%02x, ep: %d\r\n", type, endPoint);
                if (type >= ZCL_SWITCH_TYPE_TOGGLE && type < ZCL_CUSTOM_SWITCH_TYPE_MAXNUM) {
                    device_settings.switchType[idx] = type;
                    pOnOffCfg->switchType = type;
                    save = true;
                }
            } else if (attr[i].attrID == ZCL_ATTRID_SWITCH_ACTION) {
                uint8_t action = attr[i].attrData[0];
//                printf("action: 0x%02x, ep: %d\r\n", action, endPoint);
                device_settings.switchActions[idx] = action;
                save = true;
            } else if (attr[i].attrID == ZCL_CUSTOM_ATTRID_DEVICE_MODEL) {
                uint8_t model = attr[i].attrData[0];
//                printf("model: 0x%02x, ep: %d\r\n", model, endPoint);
                model--;
                if (model >= DEVICE_MODEL_1 && model < DEVICE_MODEL_MAX) {
                    device_model_save(model);
                }
            }
        }
    }

    if (clusterId == ZCL_CLUSTER_GEN_LEVEL_CONTROL) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_ATTRID_LEVEL_DEFAULT_MOVE_RATE) {
                uint8_t rate = attr[i].attrData[0];
//                printf("Level rate: 0x%02x, ep: %d\r\n", rate, endPoint);
                device_settings.defaultMoveRate[idx] = rate;
                save = true;
            }
        }
    }

    if (clusterId == ZCL_CLUSTER_GEN_SCENES) {
        for (u8 i = 0; i < numAttr; i++) {
            if (attr[i].attrID == ZCL_CUSTOM_ATTRID_SCENE_SCENE_ID) {
                uint8_t scene = attr[i].attrData[0];
                printf("Scne ID: 0x%02x, ep: %d\r\n", scene, endPoint);
                device_settings.scene[idx].sceneId = scene;
                save = true;
            } else if (attr[i].attrID == ZCL_CUSTOM_ATTRID_SCENE_GROUP_ID) {
                uint16_t group = attr[i].attrData[0] & 0xff;
                group |= (attr[i].attrData[1] << 8) & 0xffff;
                printf("Group ID: 0x%04x, ep: %d\r\n", group, endPoint);
                device_settings.scene[idx].groupId = group;
                save = true;
            }
        }
    }

    if (save) device_settings_save();

#ifdef ZCL_POLL_CTRL
	if(clusterId == ZCL_CLUSTER_GEN_POLL_CONTROL){
		for(int32_t i = 0; i < numAttr; i++){
			if(attr[i].attrID == ZCL_ATTRID_CHK_IN_INTERVAL){
				app_zclCheckInStart();
				return;
			}
		}
	}
#endif
}
#endif	/* ZCL_WRITE */


/*********************************************************************
 * @fn      app_zclDfltRspCmd
 *
 * @brief   Handler for ZCL Default Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclDfltRspCmd(uint16_t clusterId, zclDefaultRspCmd_t *pDftRspCmd)
{
    //printf("app_zclDfltRspCmd\r\n");

}

#ifdef ZCL_REPORT
/*********************************************************************
 * @fn      app_zclCfgReportCmd
 *
 * @brief   Handler for ZCL Configure Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 * @param   dst_ep     - number of endPoint
 *
 * @return  None
 */
static void app_zclCfgReportCmd(uint8_t endPoint, uint16_t clusterId, zclCfgReportCmd_t *pCfgReportCmd)
{
//    printf("app_zclCfgReportCmd\r\n");
//    reportCfgInfo_t *pEntry = NULL;
//    for (u8 i = 0; i < ZCL_REPORTING_TABLE_NUM; i++) {
//        pEntry = &reportingTab.reportCfgInfo[i];
//        if (pEntry->used) {
//            printf("ep: %d, cluster: 0x%04x, attr: 0x%04x, min: %d, max: %d\r\n", pEntry->endPoint, pEntry->clusterID, pEntry->attrID, pEntry->minInterval, pEntry->maxInterval);
//        }
//
//    }

    if (clusterId == ZCL_CLUSTER_GEN_POWER_CFG) {
        for (uint8_t i = 0; i < pCfgReportCmd->numAttr; i++) {
            if (pCfgReportCmd->attrList[i].attrID == ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING) {
                DEBUG(DEBUG_REPORTING_EN, "Battery reporting configure - ep: %d, clId: 0x%04x, attrId: 0x%04x, maxInterval: %d\r\n",
                        endPoint, clusterId, pCfgReportCmd->attrList[i].attrID, pCfgReportCmd->attrList[i].maxReportInt);

                if (g_appCtx.timerBatteryEvt) {
                    TL_ZB_TIMER_CANCEL(&g_appCtx.timerBatteryEvt);
                }
                g_appCtx.timerBatteryEvt = TL_ZB_TIMER_SCHEDULE(batteryCb, NULL, pCfgReportCmd->attrList[i].maxReportInt * 1000);
            }
        }
    }

    app_setPollRate(TIMEOUT_30SEC);
}

/*********************************************************************
 * @fn      app_zclCfgReportRspCmd
 *
 * @brief   Handler for ZCL Configure Report Response command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclCfgReportRspCmd(uint16_t clusterId, zclCfgReportRspCmd_t *pCfgReportRspCmd)
{
    printf("app_zclCfgReportRspCmd\r\n");

}

/*********************************************************************
 * @fn      app_zclReportCmd
 *
 * @brief   Handler for ZCL Report command.
 *
 * @param   pInHdlrMsg - incoming message to process
 *
 * @return  None
 */
static void app_zclReportCmd(uint16_t clusterId, zclReportCmd_t *pReportCmd)
{
    //printf("app_zclReportCmd\n");

}
#endif	/* ZCL_REPORT */

#ifdef ZCL_BASIC
/*********************************************************************
 * @fn      app_zclBasicResetCmdHandler
 *
 * @brief   Handler for ZCL Basic Reset command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_basicCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
	if(cmdId == ZCL_CMD_BASIC_RESET_FAC_DEFAULT){
		//Reset all the attributes of all its clusters to factory defaults
		//zcl_nv_attr_reset();
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_BASIC */

#ifdef ZCL_IDENTIFY
int32_t app_zclIdentifyTimerCb(void *arg)
{
	if(g_zcl_identifyAttrs.identifyTime <= 0){
		identifyTimerEvt = NULL;
		return -1;
	}
	g_zcl_identifyAttrs.identifyTime--;
	return 0;
}

void app_zclIdentifyTimerStop(void)
{
	if(identifyTimerEvt){
		TL_ZB_TIMER_CANCEL(&identifyTimerEvt);
	}
}

/*********************************************************************
 * @fn      app_zclIdentifyCmdHandler
 *
 * @brief   Handler for ZCL Identify command. This function will set blink LED.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyTime - identify time
 *
 * @return  None
 */
void app_zclIdentifyCmdHandler(uint8_t endpoint, uint16_t srcAddr, uint16_t identifyTime)
{
	g_zcl_identifyAttrs.identifyTime = identifyTime;

	if(identifyTime == 0){
		app_zclIdentifyTimerStop();
		light_blink_all_stop();
	}else{
		if(!identifyTimerEvt){
			light_blink_all_start(identifyTime, 500, 500);
			identifyTimerEvt = TL_ZB_TIMER_SCHEDULE(app_zclIdentifyTimerCb, NULL, 1000);
		}
	}
}

/*********************************************************************
 * @fn      app_zcltriggerCmdHandler
 *
 * @brief   Handler for ZCL trigger command.
 *
 * @param   pTriggerEffect
 *
 * @return  None
 */
static void app_zcltriggerCmdHandler(zcl_triggerEffect_t *pTriggerEffect)
{
	uint8_t effectId = pTriggerEffect->effectId;
	//uint8_t effectVariant = pTriggerEffect->effectVariant;

	switch(effectId){
		case IDENTIFY_EFFECT_BLINK:
			light_blink_all_start(1, 500, 500);
			break;
		case IDENTIFY_EFFECT_BREATHE:
			light_blink_all_start(15, 300, 700);
			break;
		case IDENTIFY_EFFECT_OKAY:
			light_blink_all_start(2, 250, 250);
			break;
		case IDENTIFY_EFFECT_CHANNEL_CHANGE:
			light_blink_all_start(1, 500, 7500);
			break;
		case IDENTIFY_EFFECT_FINISH_EFFECT:
			light_blink_all_start(1, 300, 700);
			break;
		case IDENTIFY_EFFECT_STOP_EFFECT:
			light_blink_all_stop();
			break;
		default:
			break;
	}
}

/*********************************************************************
 * @fn      app_zclIdentifyQueryRspCmdHandler
 *
 * @brief   Handler for ZCL Identify Query response command.
 *
 * @param   endpoint
 * @param   srcAddr
 * @param   identifyRsp
 *
 * @return  None
 */
static void app_zclIdentifyQueryRspCmdHandler(uint8_t endpoint, uint16_t srcAddr, zcl_identifyRspCmd_t *identifyRsp)
{
#if FIND_AND_BIND_SUPPORT
	if(identifyRsp->timeout){
		findBindDst_t dstInfo;
		dstInfo.addr = srcAddr;
		dstInfo.endpoint = endpoint;

		bdb_addIdentifyActiveEpForFB(dstInfo);
	}
#endif
}

/*********************************************************************
 * @fn      app_identifyCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_identifyCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
	if(pAddrInfo->dstEp == APP_ENDPOINT1){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_IDENTIFY:
					app_zclIdentifyCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, ((zcl_identifyCmd_t *)cmdPayload)->identifyTime);
					break;
				case ZCL_CMD_TRIGGER_EFFECT:
					app_zcltriggerCmdHandler((zcl_triggerEffect_t *)cmdPayload);
					break;
				default:
					break;
			}
		}else{
			if(cmdId == ZCL_CMD_IDENTIFY_QUERY_RSP){
				app_zclIdentifyQueryRspCmdHandler(pAddrInfo->dstEp, pAddrInfo->srcAddr, (zcl_identifyRspCmd_t *)cmdPayload);
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_IDENTIFY */

/*********************************************************************
 * @fn      app_powerCfgCb
 *
 * @brief   Handler for ZCL Identify command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_powerCfgCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{

    return ZCL_STA_SUCCESS;
}


#ifdef ZCL_GROUP
/*********************************************************************
 * @fn      app_zclAddGroupRspCmdHandler
 *
 * @brief   Handler for ZCL add group response command.
 *
 * @param   pAddGroupRsp
 *
 * @return  None
 */
static void app_zclAddGroupRspCmdHandler(zcl_addGroupRsp_t *pAddGroupRsp)
{
//    printf("app_zclAddGroupRspCmdHandler\r\n");
}

/*********************************************************************
 * @fn      app_zclViewGroupRspCmdHandler
 *
 * @brief   Handler for ZCL view group response command.
 *
 * @param   pViewGroupRsp
 *
 * @return  None
 */
static void app_zclViewGroupRspCmdHandler(zcl_viewGroupRsp_t *pViewGroupRsp)
{
//    printf("app_zclViewGroupRspCmdHandler\r\n");
}

/*********************************************************************
 * @fn      app_zclRemoveGroupRspCmdHandler
 *
 * @brief   Handler for ZCL remove group response command.
 *
 * @param   pRemoveGroupRsp
 *
 * @return  None
 */
static void app_zclRemoveGroupRspCmdHandler(zcl_removeGroupRsp_t *pRemoveGroupRsp)
{
//    printf("app_zclRemoveGroupRspCmdHandler\r\n");
}

/*********************************************************************
 * @fn      app_zclGetGroupMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get group membership response command.
 *
 * @param   pGetGroupMembershipRsp
 *
 * @return  None
 */
static void app_zclGetGroupMembershipRspCmdHandler(zcl_getGroupMembershipRsp_t *pGetGroupMembershipRsp)
{
//    printf("app_zclGetGroupMembershipRspCmdHandler\r\n");
}

/*********************************************************************
 * @fn      app_groupCb
 *
 * @brief   Handler for ZCL Group command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_groupCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
//    printf("app_groupCb\r\n");
	if(pAddrInfo->dstEp == APP_ENDPOINT1){
		if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
			switch(cmdId){
				case ZCL_CMD_GROUP_ADD_GROUP_RSP:
					app_zclAddGroupRspCmdHandler((zcl_addGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_VIEW_GROUP_RSP:
					app_zclViewGroupRspCmdHandler((zcl_viewGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_REMOVE_GROUP_RSP:
					app_zclRemoveGroupRspCmdHandler((zcl_removeGroupRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_GROUP_GET_MEMBERSHIP_RSP:
					app_zclGetGroupMembershipRspCmdHandler((zcl_getGroupMembershipRsp_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return ZCL_STA_SUCCESS;
}
#endif	/* ZCL_GROUP */

#ifdef ZCL_SCENE1
/*********************************************************************
 * @fn      app_zclAddSceneRspCmdHandler
 *
 * @brief   Handler for ZCL add scene response command.
 *
 * @param   cmdId
 * @param   pAddSceneRsp
 *
 * @return  None
 */
static void app_zclAddSceneRspCmdHandler(uint8_t cmdId, addSceneRsp_t *pAddSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclViewSceneRspCmdHandler
 *
 * @brief   Handler for ZCL view scene response command.
 *
 * @param   cmdId
 * @param   pViewSceneRsp
 *
 * @return  None
 */
static void app_zclViewSceneRspCmdHandler(uint8_t cmdId, viewSceneRsp_t *pViewSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclRemoveSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove scene response command.
 *
 * @param   pRemoveSceneRsp
 *
 * @return  None
 */
static void app_zclRemoveSceneRspCmdHandler(removeSceneRsp_t *pRemoveSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclRemoveAllSceneRspCmdHandler
 *
 * @brief   Handler for ZCL remove all scene response command.
 *
 * @param   pRemoveAllSceneRsp
 *
 * @return  None
 */
static void app_zclRemoveAllSceneRspCmdHandler(removeAllSceneRsp_t *pRemoveAllSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclStoreSceneRspCmdHandler
 *
 * @brief   Handler for ZCL store scene response command.
 *
 * @param   pStoreSceneRsp
 *
 * @return  None
 */
static void app_zclStoreSceneRspCmdHandler(storeSceneRsp_t *pStoreSceneRsp)
{

}

/*********************************************************************
 * @fn      app_zclGetSceneMembershipRspCmdHandler
 *
 * @brief   Handler for ZCL get scene membership response command.
 *
 * @param   pGetSceneMembershipRsp
 *
 * @return  None
 */
static void app_zclGetSceneMembershipRspCmdHandler(getSceneMemRsp_t *pGetSceneMembershipRsp)
{

}
#endif  /* ZCL_SCENE */

/*********************************************************************
 * @fn      app_sceneCb
 *
 * @brief   Handler for ZCL Scene command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_sceneCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload) {

    printf("app_sceneCb\r\n");

//    if(pAddrInfo->dirCluster == ZCL_FRAME_SERVER_CLIENT_DIR){
//        switch(cmdId){
//            case ZCL_CMD_SCENE_ADD_SCENE_RSP:
//            case ZCL_CMD_SCENE_ENHANCED_ADD_SCENE_RSP:
//                app_zclAddSceneRspCmdHandler(cmdId, (addSceneRsp_t *)cmdPayload);
//                break;
//            case ZCL_CMD_SCENE_VIEW_SCENE_RSP:
//            case ZCL_CMD_SCENE_ENHANCED_VIEW_SCENE_RSP:
//                app_zclViewSceneRspCmdHandler(cmdId, (viewSceneRsp_t *)cmdPayload);
//                break;
//            case ZCL_CMD_SCENE_REMOVE_SCENE_RSP:
//                app_zclRemoveSceneRspCmdHandler((removeSceneRsp_t *)cmdPayload);
//                break;
//            case ZCL_CMD_SCENE_REMOVE_ALL_SCENE_RSP:
//                app_zclRemoveAllSceneRspCmdHandler((removeAllSceneRsp_t *)cmdPayload);
//                break;
//            case ZCL_CMD_SCENE_STORE_SCENE_RSP:
//                app_zclStoreSceneRspCmdHandler((storeSceneRsp_t *)cmdPayload);
//                break;
//            case ZCL_CMD_SCENE_GET_SCENE_MEMSHIP_RSP:
//                app_zclGetSceneMembershipRspCmdHandler((getSceneMemRsp_t *)cmdPayload);
//                break;
//            default:
//                break;
//        }
//    }

	return ZCL_STA_SUCCESS;
}

#ifdef ZCL_POLL_CTRL
static ev_timer_event_t *zclFastPollTimeoutTimerEvt = NULL;
static ev_timer_event_t *zclCheckInTimerEvt = NULL;
static bool isFastPollMode = FALSE;

void app_zclCheckInCmdSend(void)
{
	epInfo_t dstEpInfo;
	TL_SETSTRUCTCONTENT(dstEpInfo, 0);

	dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
	dstEpInfo.dstEp = APP_ENDPOINT;
	dstEpInfo.profileId = HA_PROFILE_ID;

	zcl_pollCtrl_checkInCmd(APP_ENDPOINT, &dstEpInfo, TRUE);
}

int32_t app_zclCheckInTimerCb(void *arg)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(!pPollCtrlAttr->chkInInterval){
		zclCheckInTimerEvt = NULL;
		return -1;
	}

	app_zclCheckInCmdSend();

	return 0;
}

void app_zclCheckInStart(void)
{
	if(zb_bindingTblSearched(ZCL_CLUSTER_GEN_POLL_CONTROL, APP_ENDPOINT)){
		zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

		if(!zclCheckInTimerEvt){
			zclCheckInTimerEvt = TL_ZB_TIMER_SCHEDULE(app_zclCheckInTimerCb, NULL, pPollCtrlAttr->chkInInterval * POLL_RATE_QUARTERSECONDS);

			if(pPollCtrlAttr->chkInInterval){
				app_zclCheckInCmdSend();
			}
		}
	}
}

void app_zclSetFastPollMode(bool fastPollMode)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	isFastPollMode = fastPollMode;
	uint32_t pollRate = fastPollMode ? pPollCtrlAttr->shortPollInterval : pPollCtrlAttr->longPollInterval;

	zb_setPollRate(pollRate  * POLL_RATE_QUARTERSECONDS);
}

int32_t app_zclFastPollTimeoutCb(void *arg)
{
	app_zclSetFastPollMode(FALSE);

	zclFastPollTimeoutTimerEvt = NULL;
	return -1;
}

static status_t app_zclPollCtrlChkInRspCmdHandler(zcl_chkInRsp_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if(pCmd->startFastPolling){
		uint16_t fastPollTimeoutCnt = 0;

		if(pCmd->fastPollTimeout){
			if(pCmd->fastPollTimeout > pPollCtrlAttr->fastPollTimeoutMax){
				return ZCL_STA_INVALID_FIELD;
			}

			fastPollTimeoutCnt = pCmd->fastPollTimeout;
		}else{
			fastPollTimeoutCnt = pPollCtrlAttr->fastPollTimeout;
		}

		if(fastPollTimeoutCnt){
			app_zclSetFastPollMode(TRUE);

			if(zclFastPollTimeoutTimerEvt){
				TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
			}
			zclFastPollTimeoutTimerEvt = TL_ZB_TIMER_SCHEDULE(app_zclFastPollTimeoutCb, NULL, fastPollTimeoutCnt * POLL_RATE_QUARTERSECONDS);
		}
	}else{
		//continue in normal operation and not required to go into fast poll mode.
	}

	return ZCL_STA_SUCCESS;
}

static status_t app_zclPollCtrlFastPollStopCmdHandler(void)
{
	if(!isFastPollMode){
		return ZCL_STA_ACTION_DENIED;
	}else{
		if(zclFastPollTimeoutTimerEvt){
			TL_ZB_TIMER_CANCEL(&zclFastPollTimeoutTimerEvt);
		}
		app_zclSetFastPollMode(FALSE);
	}

	return ZCL_STA_SUCCESS;
}

static status_t app_zclPollCtrlSetLongPollIntervalCmdHandler(zcl_setLongPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newLongPollInterval >= 0x04) && (pCmd->newLongPollInterval <= 0x6E0000)
		&& (pCmd->newLongPollInterval <= pPollCtrlAttr->chkInInterval) && (pCmd->newLongPollInterval >= pPollCtrlAttr->shortPollInterval)){
		pPollCtrlAttr->longPollInterval = pCmd->newLongPollInterval;
		zb_setPollRate(pCmd->newLongPollInterval * POLL_RATE_QUARTERSECONDS);
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

static status_t app_zclPollCtrlSetShortPollIntervalCmdHandler(zcl_setShortPollInterval_t *pCmd)
{
	zcl_pollCtrlAttr_t *pPollCtrlAttr = zcl_pollCtrlAttrGet();

	if((pCmd->newShortPollInterval >= 0x01) && (pCmd->newShortPollInterval <= 0xff)
		&& (pCmd->newShortPollInterval <= pPollCtrlAttr->longPollInterval)){
		pPollCtrlAttr->shortPollInterval = pCmd->newShortPollInterval;
		zb_setPollRate(pCmd->newShortPollInterval * POLL_RATE_QUARTERSECONDS);
	}else{
		return ZCL_STA_INVALID_VALUE;
	}

	return ZCL_STA_SUCCESS;
}

/*********************************************************************
 * @fn      app_pollCtrlCb
 *
 * @brief   Handler for ZCL Poll Control command.
 *
 * @param   pAddrInfo
 * @param   cmdId
 * @param   cmdPayload
 *
 * @return  status_t
 */
status_t app_pollCtrlCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload)
{
	status_t status = ZCL_STA_SUCCESS;

	if(pAddrInfo->dstEp == APP_ENDPOINT){
		if(pAddrInfo->dirCluster == ZCL_FRAME_CLIENT_SERVER_DIR){
			switch(cmdId){
				case ZCL_CMD_CHK_IN_RSP:
					status = app_zclPollCtrlChkInRspCmdHandler((zcl_chkInRsp_t *)cmdPayload);
					break;
				case ZCL_CMD_FAST_POLL_STOP:
					status = app_zclPollCtrlFastPollStopCmdHandler();
					break;
				case ZCL_CMD_SET_LONG_POLL_INTERVAL:
					status = app_zclPollCtrlSetLongPollIntervalCmdHandler((zcl_setLongPollInterval_t *)cmdPayload);
					break;
				case ZCL_CMD_SET_SHORT_POLL_INTERVAL:
					status = app_zclPollCtrlSetShortPollIntervalCmdHandler((zcl_setShortPollInterval_t *)cmdPayload);
					break;
				default:
					break;
			}
		}
	}

	return status;
}
#endif	/* ZCL_POLL_CTRL */

status_t app_msInputCb(zclIncomingAddrInfo_t *pAddrInfo, uint8_t cmdId, void *cmdPayload) {

//    printf("app_aInputCb(). pAddrInfo->dirCluster: %0x%x, cmdId: 0x%x\r\n", pAddrInfo->dirCluster, cmdId);

    status_t status = ZCL_STA_SUCCESS;

    return status;
}

/*********************************************************************
 * @fn      app_levelCb
 *
 * @brief   Handler for ZCL LEVEL command. This function will set LEVEL attribute first.
 *
 * @param   pAddrInfo
 * @param   cmd - level cluster command id
 * @param   cmdPayload
 *
 * @return  status_t
 */

status_t app_levelCb(zclIncomingAddrInfo_t *pAddrInfo, u8 cmdId, void *cmdPayload) {
    printf("app_LevelCb\r\n");
//    if(pAddrInfo->dstEp == APP_ENDPOINT1 || pAddrInfo->dstEp == APP_ENDPOINT2) {
//        switch(cmdId){
//            case ZCL_CMD_LEVEL_MOVE_TO_LEVEL:
//                app_displayMoveToLevelProcess(pAddrInfo->dstEp, cmdId, (moveToLvl_t *)cmdPayload);
//                break;
//            default:
//                break;
//        }
//    }

    return ZCL_STA_SUCCESS;
}

