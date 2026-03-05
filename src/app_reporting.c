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

        status_t ret = 0;
        dstEpInfo.profileId = HA_PROFILE_ID;
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;

        zclAttrInfo_t *pAttrEntry = zcl_findAttribute(endpoint, claster_id, attr_id);

        if (!pAttrEntry) {
            //should not happen.
            ZB_EXCEPTION_POST(SYS_EXCEPTTION_ZB_ZCL_ENTRY);
            return;
        }

        ret = zcl_sendReportCmd(endpoint, &dstEpInfo,  TRUE, ZCL_FRAME_SERVER_CLIENT_DIR,
                    claster_id, pAttrEntry->id, pAttrEntry->type, pAttrEntry->data);

        DEBUG(DEBUG_REPORTING_EN, "ret: %d, forceReportCb. endpoint: 0x%x, claster_id: 0x%x, attr_id: 0x%x, data: 0x%08x\r\n", ret, endpoint, claster_id, attr_id, *pAttrEntry->data);
    }


}

