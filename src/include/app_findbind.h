#ifndef SRC_INCLUDE_APP_FINDBIND_H_
#define SRC_INCLUDE_APP_FINDBIND_H_

typedef struct {
    ev_timer_event_t *timerBdbFindBindEvt;
    ev_timer_event_t *timerClearFindBindFlagEvt;
    ev_timer_event_t *timerGetIeeeCoordinatorEvt;
    bool      addrCoordinatorSet;
    addrExt_t extAddrCoordinator;
    bool      find_bind_flag;
    uint8_t   find_bind_src_ep;
    uint8_t   find_bind_dst_ep;

} app_findbind_t;

extern app_findbind_t *findbind;

int32_t app_getCoordinatorExtAddrCb(void *args);
void app_findBindStart(uint8_t i);
void stop_timerClearFindBindFlag();

#endif /* SRC_INCLUDE_APP_FINDBIND_H_ */
