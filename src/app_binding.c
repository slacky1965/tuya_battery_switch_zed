#include "app_main.h"

static app_bind_t app_bind = {0};
app_bind_t *g_app_bind = &app_bind;

static void app_bind_req(uint8_t endpoint, uint16_t cluster) {

    if(zb_isDeviceJoinedNwk()) {

        zdo_bind_req_t req;
        memset(&req, 0, sizeof(zdo_bind_req_t));


        ZB_IEEE_ADDR_COPY(req.src_addr, g_app_bind->extAddrCoordinator);
        req.src_endpoint = 1;
        req.dst_endpoint = endpoint;

        req.cid16_l = cluster & 0xFF;
        req.cid16_h = (cluster >> 8) & 0xFF;
        req.dst_addr_mode = APS_LONG_DSTADDR_WITHEP;
        ZB_IEEE_ADDR_COPY(req.dst_ext_addr, NIB_IEEE_ADDRESS());

        uint8_t sn = 0;
        zb_zdoBindUnbindReq(0, &req, &sn, NULL);
    }
}

static void app_update_bind() {

    for (uint8_t endpoint = device->button_num; endpoint > 0; endpoint--) {
        app_bind_req(endpoint, ZCL_CLUSTER_GEN_ON_OFF);
        app_bind_req(endpoint, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC);
        app_bind_req(endpoint, ZCL_CLUSTER_GEN_LEVEL_CONTROL);
        app_bind_req(endpoint, ZCL_CLUSTER_GEN_SCENES);
    }
}

static void app_getIeeeCb(void *args) {
    zdo_zdpDataInd_t *p = (zdo_zdpDataInd_t *)args;
    zdo_ieee_addr_resp_t *rsp = (zdo_ieee_addr_resp_t*)p->zpdu;

    if (rsp->status == ZDO_SUCCESS) {
        ZB_IEEE_ADDR_COPY(g_app_bind->extAddrCoordinator, rsp->ieee_addr_remote);
        g_app_bind->addrCoordinatorSet = true;
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

static int32_t app_getIeeeCoordinatorCb(void *args) {

//    printf("app_bindTimerCb: network: %d\r\n", zb_isDeviceJoinedNwk());
    if (zb_isDeviceJoinedNwk()) {
        if (!g_app_bind->addrCoordinatorSet) {
            app_getIeee(0x0000);
        } else {
            APP_DEBUG(DEBUG_BINDING_EN, "extAddr coord: 0x%02x%02x%02x%02x%02x%02x%02x%02x\r\n",
                    g_app_bind->extAddrCoordinator[0], g_app_bind->extAddrCoordinator[1],
                    g_app_bind->extAddrCoordinator[2], g_app_bind->extAddrCoordinator[3],
                    g_app_bind->extAddrCoordinator[4], g_app_bind->extAddrCoordinator[5],
                    g_app_bind->extAddrCoordinator[6], g_app_bind->extAddrCoordinator[7]);
            TL_SCHEDULE_TASK(app_update_bind, NULL);
            g_app_bind->timerGetIeeeCoordinatorEvt = NULL;
            return -1;
        }
    }

    return 0;
}

void app_getIeeeCoordinator() {
    g_app_bind->addrCoordinatorSet = false;
    if (g_app_bind->timerGetIeeeCoordinatorEvt) {
        TL_ZB_TIMER_CANCEL(&g_app_bind->timerGetIeeeCoordinatorEvt);
    }
    g_app_bind->timerGetIeeeCoordinatorEvt = TL_ZB_TIMER_SCHEDULE(app_getIeeeCoordinatorCb, NULL, TIMEOUT_2SEC);

}

