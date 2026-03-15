#include "app_main.h"

//void forceReportBattery(void *args) {
//
//    DEBUG(DEBUG_BATTERY_EN, "forceReportBattery()\r\n");
//
//    struct report_t {
//        u8 numAttr;
//        zclReport_t attr[2];
//    };
//
//    struct report_t report;
//
//    epInfo_t dstEpInfo;
//    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
//
//    dstEpInfo.profileId = HA_PROFILE_ID;
//    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
//
//    report.numAttr = 0;
//
//    zclAttrInfo_t *pAttrEntry;
//
//    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_VOLTAGE);
//
//    if (pAttrEntry) {
//        report.attr[report.numAttr].attrID = pAttrEntry->id;
//        report.attr[report.numAttr].dataType = pAttrEntry->type;
//        report.attr[report.numAttr].attrData = pAttrEntry->data;
//        report.numAttr++;
//    }
//
//    pAttrEntry = zcl_findAttribute(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
//
//    if (pAttrEntry) {
//        report.attr[report.numAttr].attrID = pAttrEntry->id;
//        report.attr[report.numAttr].dataType = pAttrEntry->type;
//        report.attr[report.numAttr].attrData = pAttrEntry->data;
//        report.numAttr++;
//    }
//
//    if (report.numAttr) {
//        zcl_sendReportAttrsCmd(APP_ENDPOINT1, &dstEpInfo, TRUE, ZCL_FRAME_SERVER_CLIENT_DIR, ZCL_CLUSTER_GEN_POWER_CFG, (zclReportCmd_t* )&report);
//    }
//}

void forceReportBattery(void *args) {

    app_forcedReport(APP_ENDPOINT1, ZCL_CLUSTER_GEN_POWER_CFG, ZCL_ATTRID_BATTERY_PERCENTAGE_REMAINING);
}

void app_forcedReport(uint8_t endpoint, uint16_t claster_id, uint16_t attr_id) {

    if (zb_isDeviceJoinedNwk()) {

        epInfo_t dstEpInfo;
        TL_SETSTRUCTCONTENT(dstEpInfo, 0);

        status_t st = 0;
        dstEpInfo.profileId = HA_PROFILE_ID;
//        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
//        dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
//        dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;

        zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, claster_id, attr_id);

        if (pAttrEntry) {

            aps_binding_entry_t *bind_tbl = bindTblEntryGet();
            for (uint8_t j = 0; j < APS_BINDING_TABLE_NUM; j++) {
                if (bind_tbl->used && bind_tbl->clusterId == claster_id && bind_tbl->srcEp == endpoint) {
                    dstEpInfo.dstAddrMode = bind_tbl->dstAddrMode;
                    if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                        dstEpInfo.dstAddr.shortAddr = bind_tbl->groupAddr;
                    } else {
                        dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
                        dstEpInfo.dstEp = bind_tbl->dstExtAddrInfo.dstEp;
                        memcpy(dstEpInfo.dstAddr.extAddr, bind_tbl->dstExtAddrInfo.extAddr, sizeof(extAddr_t));
                    }
                    st = zcl_sendReportCmd(endpoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                                claster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);
#if DEBUG_REPORTING_EN
                    APP_DEBUG(DEBUG_REPORTING_EN, "forceReportCb. Ep: %d, clId: 0x%04x, attr_id: 0x%04x, data: 0x%08x, addrMode: %d - %s, ",
                            bind_tbl->srcEp, bind_tbl->clusterId, attr_id, *pAttrEntry->data, dstEpInfo.dstAddrMode,
                            (dstEpInfo.dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT)?"APS_DSTADDR_EP_NOTPRESETNT":
                            (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP)?"APS_SHORT_GROUPADDR_NOEP":
                            (dstEpInfo.dstAddrMode == APS_SHORT_DSTADDR_WITHEP)?"APS_SHORT_DSTADDR_WITHEP":"APS_LONG_DSTADDR_WITHEP");
                    if (dstEpInfo.dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
                        APP_DEBUG(DEBUG_REPORTING_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                                bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                                bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                                bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                                bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
                    } else if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                        APP_DEBUG(DEBUG_REPORTING_EN, "groupAddr: 0x%04x, ", dstEpInfo.dstAddr.shortAddr);
                    } else {
                        APP_DEBUG(DEBUG_REPORTING_EN, "shortAddr: 0x%04x, ", dstEpInfo.dstAddr.shortAddr);
                    }
                    APP_DEBUG(DEBUG_REPORTING_EN, "status: 0x%02x\r\n", st);
#endif
                }
                bind_tbl++;
            }
        }
    }


}

