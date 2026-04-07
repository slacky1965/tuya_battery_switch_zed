#include "app_main.h"

static status_t cmdOnOffSend(uint8_t ep, epInfo_t *dstEpInfo, uint8_t command) {

    status_t st = 0xFF;

    /* command 0x00 - off, 0x01 - on, 0x02 - toggle */
    switch(command) {
        case ZCL_CMD_ONOFF_OFF:
//            APP_DEBUG(DEBUG_ONOFF_EN, "OnOff command: off\r\n");
            st = zcl_onOff_offCmd(ep, dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_ON:
//            APP_DEBUG(DEBUG_ONOFF_EN, "OnOff command: on\r\n");
            st = zcl_onOff_onCmd(ep, dstEpInfo, FALSE);
            break;
        case ZCL_CMD_ONOFF_TOGGLE:
//            APP_DEBUG(DEBUG_ONOFF_EN, "OnOff command: toggle\r\n");
            st = zcl_onOff_toggleCmd(ep, dstEpInfo, FALSE);
            break;
        default:
            APP_DEBUG(DEBUG_ONOFF_EN, "OnOff command: unknown\r\n");
            break;
    }
//    APP_DEBUG(DEBUG_ONOFF_EN, ", status: 0x%02x\r\n", st);
    return st;
}

void app_cmdOnOff(uint8_t ep, uint8_t command) {

    status_t st;
    epInfo_t dstEpInfo;
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
            st = cmdOnOffSend(ep, &dstEpInfo, command);
            APP_DEBUG(DEBUG_ONOFF_EN, "OnOff in groups. cmd: %d, src_ep: %d, dst_ep: %d, addr: 0x%04x, status: %d\r\n",
                    (command == 0)?"Off":(command == 1)?"On":"Toggle", ep, grEntry->n_endpoints, grEntry->group_addr, st);
        }
    }

    /* command when binding */
    TL_SETSTRUCTCONTENT(dstEpInfo, 0);
    dstEpInfo.profileId = HA_PROFILE_ID;
//    dstEpInfo.dstAddrMode = APS_DSTADDR_EP_NOTPRESETNT;
//    dstEpInfo.dstAddrMode = APS_SHORT_GROUPADDR_NOEP;
//    dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;

    aps_binding_entry_t *bind_tbl = bindTblEntryGet();
    for (uint8_t j = 0; j < APS_BINDING_TABLE_NUM; j++) {
        if (bind_tbl->used && bind_tbl->clusterId == ZCL_CLUSTER_GEN_ON_OFF && bind_tbl->srcEp == ep) {
            dstEpInfo.dstAddrMode = bind_tbl->dstAddrMode;
            if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                dstEpInfo.dstAddr.shortAddr = bind_tbl->groupAddr;
            } else {
                dstEpInfo.dstAddrMode = APS_LONG_DSTADDR_WITHEP;
                dstEpInfo.dstEp = bind_tbl->dstExtAddrInfo.dstEp;
                memcpy(dstEpInfo.dstAddr.extAddr, bind_tbl->dstExtAddrInfo.extAddr, sizeof(extAddr_t));
            }
            st = cmdOnOffSend(ep, &dstEpInfo, command);
#if UART_PRINTF_MODE && DEBUG_ONOFF_EN
            APP_DEBUG(DEBUG_ONOFF_EN, "OnOff for bind. cmd: %s, src_ep: %d, dst_ep: %d, clId: 0x%04x, addrMode: %d - %s, ",
                    (command == 0)?"Off":(command == 1)?"On":"Toggle",
                     bind_tbl->srcEp, bind_tbl->dstExtAddrInfo.dstEp, bind_tbl->clusterId, dstEpInfo.dstAddrMode,
                    (dstEpInfo.dstAddrMode == APS_DSTADDR_EP_NOTPRESETNT)?"APS_DSTADDR_EP_NOTPRESETNT":
                    (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP)?"APS_SHORT_GROUPADDR_NOEP":
                    (dstEpInfo.dstAddrMode == APS_SHORT_DSTADDR_WITHEP)?"APS_SHORT_DSTADDR_WITHEP":"APS_LONG_DSTADDR_WITHEP");
            if (dstEpInfo.dstAddrMode == APS_LONG_DSTADDR_WITHEP) {
                APP_DEBUG(DEBUG_ONOFF_EN, "ieee: 0x%02x%02x%02x%02x%02x%02x%02x%02x, ",
                        bind_tbl->dstExtAddrInfo.extAddr[0], bind_tbl->dstExtAddrInfo.extAddr[1],
                        bind_tbl->dstExtAddrInfo.extAddr[2], bind_tbl->dstExtAddrInfo.extAddr[3],
                        bind_tbl->dstExtAddrInfo.extAddr[4], bind_tbl->dstExtAddrInfo.extAddr[5],
                        bind_tbl->dstExtAddrInfo.extAddr[6], bind_tbl->dstExtAddrInfo.extAddr[7]);
            } else if (dstEpInfo.dstAddrMode == APS_SHORT_GROUPADDR_NOEP) {
                APP_DEBUG(DEBUG_ONOFF_EN, "groupAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            } else {
                APP_DEBUG(DEBUG_ONOFF_EN, "shortAddr: 0x%04x, ",
                        dstEpInfo.dstAddr.shortAddr);
            }
            APP_DEBUG(DEBUG_ONOFF_EN, "status: 0x%02x\r\n", st);
#endif
        }
        bind_tbl++;
    }
}

