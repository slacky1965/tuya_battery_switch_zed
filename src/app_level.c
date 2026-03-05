#include "app_main.h"

void app_level_move(uint8_t ep, uint8_t up_down) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    move_t move;
    move.moveMode = up_down;
    move.rate = device_settings.defaultMoveRate[ep-1];
    move.optPresent = 0;

    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    uint16_t groupList[APS_GROUP_TABLE_NUM];
    uint8_t groupCnt = 0;
    aps_group_list_get(&groupCnt, groupList);

    /* command for groups */
    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
    for (uint8_t i = 0; i < groupCnt; i++) {
        dstEpInfo.dstAddr.shortAddr = groupList[i];
        zcl_level_moveWithOnOffCmd(ep, &dstEpInfo, FALSE, &move);
//        printf("groupAddr: 0x%04x\r\n", dstEpInfo.dstAddr.shortAddr);
    }

    /* command when binding */
    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;


    zcl_level_moveWithOnOffCmd(ep, &dstEpInfo, FALSE, &move);

    DEBUG(DEBUG_LEVEL_EN, "Ep: %d, Level move %s with rate: %d\r\n", ep, up_down?"Down":"Up", move.rate);
}

void app_level_stop(uint8_t ep) {

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    stop_t stop;
    stop.optPresent = 0;

    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    uint16_t groupList[APS_GROUP_TABLE_NUM];
    uint8_t groupCnt = 0;
    aps_group_list_get(&groupCnt, groupList);

    /* command for groups */
    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
    for (uint8_t i = 0; i < groupCnt; i++) {
        dstEpInfo.dstAddr.shortAddr = groupList[i];
        zcl_level_stopCmd(ep, &dstEpInfo, FALSE, &stop);
//        printf("groupAddr: 0x%04x\r\n", dstEpInfo.dstAddr.shortAddr);
    }

    /* command when binding */
    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;


    zcl_level_stopCmd(ep, &dstEpInfo, FALSE, &stop);

    DEBUG(DEBUG_LEVEL_EN, "Ep: %d, Level stop\r\n", ep);
}

void app_level_step(uint8_t ep, uint8_t up_down) {
    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);

    step_t step;
    step.stepMode = up_down;
    step.stepSize = 25;
    step.transitionTime = 1;
    step.optPresent = 0;

    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    uint16_t groupList[APS_GROUP_TABLE_NUM];
    uint8_t groupCnt = 0;
    aps_group_list_get(&groupCnt, groupList);

    /* command for groups */
    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
    for (uint8_t i = 0; i < groupCnt; i++) {
        dstEpInfo.dstAddr.shortAddr = groupList[i];
        zcl_level_stepWithOnOffCmd(ep, &dstEpInfo, FALSE, &step);
//        printf("groupAddr: 0x%04x\r\n", dstEpInfo.dstAddr.shortAddr);
    }

    /* command when binding */
    dstEpInfo.profileId = HA_PROFILE_ID;
    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;


    zcl_level_stepWithOnOffCmd(ep, &dstEpInfo, FALSE, &step);

    DEBUG(DEBUG_LEVEL_EN, "Ep: %d, Level step %s\r\n", ep, up_down?"Down":"Up");
}

