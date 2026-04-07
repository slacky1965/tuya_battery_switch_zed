#include "app_main.h"

void app_scene_send(uint8_t ep) {

    status_t st;
    uint16_t addrGroup = device_settings.scene[ep-1].groupId;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;
    recallScene_t recallScene;

    recallScene.sceneId = device_settings.scene[ep-1].sceneId;
    recallScene.groupId = device_settings.scene[ep-1].groupId;

    recallScene.transTime = 0;

    if (addrGroup) {
        dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
        dstEpInfo.dstAddr.shortAddr = addrGroup;
        st = zcl_scene_recallSceneCmd(ep, &dstEpInfo, FALSE, &recallScene);
        APP_DEBUG(DEBUG_SCENE_EN, "Recall scene command, status: 0x%02x,  ep: %d, sceneID: %d, group: %d\r\n", st, ep, recallScene.sceneId, recallScene.groupId);
    } else {
        aps_binding_entry_t *bind_tbl = bindTblEntryGet();
        for (uint8_t j = 0; j < APS_BINDING_TABLE_NUM; j++) {
            if (bind_tbl->used && bind_tbl->clusterId == ZCL_CLUSTER_GEN_SCENES && bind_tbl->srcEp == ep) {
                dstEpInfo.dstAddrMode = bind_tbl->dstAddrMode;
                if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                    dstEpInfo.dstAddr.shortAddr = bind_tbl->groupAddr;
                } else {
                    dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
                    dstEpInfo.dstEp = bind_tbl->dstExtAddrInfo.dstEp;
                    memcpy(dstEpInfo.dstAddr.extAddr, bind_tbl->dstExtAddrInfo.extAddr, sizeof(extAddr_t));
                }
                st = zcl_scene_recallSceneCmd(ep, &dstEpInfo, FALSE, &recallScene);
#if UART_PRINTF_MODE && DEBUG_SCENE_EN
                APP_DEBUG(DEBUG_SCENE_EN, "Recall scene command, ep: %d, clId: 0x%04x, addrMode: %d - %s, ",
                         bind_tbl->srcEp, bind_tbl->clusterId, dstEpInfo.dstAddrMode,
                        (dstEpInfo.dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT)?"APS_DSTADDR_EP_NOTPRESETNT":
                        (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP)?"APS_SHORT_GROUPADDR_NOEP":
                        (dstEpInfo.dstAddrMode == APS_SHORT_DSTADDR_WITHEP)?"APS_SHORT_DSTADDR_WITHEP":"APS_LONG_DSTADDR_WITHEP");
                if (dstEpInfo.dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
                    APP_DEBUG(DEBUG_SCENE_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                        bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                        bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                        bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                        bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
                } else if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                APP_DEBUG(DEBUG_SCENE_EN, "groupAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
                } else {
                    APP_DEBUG(DEBUG_SCENE_EN, "shortAddr: 0x%04x, ", dstEpInfo.dstAddr.shortAddr);
                }
                APP_DEBUG(DEBUG_SCENE_EN, "status: 0x%02x\r\n", st);
#endif
            }
            bind_tbl++;
        }
    }
}
