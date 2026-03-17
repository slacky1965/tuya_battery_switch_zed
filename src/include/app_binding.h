#ifndef SRC_INCLUDE_APP_BINDING_H_
#define SRC_INCLUDE_APP_BINDING_H_

typedef struct {
    ev_timer_event_t *timerGetIeeeCoordinatorEvt;
    bool    addrCoordinatorSet;
    addrExt_t  extAddrCoordinator;
    bool bind_onoff[DEVICE_BUTTON_MAX];
    bool bind_multi[DEVICE_BUTTON_MAX];
    bool bind_level[DEVICE_BUTTON_MAX];
    bool bind_scene[DEVICE_BUTTON_MAX];
} app_bind_t;

app_bind_t *g_app_bind;

void app_getIeeeCoordinator();
void app_update_bind(void *args);

#endif /* SRC_INCLUDE_APP_BINDING_H_ */
