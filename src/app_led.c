#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#define DEBUG_LED_EN2 1 //DEBUG_LED_EN
#define DEBUG_LED_EN1 0 //DEBUG_LED_EN

static light_t light[DEVICE_BUTTON_MAX + 1];

void led_on(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "LED %d is ON\r\n", pin_idx+1);
    if (device->device_en) drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_ON(device->led_on));
//    light[pin_idx].led_status = true;
}

void led_off(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "LED %d is OFF\r\n", pin_idx+1);
    if (device->device_en) drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_OFF(device->led_off));
//    light[pin_idx].led_status = false;
}

void light_on(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "light %d is ON\r\n", pin_idx+1);
    if (device->device_en) led_on(pin_idx);
}

void light_off(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "light %d is OFF\r\n", pin_idx+1);
    if (device->device_en) led_off(pin_idx);
}

void light_init(void) {
    TL_SETSTRUCTCONTENT(light, 0);
    for (uint8_t i = 0; i < device->button_num; i++) {
        if (device->device_en) led_off(i);
    }

}

static int32_t lightTimerCb(void *args) {
    uint32_t interval = 0;
    uint8_t pin_idx = (uint8_t)((uint32_t)args);

    light_t *lt = &light[pin_idx];

    if(lt->sta == lt->oriSta){
        lt->times--;
        if(lt->times <= 0){
            lt->timerLedEvt = NULL;
            return -1;
        }
    }

    lt->sta = !lt->sta;
    if(lt->sta) {
        if (lt->timer_stop) {
            light_off(pin_idx);
            lt->timer_stop = false;
            lt->timerLedEvt = NULL;
            return -1;
        }
        light_on(pin_idx);
        interval = lt->ledOnTime;
    }else {
        light_off(pin_idx);
        interval = lt->ledOffTime;
    }

    APP_DEBUG(DEBUG_LED_EN2, "lightTimerCb. idx: %d sta: %d, interval: %d\r\n", pin_idx, lt->sta, interval);

    return interval;
}

static int32_t lightAllTimerCb(void *args) {
    uint32_t interval = 0;

    light_t *lt = &light[device->button_num];

    APP_DEBUG(DEBUG_LED_EN2, "lightAllTimerCb. onTime: %d, offTime: %d, times: %d\r\n", lt->ledOnTime, lt->ledOffTime, lt->times);

    if(lt->sta == lt->oriSta) {
        lt->times--;
        if(lt->times <= 0) {
            lt->all_led = false;
            lt->timerAllLedEvt = NULL;
            return -1;
        }
    }

    lt->sta = !lt->sta;
    if(lt->sta) {
        if (lt->timer_stop) {
            for (uint8_t i = 0; i < device->button_num; i++) {
                light_off(i);
            }
            lt->timer_stop = false;
            lt->all_led = false;
            lt->timerAllLedEvt = NULL;
            return -1;
        }
        for (uint8_t i = 0; i < device->button_num; i++) {
            light_on(i);
        }
        interval = lt->ledOnTime;
    }else {
        for (uint8_t i = 0; i < device->button_num; i++) {
            light_off(i);
        }
        interval = lt->ledOffTime;
    }

    APP_DEBUG(DEBUG_LED_EN2, "lightAllTimerCb. sta: %d, interval: %d\r\n", lt->sta, interval);

    return interval;
}

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t pin_idx) {
    uint32_t interval = 0;

    light_t *lt = &light[pin_idx];


    APP_DEBUG(DEBUG_LED_EN2, "timer: %s, pin_idx: %d, light_blink_start, times: %d, onTime: %d, offTime: %d\r\n",
            lt->timerLedEvt?"true":"false", pin_idx, times, ledOnTime, ledOffTime);

    if(!lt->timerLedEvt){
        lt->times = times;
        if(lt->oriSta) {
            light_off(pin_idx);
            lt->sta = 0;
            interval = ledOffTime;
        }else{
            light_on(pin_idx);
            lt->sta = 1;
            interval = ledOnTime;
        }
        lt->ledOnTime = ledOnTime;
        lt->ledOffTime = ledOffTime;
        lt->timer_stop = false;
        lt->all_led = false;

        APP_DEBUG(DEBUG_LED_EN1, "light_blink_start. idx: %d, timerLedEvt = NULL, sta: %d, interval: %d\r\n", pin_idx, lt->sta, interval);

        lt->timerLedEvt = TL_ZB_TIMER_SCHEDULE(lightTimerCb, (void*)((uint32_t)pin_idx), interval);
    }
}

uint8_t light_blink_stop(uint8_t pin_idx) {
//    APP_DEBUG(DEBUG_LED_EN1, "light_blink_stop\r\n");

    uint8_t ret = 0;

    light_t *lt = &light[pin_idx];

//    lt->timer_stop = true;

    if(lt->timerLedEvt) {
//        APP_DEBUG(DEBUG_LED_EN1, "idx: %d, timerLedEvt true\r\n", pin_idx);
        ret = TL_ZB_TIMER_CANCEL(&(lt->timerLedEvt));
        if (ret == NO_TIMER_AVAIL) {
            lt->timerLedEvt = NULL;
        } else if (ret == TIMER_CANCEL_NOT_ALLOWED) {
            lt->timer_stop = true;
        }
        lt->times = 0;
    }
    light_off(pin_idx);
    APP_DEBUG(DEBUG_LED_EN2, "light_blink_stop. idx: %d, status: %d, timerLedEvt: %s, ret: 0x%02x\r\n", pin_idx, lt->led_status, lt->timerLedEvt?"true":"NULL", ret);
    return ret;
}

void light_blink_all_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime) {
    uint32_t interval = 0;

    light_t *lt = &light[device->button_num];


//    APP_DEBUG(DEBUG_LED_EN2, "timer: %s, pin_idx: %d, light_blink_start, times: %d, onTime: %d, offTime: %d\r\n",
//            lt->timerLedEvt?"true":"false", pin_idx, times, ledOnTime, ledOffTime);

    if(!lt->timerAllLedEvt) {
        lt->times = times;
        if(lt->oriSta) {
            for (uint8_t i = 0; i < device->button_num; i++) {
                light_off(i);
            }
            lt->sta = 0;
            interval = ledOffTime;
        }else {
            for (uint8_t i = 0; i < device->button_num; i++) {
                light_on(i);
            }
            lt->sta = 1;
            interval = ledOnTime;
        }
        lt->ledOnTime = ledOnTime;
        lt->ledOffTime = ledOffTime;
        lt->timer_stop = false;
        lt->all_led = true;

        APP_DEBUG(DEBUG_LED_EN2, "light_blink_all_start. timerAllLedEvt = NULL, times: %d, sta: %d, interval: %d\r\n", lt->times, lt->sta, interval);

        lt->timerAllLedEvt = TL_ZB_TIMER_SCHEDULE(lightAllTimerCb, NULL, interval);
    }
}

void light_blink_all_stop() {
    APP_DEBUG(DEBUG_LED_EN1, "light_blink_all_stop\r\n");

    uint8_t ret = 0;
    light_t *lt = &light[device->button_num];

    if(lt->timerAllLedEvt) {
        ret = TL_ZB_TIMER_CANCEL(&(lt->timerAllLedEvt));
        if (ret == NO_TIMER_AVAIL) {
            lt->timerAllLedEvt = NULL;
        } else if (ret == TIMER_CANCEL_NOT_ALLOWED) {
            lt->timer_stop = true;
        }
        lt->times = 0;
    }
    APP_DEBUG(DEBUG_LED_EN2, "light_blink_all_stop. timerAllLedEvt: %s, ret: 0x%02x\r\n", lt->timerAllLedEvt?"true":"NULL", ret);
    for (uint8_t i = 0; i < device->button_num; i++) {
        while (light_blink_stop(i));
    }
}

int32_t lightTimerStopCb(void *args) {

    uint8_t pin_idx = (uint8_t)((uint32_t)args);

    if (pin_idx < device->button_num) {
        APP_DEBUG(DEBUG_LED_EN2, "lightTimerStopCb idx: %d\r\n", pin_idx);
        while(light_blink_stop(pin_idx));
    } else {
        APP_DEBUG(DEBUG_LED_EN2, "lightTimerStopCb all gpio\r\n");
        light_blink_all_stop();
    }



    return -1;
}



bool light_idle() {

    light_t *lt = NULL;

    for (uint8_t i = 0; i < device->button_num; i++) {
        lt = &light[i];
        if (lt->timerLedEvt || light[device->button_num].timerAllLedEvt) {
            return true;
        }
    }
    return false;
}

#if DEBUG_FINDBIND_EN
void light_test_timer() {
    light_t *lt = NULL;

    for (uint8_t i = 0; i < device->button_num; i++) {
        lt = &light[i];
        APP_DEBUG(DEBUG_FINDBIND_EN, "timer ep: %d is %s\r\n", i+1, lt->timerLedEvt?"true":"false");
    }
}
#endif
