#include "app_main.h"

#define FR_COUNTER_MAX      5      /* number for factory reset                     */
#define BATTERY_COUNTER     4

typedef struct {
    bool        released;
    bool        pressed;
    uint8_t     hold;
    bool        level_up;
    uint8_t     counter;
    uint8_t     debounce;
    uint32_t    pressed_time;
    uint32_t    released_time;
    uint32_t    hold_time;
} app_button_t;

enum {
    HOLD_NOT_PRESENT = 0,
    HOLD_PRESENT,
    HOLD_FIX
};

static ev_timer_event_t *timerClearSleepEvt = NULL;
static ev_timer_event_t *timerFactoryResetEvt = NULL;
static app_button_t app_button[DEVICE_BUTTON_MAX];
bool factory_reset = false;

static int32_t clearSleepCb(void *args) {

    APP_DEBUG(DEBUG_PM_EN, "clearSleepCb\r\n");

    if (!g_appCtx.timerSetPollRateEvt && !g_appCtx.ota) {
        g_appCtx.not_sleep = false;
    }

    timerClearSleepEvt = NULL;
    return -1;
}

static void clearSleepTimer() {
    if (timerClearSleepEvt) {
        TL_ZB_TIMER_CANCEL(&timerClearSleepEvt);
    }
    timerClearSleepEvt = TL_ZB_TIMER_SCHEDULE(clearSleepCb, NULL, TIMEOUT_100MS);
}

static int32_t start_factory_reset_lightCb(void *args) {

    light_blink_all_start(90, 250, 750);

    return -1;
}

static void button_factory_reset_start() {

    APP_DEBUG(DEBUG_BUTTON_EN, "button_factory_reset_start\r\n");

    factory_reset = false;

    zb_factoryReset();

    g_appCtx.net_steer_start = true;
    TL_ZB_TIMER_SCHEDULE(net_steer_start_offCb, NULL, TIMEOUT_1MIN30SEC);
    light_blink_all_stop();
    TL_ZB_TIMER_SCHEDULE(start_factory_reset_lightCb, NULL, TIMEOUT_500MS);
    app_setPollRate(TIMEOUT_2MIN);
}

static int32_t factoryResetCb(void *args) {

    APP_DEBUG(DEBUG_BUTTON_EN, "factoryResetCb\r\n");

    if (!g_appCtx.ota) g_appCtx.not_sleep = false;

    factory_reset = false;

    timerFactoryResetEvt = NULL;
    return -1;
}


static void read_button_level(uint8_t i) {
    uint8_t up_down = 0xFF;
    uint8_t cmdOnOff = 0xFF;
    app_button_t *button = &app_button[i];
    zcl_levelAttr_t *levelAttr = zcl_levelAttrsGet();
    levelAttr += i;

    switch(device_settings.switchType[i]) {
        case ZCL_CUSTOM_SWITCH_TYPE_LEVEL_MOVE_UP:
            up_down = LEVEL_MOVE_UP;
            cmdOnOff = ZCL_CMD_ONOFF_ON;
            break;
        case ZCL_CUSTOM_SWITCH_TYPE_LEVEL_MOVE_DOWN:
            up_down = LEVEL_MOVE_DOWN;
            cmdOnOff = ZCL_CMD_ONOFF_OFF;
            break;
        default:
            break;
    }

    if (!drv_gpio_read(device->button_gpio[i].gpio)) {
        if (button->pressed) {
            if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_750MS)) {
                if (button->hold == HOLD_NOT_PRESENT) {
                    button->hold = HOLD_PRESENT;
                    if (!factory_reset) {
                        APP_DEBUG(DEBUG_BUTTON_EN, "Level. Press and hold button: %d\r\n", i+1);
                        if (up_down != 0xFF) {
                            if (device->button_num == 1) {
                                if (!button->level_up) {
                                    up_down = LEVEL_MOVE_UP;
                                    button->level_up = true;
                                } else {
                                    up_down = LEVEL_MOVE_DOWN;
                                    button->level_up = false;
                                }
                            }
                            APP_DEBUG(DEBUG_BUTTON_EN, "Level. Key: %d, up_down: %d, button->level_up: %d\r\n", i+1, up_down, button->level_up);
                            app_level_move(i+1, up_down);
                        }
                    } else {
                        if (timerFactoryResetEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
                        }
                        button_factory_reset_start();
                    }
                }
            }
        }
        if (button->debounce != device->button_debounce) {
            button->debounce++;
            if (button->debounce == device->button_debounce) {
                button->pressed = true;
                g_appCtx.not_sleep = true;
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d pressed level\r\n", i+1);
                light_blink_start(1, 30, 1, i);
                if (!clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
                    button->counter++;
                } else {
                    button->counter = 1;
                    if (!zb_isDeviceJoinedNwk() && !zb_isDeviceFactoryNew()) {
                        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                    }
                }
                button->hold_time = button->pressed_time = clock_time();
            }
        }
    } else {
        if (button->debounce != 1) {
            button->debounce--;
            if (button->debounce == 1 && (button->pressed || button->hold == HOLD_FIX)) {
                button->released = true;
                g_appCtx.not_sleep = true;
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d released level\r\n", i+1);
            }
        }
    }

    if (button->released && clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
        if (button->counter >= FR_COUNTER_MAX) {
            APP_DEBUG(DEBUG_BUTTON_EN, "Reset Factory is ready from level\r\n");
            factory_reset = true;
            light_blink_stop(i);
            light_blink_start(1, 2000, 1, i);
            if (timerFactoryResetEvt) {
                TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
            }
            timerFactoryResetEvt = TL_ZB_TIMER_SCHEDULE(factoryResetCb, NULL, TIMEOUT_3SEC);
        } else {
            if (button->hold) {
                APP_DEBUG(DEBUG_BUTTON_EN, "Level. Released button: %d\r\n", i+1);
                app_level_stop(i+1);
            } else {
                APP_DEBUG(DEBUG_BUTTON_EN, "Level. Button %d press %d times\r\n", i+1, button->counter);
                switch(button->counter) {
                    case ACTION_SINGLE:                                         // 1
                        if (device->button_num == 1) {
                            cmdOnOff = ZCL_CMD_ONOFF_TOGGLE;
                        }
                        app_cmdOnOff(i+1, cmdOnOff);
                        break;
                    case ACTION_DOUBLE:                                         // 2
                        if (device->button_num != 1) {
                            app_level_step(i+1, up_down);
                        }
                        break;
                    case BATTERY_COUNTER:                                      // 4
                        batteryCb(NULL);
                        if (!g_appCtx.timerSetPollRateEvt) {
                            app_setPollRate(TIMEOUT_20SEC);
                        }
                        break;
                    default:
                        break;
                }

            }
            clearSleepTimer();
        }

        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->hold = false;
    } else if (button->pressed && button->counter == 1 && button->hold == HOLD_PRESENT) {
        button->hold = HOLD_FIX;
        button->counter = 0;
        button->pressed = false;
        clearSleepTimer();
    }
}

static void read_button_multifunction(uint8_t i) {
    bool report = false;
    app_button_t *button = &app_button[i];
    zcl_msInputAttr_t *msInputAttr = zcl_msInputAttrsGet();
    msInputAttr += i;

    if (!drv_gpio_read(device->button_gpio[i].gpio)) {
        if (button->pressed) {
            if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_750MS)) {
                if (button->hold == HOLD_NOT_PRESENT) {
                    button->hold = HOLD_PRESENT;
                    if (!factory_reset) {
                        APP_DEBUG(DEBUG_BUTTON_EN, "Multifunction. Press and hold button: %d\r\n", i+1);
                        msInputAttr->value = ACTION_HOLD;
//                        APP_DEBUG(DEBUG_REPORTING_EN, "MSI report ep: %d value %d\r\n", i+1, msInputAttr->value);
                        app_forcedReport(i+1, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC, ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE);
                    } else {
                        if (timerFactoryResetEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
                        }
                        button_factory_reset_start();
                    }
                }
            }
        }
        if (button->debounce != device->button_debounce) {
            button->debounce++;
            if (button->debounce == device->button_debounce) {
                button->pressed = true;
                g_appCtx.not_sleep = true;
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d pressed multifunction\r\n", i+1);
                light_blink_start(1, 30, 1, i);
                if (!clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
                    button->counter++;
                } else {
                    button->counter = 1;
                    if (!zb_isDeviceJoinedNwk() && !zb_isDeviceFactoryNew()) {
                        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                    }
                }
                button->hold_time = button->pressed_time = clock_time();
            }
        }
    } else {
        if (button->debounce != 1) {
            button->debounce--;
            if (button->debounce == 1 && (button->pressed || button->hold == HOLD_FIX)) {
                button->released = true;
                g_appCtx.not_sleep = true;
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d released multifunction\r\n", i+1);
            }
        }
    }

    if (button->released && clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
        if (button->counter >= FR_COUNTER_MAX) {
            APP_DEBUG(DEBUG_BUTTON_EN, "Reset Factory is ready from multifunction\r\n");
            factory_reset = true;
            light_blink_stop(i);
            light_blink_start(1, 2000, 1, i);
            if (timerFactoryResetEvt) {
                TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
            }
            timerFactoryResetEvt = TL_ZB_TIMER_SCHEDULE(factoryResetCb, NULL, TIMEOUT_3SEC);
        } else {
            if (button->hold) {
                msInputAttr->value = ACTION_RELEASE;
                report = true;
                APP_DEBUG(DEBUG_BUTTON_EN, "Multifunction. Released button: %d\r\n", i+1);
            } else {
                APP_DEBUG(DEBUG_BUTTON_EN, "Multifunction. Button %d press %d times\r\n", i+1, button->counter);
                switch(button->counter) {
                    case ACTION_SINGLE:                                         // 1
                    case ACTION_DOUBLE:                                         // 2
                    case ACTION_TRIPLE:                                         // 3
                        msInputAttr->value = button->counter;
                        report = true;
                        break;
                    case BATTERY_COUNTER:
                        batteryCb(NULL);
                        if (!g_appCtx.timerSetPollRateEvt) {
                            app_setPollRate(TIMEOUT_20SEC);
                        }
                        break;
                    default:
                        break;
                }

            }
            if (report) {
//                APP_DEBUG(DEBUG_REPORTING_EN, "MSI report ep: %d value %d\r\n", i+1, msInputAttr->value);
                app_forcedReport(i+1, ZCL_CLUSTER_GEN_MULTISTATE_INPUT_BASIC, ZCL_MULTISTATE_INPUT_ATTRID_PRESENT_VALUE);
            }
            clearSleepTimer();
        }

        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->hold = HOLD_NOT_PRESENT;
    } else if (button->pressed && button->counter == 1 && button->hold == HOLD_PRESENT) {
        button->hold = HOLD_FIX;
        button->counter = 0;
        button->pressed = false;
        clearSleepTimer();
    }
}

static void read_button_scene(uint8_t i) {
    app_button_t *button = &app_button[i];

    if (!drv_gpio_read(device->button_gpio[i].gpio)) {
        if (button->pressed) {
            if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_750MS)) {
                if (!button->hold) {
                    button->hold = true;
                    APP_DEBUG(DEBUG_BUTTON_EN, "Scene. Press and hold button: %d\r\n", i+1);
                    if (factory_reset) {
                        if (timerFactoryResetEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
                        }
                        button_factory_reset_start();
                    }
                }
            }
        }
        if (button->debounce != device->button_debounce) {
            button->debounce++;
            if (button->debounce == device->button_debounce) {
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d pressed scene\r\n", i+1);
                light_blink_start(1, 30, 1, i);
                if (button->counter == 0) {
                    button->counter++;
                    button->pressed = true;
                    g_appCtx.not_sleep = true;
                    if(zb_isDeviceJoinedNwk()) {
                        app_scene_send(i+1);
                    } else if (!zb_isDeviceFactoryNew()) {
                        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                    }
                } else if (button->pressed && !clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
                    button->counter++;
                    if (button->counter >= FR_COUNTER_MAX) {
                        APP_DEBUG(DEBUG_BUTTON_EN, "Reset Factory is ready from scene\r\n");
                        g_appCtx.not_sleep = true;
                        factory_reset = true;
                        light_blink_stop(i);
                        light_blink_start(1, 2000, 1, i);
                        if (timerFactoryResetEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
                        }
                        timerFactoryResetEvt = TL_ZB_TIMER_SCHEDULE(factoryResetCb, NULL, TIMEOUT_3SEC);
                        button->counter = 0;
                    }
                }
                button->hold_time = button->pressed_time = clock_time();
            }
        }
    } else {
        if (button->debounce != 1) {
            button->debounce--;
            if (button->debounce == 1 && button->pressed) {
                button->released = true;
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d released scene\r\n", i+1);
            }
        }
    }

    if (button->released && clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
        APP_DEBUG(DEBUG_BUTTON_EN, "Scene. Button %d pressed %d times\r\n", i+1, button->counter);
        if (button->counter == BATTERY_COUNTER) {
            batteryCb(NULL);
            if (!g_appCtx.timerSetPollRateEvt) {
                app_setPollRate(TIMEOUT_20SEC);
            }
        }
        clearSleepTimer();
        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->hold = false;
    }
}

static void read_button_toggle(uint8_t i) {
    uint8_t cmd_onoff;

    app_button_t *button = &app_button[i];

    if (!drv_gpio_read(device->button_gpio[i].gpio)) {
        if (button->pressed) {
            if (clock_time_exceed(button->hold_time, TIMEOUT_TICK_750MS)) {
                if (button->hold == HOLD_NOT_PRESENT) {
                    button->hold = HOLD_PRESENT;
                    APP_DEBUG(DEBUG_BUTTON_EN, "Toggle. Press and hold button: %d\r\n", i+1);
                    if (factory_reset) {
                        if (timerFactoryResetEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
                        }
                        button_factory_reset_start();
                    }
                }
            }
        }
        if (button->debounce != device->button_debounce) {
            button->debounce++;
            if (button->debounce == device->button_debounce) {
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d pressed toggle\r\n", i+1);
                light_blink_start(1, 30, 1, i);
                if (button->counter == 0) {
                    button->counter++;
                    button->pressed = true;
                    g_appCtx.not_sleep = true;
                    if(zb_isDeviceJoinedNwk()) {
                        cmd_onoff = ZCL_CMD_ONOFF_ON;
                        switch(device_settings.switchActions[i]) {
                            case ZCL_SWITCH_ACTION_OFF_ON:
                                cmd_onoff = ZCL_CMD_ONOFF_ON;
                                break;
                            case ZCL_SWITCH_ACTION_ON_OFF:
                                cmd_onoff = ZCL_CMD_ONOFF_OFF;
                                break;
                            case ZCL_SWITCH_ACTION_TOGGLE:
                                cmd_onoff = ZCL_CMD_ONOFF_TOGGLE;
                                break;
                            default:
                                break;
                        }
                        app_cmdOnOff(i+1, cmd_onoff);
                    } else if (!zb_isDeviceFactoryNew()) {
                        zb_rejoinReq(zb_apsChannelMaskGet(), g_bdbAttrs.scanDuration);
                    }
                } else if (button->pressed && !clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
                    button->counter++;
                    if (button->counter >= FR_COUNTER_MAX) {
                        APP_DEBUG(DEBUG_BUTTON_EN, "Reset Factory is ready from toggle\r\n");
                        g_appCtx.not_sleep = true;
                        factory_reset = true;
                        light_blink_stop(i);
                        light_blink_start(1, 2000, 1, i);
                        if (timerFactoryResetEvt) {
                            TL_ZB_TIMER_CANCEL(&timerFactoryResetEvt);
                        }
                        timerFactoryResetEvt = TL_ZB_TIMER_SCHEDULE(factoryResetCb, NULL, TIMEOUT_3SEC);
                        button->counter = 0;
                    }
                }
                button->hold_time = button->pressed_time = clock_time();
            }
        }
    } else {
        if (button->debounce != 1) {
            button->debounce--;
            if (button->debounce == 1 && (button->pressed || button->hold == HOLD_FIX)) {
                button->released = true;
                g_appCtx.not_sleep = true;
//                APP_DEBUG(DEBUG_BUTTON_EN, "Key %d released toggle\r\n", i+1);
                if((button->counter == 1 || button->hold == HOLD_FIX) && zb_isDeviceJoinedNwk()) {
                    if (device_settings.switchType[i] == ZCL_SWITCH_TYPE_MOMENTARY) {
                        cmd_onoff = ZCL_SWITCH_ACTION_ON_OFF;
                        switch(device_settings.switchActions[i]) {
                            case ZCL_SWITCH_ACTION_OFF_ON:
                                cmd_onoff = ZCL_CMD_ONOFF_OFF;
                                break;
                            case ZCL_SWITCH_ACTION_ON_OFF:
                                cmd_onoff = ZCL_CMD_ONOFF_ON;
                                break;
                            case ZCL_SWITCH_ACTION_TOGGLE:
                                cmd_onoff = ZCL_CMD_ONOFF_TOGGLE;
                                break;
                            default:
                                break;
                        }
                        app_cmdOnOff(i+1, cmd_onoff);
                    }
                }
            }
        }
    }

    if (button->released && clock_time_exceed(button->pressed_time, TIMEOUT_TICK_750MS)) {
        APP_DEBUG(DEBUG_BUTTON_EN, "Toggle. Button %d pressed %d times\r\n", i+1, button->counter);
        if (button->counter == BATTERY_COUNTER) {
            batteryCb(NULL);
            if (!g_appCtx.timerSetPollRateEvt) {
                app_setPollRate(TIMEOUT_20SEC);
            }
        }
        clearSleepTimer();
        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->hold = HOLD_NOT_PRESENT;
    } else if (button->pressed && button->counter == 1 && button->hold == HOLD_PRESENT) {
        button->hold = HOLD_FIX;
        button->counter = 0;
        button->pressed = false;
        clearSleepTimer();
    }
}

void button_handler() {

    for (uint8_t i = 0; i < device->button_num; i++) {
        switch(device_settings.switchType[i]) {
            case ZCL_SWITCH_TYPE_TOGGLE:
            case ZCL_SWITCH_TYPE_MOMENTARY:
                read_button_toggle(i);
                break;
            case ZCL_SWITCH_TYPE_MULTIFUNCTION:
                read_button_multifunction(i);
                break;
            case ZCL_CUSTOM_SWITCH_TYPE_LEVEL_MOVE_UP:
            case ZCL_CUSTOM_SWITCH_TYPE_LEVEL_MOVE_DOWN:
                read_button_level(i);
                break;
            case ZCL_CUSTOM_SWITCH_TYPE_SCENE:
                read_button_scene(i);
                break;
            default:
                break;
        }
    }
}

u8 button_idle() {
    app_button_t *button = NULL;
    for (uint8_t i = 0; i < device->button_num; i++) {
        button = &app_button[i];
        if ((button->debounce != 1 && button->debounce != device->button_debounce) || button->pressed || button->counter) {
            return true;
        }
    }
    return false;
}

void button_init() {
    app_button_t *button = NULL;
    for (uint8_t i = 0; i < DEVICE_BUTTON_MAX; i++) {
        button = &app_button[i];
        button->debounce = 1;
        button->hold = false;
        button->counter = 0;
        button->pressed = false;
        button->released = false;
        button->level_up = false;
        button->pressed_time = clock_time();
    }
}
