/********************************************************************************************************
 * @file    zb_appCb.c
 *
 * @brief   This is the source file for zb_appCb
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
#include "zb_api.h"
#include "zcl_include.h"
#include "bdb.h"
#include "ota.h"

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
void zb_bdbInitCb(u8 status, u8 joinedNetwork);
void zb_bdbCommissioningCb(u8 status, void *arg);
void zb_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime);
void zb_bdbFindBindSuccessCb(findBindDst_t *pDstInfo);


/**********************************************************************
 * LOCAL VARIABLES
 */
bdb_appCb_t g_zbBdbCb = {
    zb_bdbInitCb,
    zb_bdbCommissioningCb,
    zb_bdbIdentifyCb,
    zb_bdbFindBindSuccessCb
};

#ifdef ZCL_OTA
ota_callBack_t app_otaCb = {
    app_otaProcessMsgHandler,
};
#endif

static ev_timer_event_t *steerTimerEvt = NULL;
static ev_timer_event_t *rejoinBackoffTimerEvt = NULL;

/**********************************************************************
 * FUNCTIONS
 */
static s32 app_bdbNetworkSteerStart(void *arg)
{
    bdb_networkSteerStart();

    steerTimerEvt = NULL;
    return -1;
}

#if FIND_AND_BIND_SUPPORT
int32_t app_bdbFindAndBindStart(void *arg)
{
    _CODE_BDB_ u8 st = bdb_findAndBindStart(BDB_COMMISSIONING_ROLE_INITIATOR);
    APP_DEBUG(DEBUG_BDB_EN, "timer app_bdbFindAndBindStart. status from bdb_findAndBindStart: 0x%02x\r\n", st);

    if (st != BDB_STATE_IDLE) {
        return TIMEOUT_2p5SEC;
    }

    findbind->timerBdbFindBindEvt = NULL;
    return -1;
}
#endif

static s32 app_rejoinBackoff(void *arg)
{
    static bool rejoinMode = REJOIN_SECURITY;

    if (zb_isDeviceFactoryNew()) {
        rejoinBackoffTimerEvt = NULL;
        return -1;
    }

    //APP_DEBUG(DEBUG_ZB_CB_EN, "rejoin mode = %d\n", rejoinMode);

    zb_rejoinSecModeSet(rejoinMode);
    zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);

    rejoinMode = !rejoinMode;

    return 0;
}

/*********************************************************************
 * @fn      zb_bdbInitCb
 *
 * @brief   application callback for bdb initiation
 *
 * @param   status - the status of bdb init BDB_INIT_STATUS_SUCCESS or BDB_INIT_STATUS_FAILURE
 *
 * @param   joinedNetwork  - 1: node is on a network, 0: node isn't on a network
 *
 * @return  None
 */
void zb_bdbInitCb(u8 status, u8 joinedNetwork)
{
    //APP_DEBUG(DEBUG_ZB_CB_EN, "bdbInitCb: sta = %x, joined = %x\n", status, joinedNetwork);

    if (status == BDB_INIT_STATUS_SUCCESS) {
        /*
         * for non-factory-new device:
         * load zcl data from NV, start poll rate, start ota query, bdb_networkSteerStart
         *
         * for factory-new device:
         * steer a network
         *
         */
        if (joinedNetwork) {
            g_appCtx.net_steer_start = false;
            app_setPollRate(TIMEOUT_1MIN);

#ifdef ZCL_OTA
            ota_queryStart(APP_OTA_PERIODIC_QUERY_INTERVAL);
#endif

#ifdef ZCL_POLL_CTRL
            app_zclCheckInStart();
#endif
            if (findbind->timerGetIeeeCoordinatorEvt) {
                TL_ZB_TIMER_CANCEL(&(findbind->timerGetIeeeCoordinatorEvt));
            }
            findbind->timerGetIeeeCoordinatorEvt = TL_ZB_TIMER_SCHEDULE(app_getCoordinatorExtAddrCb, NULL, TIMEOUT_2SEC);
        } else if (g_appCtx.net_steer_start) {
            u16 jitter = 0;
            do {
                jitter = zb_random() % 0x0fff;
            } while(jitter == 0);

            if (steerTimerEvt) {
                TL_ZB_TIMER_CANCEL(&steerTimerEvt);
            }
            steerTimerEvt = TL_ZB_TIMER_SCHEDULE(app_bdbNetworkSteerStart, NULL, jitter);
        }
    } else {
        if (joinedNetwork) {
            //zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
            if (!rejoinBackoffTimerEvt) {
                rejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(app_rejoinBackoff, NULL, 60 * 1000);
            }
        }
    }
}

/*********************************************************************
 * @fn      zb_bdbCommissioningCb
 *
 * @brief   application callback for bdb commissioning
 *
 * @param   status - the status of bdb commissioning
 *
 * @param   arg
 *
 * @return  None
 */
void zb_bdbCommissioningCb(u8 status, void *arg)
{
    APP_DEBUG(DEBUG_ZB_CB_EN, "zb_bdbCommissioningCb: sta = %x\r\n", status);

    switch (status) {
    case BDB_COMMISSION_STA_SUCCESS:
        g_appCtx.net_steer_start = false;
//        light_blink_all_stop();
        light_blink_all_start(3, 30, 300);
//        TL_ZB_TIMER_SCHEDULE(lightTimerStopCb, (void*)((uint32_t)device->button_num), TIMEOUT_1p5SEC);
        if (findbind->timerClearFindBindFlagEvt) {
            stop_timerClearFindBindFlag();
        } else {
            app_setPollRate(TIMEOUT_1MIN);
        }

        if (steerTimerEvt) {
            TL_ZB_TIMER_CANCEL(&steerTimerEvt);
        }

        if (rejoinBackoffTimerEvt) {
            TL_ZB_TIMER_CANCEL(&rejoinBackoffTimerEvt);
        }

        if (!zb_isDeviceJoinedNwk()) {
            zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
        }

#ifdef ZCL_POLL_CTRL
        app_zclCheckInStart();
#endif
#ifdef ZCL_OTA
        ota_queryStart(APP_OTA_PERIODIC_QUERY_INTERVAL);
#endif
#if 0 //FIND_AND_BIND_SUPPORT
        //start Finding & Binding
        if (!g_switchAppCtx.bdbFBTimerEvt) {
            g_switchAppCtx.bdbFBTimerEvt = TL_ZB_TIMER_SCHEDULE(app_bdbFindAndBindStart, NULL, 50);
        }
#endif
        if (findbind->timerGetIeeeCoordinatorEvt) {
            TL_ZB_TIMER_CANCEL(&(findbind->timerGetIeeeCoordinatorEvt));
        }
        findbind->timerGetIeeeCoordinatorEvt = TL_ZB_TIMER_SCHEDULE(app_getCoordinatorExtAddrCb, NULL, TIMEOUT_2SEC);
        break;
    case BDB_COMMISSION_STA_IN_PROGRESS:
        break;
    case BDB_COMMISSION_STA_NOT_AA_CAPABLE:
        break;
    case BDB_COMMISSION_STA_NO_NETWORK:
    case BDB_COMMISSION_STA_TCLK_EX_FAILURE:
    case BDB_COMMISSION_STA_TARGET_FAILURE:
        if (g_appCtx.net_steer_start) {
            u16 jitter = 0;
            do {
                jitter = zb_random() % 0x0fff;
            } while(jitter == 0);

            if (steerTimerEvt) {
                TL_ZB_TIMER_CANCEL(&steerTimerEvt);
            }
            steerTimerEvt = TL_ZB_TIMER_SCHEDULE(app_bdbNetworkSteerStart, NULL, jitter);
        }
        break;
    case BDB_COMMISSION_STA_FORMATION_FAILURE:
        break;
    case BDB_COMMISSION_STA_NO_IDENTIFY_QUERY_RESPONSE:
//        light_blink_all_stop();
        light_blink_all_start(1, 30, 300);
        if (findbind->timerClearFindBindFlagEvt) {
            stop_timerClearFindBindFlag();
        }
        break;
    case BDB_COMMISSION_STA_BINDING_TABLE_FULL:
        break;
    case BDB_COMMISSION_STA_NO_SCAN_RESPONSE:
        break;
    case BDB_COMMISSION_STA_NOT_PERMITTED:
        break;
    case BDB_COMMISSION_STA_PARENT_LOST:
        zb_rejoinSecModeSet(REJOIN_SECURITY);
        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
        //zb_rejoinReqWithBackOff(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
        break;
    case BDB_COMMISSION_STA_REJOIN_FAILURE:
        if (!rejoinBackoffTimerEvt) {
            rejoinBackoffTimerEvt = TL_ZB_TIMER_SCHEDULE(app_rejoinBackoff, NULL, 60 * 1000);
        }
        break;
    default:
        break;
    }
}

void zb_bdbIdentifyCb(u8 endpoint, u16 srcAddr, u16 identifyTime)
{
#if FIND_AND_BIND_SUPPORT
    extern void app_zclIdentifyCmdHandler(u8 endpoint, u16 srcAddr, u16 identifyTime);

    app_zclIdentifyCmdHandler(endpoint, srcAddr, identifyTime);
#endif
}

/*********************************************************************
 * @fn      zb_bdbFindBindSuccessCb
 *
 * @brief   application callback for finding & binding
 *
 * @param   pDstInfo
 *
 * @return  None
 */
void zb_bdbFindBindSuccessCb(findBindDst_t *pDstInfo)
{
#if FIND_AND_BIND_SUPPORT
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    dstEpInfo.dstAddrMode = APS_SHORT_DSTADDR_WITHEP;
    dstEpInfo.dstAddr.shortAddr = pDstInfo->addr;
    dstEpInfo.dstEp = pDstInfo->endpoint;
    dstEpInfo.profileId = HA_PROFILE_ID;

    zcl_identify_identifyCmd(APP_ENDPOINT1, &dstEpInfo, FALSE, 0, 0);
#endif
}

#ifdef ZCL_OTA
void app_otaProcessMsgHandler(u8 evt, u8 status)
{
    //APP_DEBUG(DEBUG_ZB_CB_EN, "app_otaProcessMsgHandler: status = %x\n", status);
    if (evt == OTA_EVT_START) {
        if (status == ZCL_STA_SUCCESS) {
            DEBUG(DEBUG_OTA_EN, "OTA update start.\r\n");
            zb_setPollRate(QUEUE_POLL_RATE);
            g_appCtx.not_sleep = true;
            g_appCtx.ota = true;
            if (g_appCtx.timerSetPollRateEvt) {
                timerSetPollRate_stop();
//                TL_ZB_TIMER_CANCEL(&g_appCtx.timerSetPollRateEvt);
            }
//            light_blink_all_stop();
            light_blink_all_start(5, 100, 500);
        } else {

        }
    } else if (evt == OTA_EVT_COMPLETE) {
        app_setPollRate(TIMEOUT_20SEC);

        if (status == ZCL_STA_SUCCESS) {
            DEBUG(DEBUG_OTA_EN, "OTA update successful.\r\n");
            ota_mcuReboot();
        } else {
            DEBUG(DEBUG_OTA_EN, "OTA update failure. Try again.\r\n");
            ota_queryStart(OTA_PERIODIC_QUERY_INTERVAL);
        }
    } else if (evt == OTA_EVT_IMAGE_DONE) {
        app_setPollRate(TIMEOUT_20SEC);
    }
}
#endif

/*********************************************************************
 * @fn      app_leaveCnfHandler
 *
 * @brief   Handler for ZDO Leave Confirm message.
 *
 * @param   pRsp - parameter of leave confirm
 *
 * @return  None
 */
void app_leaveCnfHandler(nlme_leave_cnf_t *pLeaveCnf) {

    DEBUG(1, "app_leaveCnfHandler, status: 0x%04x\r\n", pLeaveCnf->status);

    if(pLeaveCnf->status == SUCCESS){
    	//SYSTEM_RESET();


        nv_sts_t device_settings_default();

        if (rejoinBackoffTimerEvt) {
            TL_ZB_TIMER_CANCEL(&rejoinBackoffTimerEvt);
        }

        zb_deviceFactoryNewSet(true);

        uint16_t jitter = 0;
        do {
            jitter = zb_random() % 0x0fff;
        } while (jitter == 0);

        if (steerTimerEvt) {
            TL_ZB_TIMER_CANCEL(&steerTimerEvt);
        }
        steerTimerEvt = TL_ZB_TIMER_SCHEDULE(app_bdbNetworkSteerStart, NULL, jitter);

//        if (!g_appCtx.net_steer_start) {
//            g_appCtx.net_steer_start = true;
//            TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
//            light_blink_start(90, 100, 1000);
//            app_setPollRate(TIMEOUT_2MIN);
//        }
    }
}

/*********************************************************************
 * @fn      app_leaveIndHandler
 *
 * @brief   Handler for ZDO leave indication message.
 *
 * @param   pInd - parameter of leave indication
 *
 * @return  None
 */
void app_leaveIndHandler(nlme_leave_ind_t *pLeaveInd)
{
    //APP_DEBUG(DEBUG_ZB_CB_EN, "app_leaveIndHandler, rejoin = %d\n", pLeaveInd->rejoin);
    //printfArray(pLeaveInd->device_address, 8);
}

/*********************************************************************
 * @fn      app_nwkStatusIndHandler
 *
 * @brief   Handler for NWK status indication message.
 *
 * @param   pInd - parameter of NWK status indication
 *
 * @return  None
 */
void app_nwkStatusIndHandler(zdo_nwk_status_ind_t *pNwkStatusInd)
{
    //APP_DEBUG(DEBUG_ZB_CB_EN, "nwkStatusIndHandler: addr = %x, status = %x\n", pNwkStatusInd->shortAddr, pNwkStatusInd->status);

    if (pNwkStatusInd->status == NWK_COMMAND_STATUS_BAD_FRAME_COUNTER) {
        tl_zb_normal_neighbor_entry_t *nbe = nwk_neTblGetByShortAddr(pNwkStatusInd->shortAddr);
        if (nbe) {
            //APP_DEBUG(DEBUG_ZB_CB_EN, "curFC = %d, rcvFC = %d, failCnt = %d\n", nbe->incomingFrameCnt, nbe->receivedFrameCnt, nbe->frameCounterFailCnt);
        }
    } else if (pNwkStatusInd->status == NWK_COMMAND_STATUS_BAD_KEY_SEQUENCE_NUMBER) {
        zb_rejoinSecModeSet(REJOIN_INSECURITY);
        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
    }
}
