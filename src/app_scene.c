#include "app_main.h"

void app_scene_send(uint8_t ep) {

    uint16_t addrGroup = device_settings.scene[ep-1].groupId;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    if (addrGroup) {
        dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
        dstEpInfo.dstAddr.shortAddr = addrGroup;
    } else {
        dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
    }
    recallScene_t recallScene;

    recallScene.sceneId = device_settings.scene[ep-1].sceneId;
    recallScene.groupId = device_settings.scene[ep-1].groupId;

    recallScene.transTime = 0;

    zcl_scene_recallSceneCmd(ep, &dstEpInfo, FALSE, &recallScene);

    DEBUG(DEBUG_SCENE_EN, "Recall scene command EP: %d, sceneID: %d, groupID: %d\r\n", ep, recallScene.sceneId, recallScene.groupId);
}
