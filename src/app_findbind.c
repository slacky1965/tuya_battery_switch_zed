#include "app_main.h"

static app_findbind_t app_findbind = {
    .timerBdbFindBindEvt = NULL,
    .timerClearFindBindFlagEvt = NULL,
    .timerGetIeeeCoordinatorEvt = NULL,
    .addrCoordinatorSet = false,
    .extAddrCoordinator = {0},
    .find_bind_flag = false,
    .find_bind_dst_ep = APP_ENDPOINT1,
    .find_bind_src_ep = APP_ENDPOINT1,
};

app_findbind_t *findbind = &app_findbind;

static void app_getIeeeCb(void *args) {
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)args;
    zdo_ieee_addr_resp_t *rsp = (zdo_ieee_addr_resp_t*)p->zpdu;

    if (rsp->status == ZDO_SUCCESS) {
        ZB_IEEE_ADDR_COPY(findbind->extAddrCoordinator, rsp->ieee_addr_remote);
        findbind->addrCoordinatorSet = true;
    }
}

static int32_t app_clearFindBindFlagCb(void *args) {

    APP_DEBUG(DEBUG_BUTTON_EN, "clearFindBindFlagCb\r\n");
    findbind->find_bind_flag = false;
    if (/*!g_appCtx.timerSetPollRateEvt &&*/ !g_appCtx.ota) {
        g_appCtx.not_sleep = false;
    }
    findbind->timerClearFindBindFlagEvt = NULL;
    return -1;
}


void app_findBindStart(uint8_t i) {

    uint8_t endpoint = i+1;
    light_blink_stop(i);
    if (!findbind->timerBdbFindBindEvt) {
        aps_binding_entry_t *bind_tbl = aps_bindingTblEntryGet();
        for (uint8_t i = 0; i < APS_BINDING_TABLE_NUM; i++) {
            if (bind_tbl->used && bind_tbl->srcEp == endpoint && bind_tbl->clusterId == ZCL_CLUSTER_GEN_ON_OFF) {
                if (memcmp(bind_tbl->dstExtAddrInfo.extAddr, findbind->extAddrCoordinator, 8)) {
                    APP_DEBUG(DEBUG_FINDBIND_EN, "Deleted src_ep: %d, dst_ep: %d, ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x\r\n",
                            bind_tbl->srcEp, bind_tbl->dstExtAddrInfo.dstEp,
                            bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                            bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                            bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                            bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
                    bind_tbl->used = false;
                }
            }
            bind_tbl++;
        }
        findbind->find_bind_src_ep = endpoint;
        findbind->find_bind_flag = true;
        findbind->timerBdbFindBindEvt = TL_ZB_TIMER_SCHEDULE(app_bdbFindAndBindStart, NULL, TIMEOUT_50MS);
        if (!findbind->timerClearFindBindFlagEvt) {
            TL_ZB_TIMER_CANCEL(&(findbind->timerClearFindBindFlagEvt));
        }
        findbind->timerClearFindBindFlagEvt = TL_ZB_TIMER_SCHEDULE(app_clearFindBindFlagCb, NULL, TIMEOUT_3MIN);
        light_blink_all_start(90, 30, 750);
    }

}



static void app_getIeee(uint16_t dstAddr) {
    zdo_ieee_addr_req_t pReq;
    u8 sn = 0;
    pReq.nwk_addr_interest = dstAddr;
    pReq.req_type = ZDO_ADDR_REQ_SINGLE_RESP;
    pReq.start_index = 0;
    zb_zdoIeeeAddrReq(dstAddr, &pReq, &sn, app_getIeeeCb);
}

int32_t app_getCoordinatorExtAddrCb(void *args) {

//    printf("app_bindTimerCb: network: %d\r\n", zb_isDeviceJoinedNwk());
    if (zb_isDeviceJoinedNwk()) {
        if (!findbind->addrCoordinatorSet) {
            app_getIeee(0x0000);
            return 0;
        }
    }

    return -1;
}

void stop_timerClearFindBindFlag() {
    APP_DEBUG(DEBUG_FINDBIND_EN, "stop_timerClearFindBindFlag\r\n");

    for (uint8_t i = 0; i < 16 && findbind->timerClearFindBindFlagEvt; i++) {
        if (TL_ZB_TIMER_CANCEL(&(findbind->timerClearFindBindFlagEvt)) == NO_TIMER_AVAIL) {
            findbind->timerClearFindBindFlagEvt = NULL;
        }
    }
    findbind->find_bind_flag = false;
    if (/*!g_appCtx.timerSetPollRateEvt &&*/ !g_appCtx.ota) {
        g_appCtx.not_sleep = false;
    }
}

