#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#define DEBUG_LED_EN2 1 //DEBUG_LED_EN
#define DEBUG_LED_EN1 0 //DEBUG_LED_EN

static light_t light[DEVICE_BUTTON_MAX + 1];

void led_on(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "LED %d is ON\r\n", pin_idx+1);
    if (device->device_en) drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_ON(device->led_on));
    light[pin_idx].led_status = true;
}

void led_off(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "LED %d is OFF\r\n", pin_idx+1);
    if (device->device_en) drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_OFF(device->led_off));
    light[pin_idx].led_status = false;
}

void light_on(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "light %d is ON\r\n", pin_idx+1);
    if (device->device_en) led_on(pin_idx);
}

void light_off(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN, "light %d is OFF\r\n", pin_idx+1);
    if (device->device_en) led_off(pin_idx);
}

void light_all_on() {
    for (uint8_t i = 0; i < device->button_num; i++) {
        light_on(i);
    }
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

    if (lt->timer_stop) {
        light_off(pin_idx);
        lt->timer_stop = false;
        lt->times = 0;
        lt->timerLedEvt = NULL;
        return -1;
    }


    if(lt->sta == lt->oriSta){
        lt->times--;
        if(lt->times <= 0){
            lt->timerLedEvt = NULL;
            return -1;
        }
    }

    lt->sta = !lt->sta;
    if(lt->sta) {
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

    if (lt->timer_stop) {
        for (uint8_t i = 0; i < device->button_num; i++) {
            light_off(i);
        }
        lt->timer_stop = false;
        lt->times = 0;
        lt->timerLedEvt = NULL;
        return -1;
    }

    if(lt->sta == lt->oriSta) {
        lt->times--;
        if(lt->times <= 0) {
            lt->times = 0;
            lt->timerLedEvt = NULL;
            return -1;
        }
    }

    lt->sta = !lt->sta;
    if(lt->sta) {
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

static int32_t lightBlinkTimerStopCb(void *args) {

    uint8_t pin_idx = (uint8_t)((uint32_t)args);
    if (pin_idx > device->button_num) pin_idx = device->button_num;
    light_t *lt = &light[pin_idx];
    lt->timerLightBlinkStopEvt = NULL;

    if (pin_idx < device->button_num) {
        APP_DEBUG(DEBUG_LED_EN2, "lightBlinkTimerStopCb idx: %d\r\n", pin_idx);
        light_blink_stop(pin_idx);
    } else {
        APP_DEBUG(DEBUG_LED_EN2, "lightBlinkTimerStopCb all gpio\r\n");
        light_blink_all_stop();
    }
    return -1;
}


void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t pin_idx) {
    uint32_t interval = 0;

    light_t *lt = &light[pin_idx];


    APP_DEBUG(DEBUG_LED_EN2, "timer: %s, pin_idx: %d, light_blink_start, times: %d, onTime: %d, offTime: %d\r\n",
            lt->timerLedEvt?"true":"false", pin_idx, times, ledOnTime, ledOffTime);

    if (lt->timerLedEvt) {
        light_blink_stop(pin_idx);
    }
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

    APP_DEBUG(DEBUG_LED_EN1, "light_blink_start. idx: %d, timerLedEvt = NULL, sta: %d, interval: %d\r\n", pin_idx, lt->sta, interval);

    lt->timerLedEvt = TL_ZB_TIMER_SCHEDULE(lightTimerCb, (void*)((uint32_t)pin_idx), interval);
    if (lt->timerLightBlinkStopEvt) TL_ZB_TIMER_CANCEL(&(lt->timerLightBlinkStopEvt));
    lt->timerLightBlinkStopEvt = TL_ZB_TIMER_SCHEDULE(lightBlinkTimerStopCb, (void*)((uint32_t)pin_idx), (ledOnTime+ledOffTime+50)*times);
}

void light_blink_stop(uint8_t pin_idx) {
    APP_DEBUG(DEBUG_LED_EN1, "light_blink_stop\r\n");

    uint8_t ret = 0;

    light_t *lt = &light[pin_idx];

    for (uint8_t i= 0; i < 128 && lt->timerLedEvt; i++) {
        ret = TL_ZB_TIMER_CANCEL(&(lt->timerLedEvt));
        if (ret == NO_TIMER_AVAIL || ret == SUCCESS) {
            lt->timerLedEvt = NULL;
        } else if (ret == TIMER_CANCEL_NOT_ALLOWED) {
            lt->timer_stop = true;
        }
    }
    lt->times = 0;
    light_off(pin_idx);
    if (lt->timerLightBlinkStopEvt) {
        if (TL_ZB_TIMER_CANCEL(&(lt->timerLightBlinkStopEvt)) == NO_TIMER_AVAIL) {
            lt->timerLightBlinkStopEvt = NULL;
        }
    }

    APP_DEBUG(DEBUG_LED_EN2, "light_blink_stop. idx: %d, status: %d, timerLedEvt: %s, ret: 0x%02x\r\n", pin_idx, lt->led_status, lt->timerLedEvt?"true":"NULL", ret);
}

void light_blink_all_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime) {
    uint32_t interval = 0;

    light_t *lt = &light[device->button_num];


    APP_DEBUG(DEBUG_LED_EN2, "timer: %s, pin_idx: %d, light_blink_all_start, times: %d, onTime: %d, offTime: %d\r\n",
            lt->timerLedEvt?"true":"false", device->button_num, times, ledOnTime, ledOffTime);

    if (lt->timerLedEvt) {
        light_blink_all_stop();
    }
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

    APP_DEBUG(DEBUG_LED_EN1, "light_blink_all_start. timerLedEvt = NULL, times: %d, sta: %d, interval: %d\r\n", lt->times, lt->sta, interval);

    lt->timerLedEvt = TL_ZB_TIMER_SCHEDULE(lightAllTimerCb, NULL, interval);
    if (lt->timerLightBlinkStopEvt) TL_ZB_TIMER_CANCEL(&(lt->timerLightBlinkStopEvt));
    lt->timerLightBlinkStopEvt = TL_ZB_TIMER_SCHEDULE(lightBlinkTimerStopCb, (void*)((uint32_t)device->button_num), (ledOnTime+ledOffTime+50)*times);
}

void light_blink_all_stop() {
    APP_DEBUG(DEBUG_LED_EN1, "light_blink_all_stop\r\n");

    uint8_t ret = 0;
    light_t *lt = &light[device->button_num];

    for (uint8_t i= 0; i < 128 && lt->timerLedEvt; i++) {
        ret = TL_ZB_TIMER_CANCEL(&(lt->timerLedEvt));
        if (ret == NO_TIMER_AVAIL || ret == SUCCESS) {
            lt->timerLedEvt = NULL;
        } else if (ret == TIMER_CANCEL_NOT_ALLOWED) {
            lt->timer_stop = true;
        }
    }
    lt->times = 0;
    if (lt->timerLightBlinkStopEvt) {
        if (TL_ZB_TIMER_CANCEL(&(lt->timerLightBlinkStopEvt)) == NO_TIMER_AVAIL) {
            lt->timerLightBlinkStopEvt = NULL;
        }
    }

    APP_DEBUG(DEBUG_LED_EN2, "light_blink_all_stop. timerLedEvt: %s, ret: 0x%02x\r\n", lt->timerLedEvt?"true":"NULL", ret);

//    for (uint8_t i = 0; i < device->button_num; i++) {
//        light_blink_stop(i);
//    }
}

bool light_idle() {

    light_t *lt = NULL;

    for (uint8_t i = 0; i <= device->button_num; i++) {
        lt = &light[i];
        if (lt->timerLedEvt && lt->timerLedEvt->used && lt->times) {
            return true;
        }
    }
    return false;
}

#if UART_PRINTF_MODE && DEBUG_FINDBIND_EN
void light_test_timer() {
    light_t *lt = NULL;

    for (uint8_t i = 0; i <= device->button_num; i++) {
        lt = &light[i];
        APP_DEBUG(DEBUG_FINDBIND_EN, "timer pin_idx: %d is %s\r\n", i, lt->timerLedEvt?"true":"false");
    }
}
#endif
