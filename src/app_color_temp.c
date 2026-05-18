#include "app_main.h"


#define CMD_MOVE_TO_COLOR_TEMP      ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE
#define CMD_STOP_MOVE_STEP          ZCL_CMD_LIGHT_COLOR_CONTROL_STOP_MOVE_STEP
#define CMD_STEP_COLOR_TEMP         ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR_TEMPERATURE

static status_t st = 0xFF;

void app_color_move_to_temp(uint8_t ep, uint8_t up_down) {
    epInfo_t dstEpInfo;

    colorCtrlMove2CTCmd_t move2ColorTemp;
    move2ColorTemp.colorTemperature = up_down?COLOR_TEMPERATURE_COOLEST:COLOR_TEMPERATURE_WARMEST;
    move2ColorTemp.transitionTime = 25;
    move2ColorTemp.optPresent = 0;

    uint8_t dstEp = 0;

    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    uint16_t groupList[APS_GROUP_TABLE_NUM];
    uint8_t groupCnt = 0;
    aps_group_list_get(&groupCnt, groupList);

    /* command for groups */
    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
    for (uint8_t i = 0; i < groupCnt; i++) {
        aps_group_tbl_ent_t *grEntry = aps_group_search(groupList[i], ep);
        if (grEntry) {
            dstEpInfo.dstAddr.shortAddr = grEntry->group_addr;
            zcl_lightColorCtrl_move2colorTemperatureCmd(ep, &dstEpInfo, TRUE, &move2ColorTemp);
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "Color move %s for bind to temperature: %d src_ep: %d, dst_ep: %d, addr: 0x%04x, status: %d\r\n",
                    up_down?"Down":"Up", move2ColorTemp.colorTemperature, ep, grEntry->n_endpoints, grEntry->group_addr, st);
        }
    }

    /* command when binding */
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    aps_binding_entry_t *bind_tbl = bindTblEntryGet();
    for (uint8_t j = 0; j < APS_BINDING_TABLE_NUM; j++) {
        if (bind_tbl->used && bind_tbl->clusterId == ZCL_CLUSTER_LIGHTING_COLOR_CONTROL && bind_tbl->srcEp == ep) {
            dstEpInfo.dstAddrMode = bind_tbl->dstAddrMode;
            dstEpInfo.txOptions = APS_TX_OPT_ACK_TX;
            if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                dstEpInfo.dstAddr.shortAddr = bind_tbl->groupAddr;
                dstEp = 0;
            } else {
                dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
                dstEpInfo.dstEp = bind_tbl->dstExtAddrInfo.dstEp;
                memcpy(dstEpInfo.dstAddr.extAddr, bind_tbl->dstExtAddrInfo.extAddr, sizeof(extAddr_t));
                dstEp = bind_tbl->dstExtAddrInfo.dstEp;
            }
            app_add_repeat_cmd(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ep, dstEp, dstEpInfo.dstAddrMode, dstEpInfo.dstAddr, CMD_MOVE_TO_COLOR_TEMP, &move2ColorTemp);
            zcl_lightColorCtrl_move2colorTemperatureCmd(ep, &dstEpInfo, TRUE, &move2ColorTemp);
#if DEBUG_COLOR_CTRL_EN
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "Color move %s for bind to temperature: %d ep: %d, clId: 0x%04x, addrMode: %d - %s, ",
                    up_down?"Down":"Up", move2ColorTemp.colorTemperature, bind_tbl->srcEp, bind_tbl->clusterId, dstEpInfo.dstAddrMode,
                    (dstEpInfo.dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT)?"APS_DSTADDR_EP_NOTPRESETNT":
                    (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP)?"APS_SHORT_GROUPADDR_NOEP":
                    (dstEpInfo.dstAddrMode == APS_SHORT_DSTADDR_WITHEP)?"APS_SHORT_DSTADDR_WITHEP":"APS_LONG_DSTADDR_WITHEP");
            if (dstEpInfo.dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                        bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                        bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                        bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                        bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
            } else if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "groupAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            } else {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "shortAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            }
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "status: 0x%02x\r\n", st);
#endif
        }
        bind_tbl++;
    }
}

void app_color_stop_step(uint8_t ep) {

    epInfo_t dstEpInfo;

    colorCtrlStopCmd_t stop;
    stop.optPresent = 0;

    uint8_t dstEp = 0;

    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    uint16_t groupList[APS_GROUP_TABLE_NUM];
    uint8_t groupCnt = 0;
    aps_group_list_get(&groupCnt, groupList);

    /* command for groups */
    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
    for (uint8_t i = 0; i < groupCnt; i++) {
        aps_group_tbl_ent_t *grEntry = aps_group_search(groupList[i], ep);
        if (grEntry) {
            dstEpInfo.dstAddr.shortAddr = grEntry->group_addr;
            st = zcl_lightColorCtrl_stopMoveStepCmd(ep, &dstEpInfo, TRUE, &stop);
            APP_DEBUG(DEBUG_ONOFF_EN, "Color move stop in groups. src_ep: %d, dst_ep: %d, addr: 0x%04x, status: %d\r\n",
                    ep, grEntry->n_endpoints, grEntry->group_addr, st);
        }
    }

    /* command when binding */
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;
//    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
//    dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;

    aps_binding_entry_t *bind_tbl = bindTblEntryGet();
    for (uint8_t j = 0; j < APS_BINDING_TABLE_NUM; j++) {
        if (bind_tbl->used && bind_tbl->clusterId == ZCL_CLUSTER_LIGHTING_COLOR_CONTROL && bind_tbl->srcEp == ep) {
            dstEpInfo.dstAddrMode = bind_tbl->dstAddrMode;
            dstEpInfo.txOptions = APS_TX_OPT_ACK_TX;
            if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                dstEpInfo.dstAddr.shortAddr = bind_tbl->groupAddr;
                dstEp = 0;
            } else {
                dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
                dstEpInfo.dstEp = bind_tbl->dstExtAddrInfo.dstEp;
                memcpy(dstEpInfo.dstAddr.extAddr, bind_tbl->dstExtAddrInfo.extAddr, sizeof(extAddr_t));
                dstEp = bind_tbl->dstExtAddrInfo.dstEp;
            }
            app_add_repeat_cmd(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ep, dstEp, dstEpInfo.dstAddrMode, dstEpInfo.dstAddr, CMD_STOP_MOVE_STEP, &stop);
            st = zcl_lightColorCtrl_stopMoveStepCmd(ep, &dstEpInfo, TRUE, &stop);
#if DEBUG_COLOR_CTRL_EN
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "Color move stop for bind, ep: %d, clId: 0x%04x, addrMode: %d - %s, ",
                    bind_tbl->srcEp, bind_tbl->clusterId, dstEpInfo.dstAddrMode,
                    (dstEpInfo.dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT)?"APS_DSTADDR_EP_NOTPRESETNT":
                    (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP)?"APS_SHORT_GROUPADDR_NOEP":
                    (dstEpInfo.dstAddrMode == APS_SHORT_DSTADDR_WITHEP)?"APS_SHORT_DSTADDR_WITHEP":"APS_LONG_DSTADDR_WITHEP");
            if (dstEpInfo.dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                        bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                        bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                        bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                        bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
            } else if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "groupAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            } else {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "shortAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            }
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "status: 0x%02x\r\n", st);
#endif
        }
        bind_tbl++;
    }
}

void app_color_step_temp(uint8_t ep, uint8_t up_down) {
    epInfo_t dstEpInfo;
    colorCtrlStepCTCmd_t step;
    step.stepMode = up_down?COLOR_CTRL_STEP_MODE_DOWN:COLOR_CTRL_STEP_MODE_UP;
    step.stepSize = 87;
    step.transitionTime = 1;
    step.optPresent = 0;
    step.colorTempMinMireds = COLOR_TEMPERATURE_COOLEST;
    step.colorTempMaxMireds = COLOR_TEMPERATURE_WARMEST;

    uint8_t dstEp = 0;

    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    uint16_t groupList[APS_GROUP_TABLE_NUM];
    uint8_t groupCnt = 0;
    aps_group_list_get(&groupCnt, groupList);

    /* command for groups */
    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
    for (uint8_t i = 0; i < groupCnt; i++) {
        aps_group_tbl_ent_t *grEntry = aps_group_search(groupList[i], ep);
        if (grEntry) {
            dstEpInfo.dstAddr.shortAddr = grEntry->group_addr;
            st = zcl_lightColorCtrl_stepColorTemperatureCmd(ep, &dstEpInfo, TRUE, &step);
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "Color step %s for bind with size: %d, src_ep: %d, dst_ep: %d, addr: 0x%04x, status: %d\r\n",
                    up_down?"Down":"Up", step.stepSize, ep, grEntry->n_endpoints, grEntry->group_addr, st);
        }
    }

    /* command when binding */
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    aps_binding_entry_t *bind_tbl = bindTblEntryGet();
    for (uint8_t j = 0; j < APS_BINDING_TABLE_NUM; j++) {
        if (bind_tbl->used && bind_tbl->clusterId == ZCL_CLUSTER_LIGHTING_COLOR_CONTROL && bind_tbl->srcEp == ep) {
            dstEpInfo.dstAddrMode = bind_tbl->dstAddrMode;
            dstEpInfo.txOptions = APS_TX_OPT_ACK_TX;
            if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                dstEpInfo.txOptions = APS_TX_OPT_ACK_TX;
                dstEpInfo.dstAddr.shortAddr = bind_tbl->groupAddr;
                dstEp = 0;
            } else {
                dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
                dstEpInfo.dstEp = bind_tbl->dstExtAddrInfo.dstEp;
                memcpy(dstEpInfo.dstAddr.extAddr, bind_tbl->dstExtAddrInfo.extAddr, sizeof(extAddr_t));
                dstEp = bind_tbl->dstExtAddrInfo.dstEp;
            }
            app_add_repeat_cmd(ZCL_CLUSTER_LIGHTING_COLOR_CONTROL, ep, dstEp, dstEpInfo.dstAddrMode, dstEpInfo.dstAddr, CMD_STEP_COLOR_TEMP, &step);
            st = zcl_lightColorCtrl_stepColorTemperatureCmd(ep, &dstEpInfo, TRUE, &step);
#if DEBUG_COLOR_CTRL_EN
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "Color step %s for bind with size: %d, ep: %d, clId: 0x%04x, addrMode: %d - %s, ",
                    up_down?"Down":"Up", step.stepSize, bind_tbl->srcEp, bind_tbl->clusterId, dstEpInfo.dstAddrMode,
                    (dstEpInfo.dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT)?"APS_DSTADDR_EP_NOTPRESETNT":
                    (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP)?"APS_SHORT_GROUPADDR_NOEP":
                    (dstEpInfo.dstAddrMode == APS_SHORT_DSTADDR_WITHEP)?"APS_SHORT_DSTADDR_WITHEP":"APS_LONG_DSTADDR_WITHEP");
            if (dstEpInfo.dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                        bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                        bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                        bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                        bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
            } else if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "groupAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            } else {
                APP_DEBUG(DEBUG_COLOR_CTRL_EN, "shortAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            }
            APP_DEBUG(DEBUG_COLOR_CTRL_EN, "status: 0x%02x\r\n", st);
#endif
        }
        bind_tbl++;
    }
}

int32_t app_repeatCmdColorCtrl(void *args) {

    APP_DEBUG(DEBUG_REPEAT_EN, "app_repeatCmdColorCtrl()\r\n");
    repeat_cmd_t *r_cmd = (repeat_cmd_t*)args;
    if (!r_cmd) return -1;

    epInfo_t dstEpInfo;
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;

    dstEpInfo.dstAddrMode = r_cmd->dstAddrMode;
    if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
        dstEpInfo.dstAddr.shortAddr = r_cmd->dstAddr.shortAddr;
    } else {
        dstEpInfo.dstEp = r_cmd->dstEp;
        memcpy(dstEpInfo.dstAddr.extAddr, r_cmd->dstAddr.extAddr, sizeof(extAddr_t));
    }

    switch(r_cmd->cmdId) {
        case ZCL_CMD_LIGHT_COLOR_CONTROL_MOVE_TO_COLOR_TEMPERATURE:
            zcl_lightColorCtrl_move2colorTemperature(r_cmd->srcEp, &dstEpInfo, FALSE, ZCL_SEQ_NUM, &r_cmd->move2ColorTemp);
            break;
        case ZCL_CMD_LIGHT_COLOR_CONTROL_STOP_MOVE_STEP:
            zcl_lightColorCtrl_stopMoveStep(r_cmd->srcEp, &dstEpInfo, FALSE, ZCL_SEQ_NUM, &r_cmd->stopMoveStep);
            break;
        case ZCL_CMD_LIGHT_COLOR_CONTROL_STEP_COLOR_TEMPERATURE:
            zcl_lightColorCtrl_stepColorTemperature(r_cmd->srcEp, &dstEpInfo, FALSE, ZCL_SEQ_NUM, &r_cmd->stepColorTemp);
            break;
        default:
            break;
    }

    return -1;
}
