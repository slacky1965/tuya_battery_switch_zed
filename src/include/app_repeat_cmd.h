#ifndef SRC_INCLUDE_APP_REPEAT_CMD_H_
#define SRC_INCLUDE_APP_REPEAT_CMD_H_

#define REPEAT_CMD_NUM   APS_BINDING_TABLE_NUM

typedef struct {
    uint8_t         used;
    uint8_t         srcEp;
    uint8_t         dstEp;
    uint16_t        clId;
    uint8_t         cmdId;
    uint8_t         dstAddrMode;
    tl_zb_addr_t    dstAddr;
    moveToLvl_t     move2Level;
    move_t          move;
    stop_t          stop;
    step_t          step;
    recallScene_t   recallScene;
} repeat_cmd_t;

extern uint8_t repeat_cmd_num;

bool app_add_repeat_cmd(uint16_t clId, uint8_t srcEp, uint8_t dstEp, uint8_t addrMode, tl_zb_addr_t addr, uint8_t cmdId, void *args);
void app_del_repeat_cmd(uint16_t clId, uint8_t srcEp, uint8_t dstEp, uint8_t addrMode, tl_zb_addr_t addr);
repeat_cmd_t *app_find_repeat_cmd(uint16_t clId, uint8_t srcEp, uint8_t dstEp, uint8_t addrMode, tl_zb_addr_t *addr);
void app_reset_repeat_cmd();
void app_timerRepeatCmdNumClearStop();

#endif /* SRC_INCLUDE_APP_REPEAT_CMD_H_ */
