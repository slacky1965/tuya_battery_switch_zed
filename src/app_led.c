#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

#define DEBUG_LED_EN1 1

light_t light[DEVICE_BUTTON_MAX];

void led_on(uint8_t pin_idx) {
    DEBUG(DEBUG_LED_EN, "LED %d is ON\r\n", pin_idx+1);
    if (device->device_en) drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_ON(device->led_on));
//    light[pin_idx].led_status = true;
}

void led_off(uint8_t pin_idx) {
    DEBUG(DEBUG_LED_EN, "LED %d is OFF\r\n", pin_idx+1);
    if (device->device_en) drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_OFF(device->led_off));
//    light[pin_idx].led_status = false;
}

void light_on(uint8_t pin_idx) {
    DEBUG(DEBUG_LED_EN, "light %d is ON\r\n", pin_idx+1);
    if (device->device_en) led_on(pin_idx);
}

void light_off(uint8_t pin_idx) {
    DEBUG(DEBUG_LED_EN, "light %d is OFF\r\n", pin_idx+1);
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
    if(lt->sta){
        light_on(pin_idx);
        interval = lt->ledOnTime;
    }else{
        light_off(pin_idx);
        interval = lt->ledOffTime;
    }

    return interval;
}

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t pin_idx) {
    uint32_t interval = 0;

    light_t *lt = &light[pin_idx];

    lt->times = times;

    if(!lt->timerLedEvt){
        DEBUG(DEBUG_LED_EN1, "pin_idx: %d, light_blink_start, times: %d, onTime: %d, offTime: %d\r\n", pin_idx, times, ledOnTime, ledOffTime);
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

        printf("timerLedEvt = NULL, interval: %d\r\n", interval);

        lt->timerLedEvt = TL_ZB_TIMER_SCHEDULE(lightTimerCb, (void*)((uint32_t)pin_idx), interval);
    }
}

void light_blink_stop(uint8_t pin_idx) {
    DEBUG(DEBUG_LED_EN1, "light_blink_stop\r\n");

    light_t *lt = &light[pin_idx];

    if(lt->timerLedEvt) {
        DEBUG(DEBUG_LED_EN1, "idx: %d, timerLedEvt true\r\n", pin_idx);
        TL_ZB_TIMER_CANCEL(&lt->timerLedEvt);
        lt->times = 0;
    }
    light_off(pin_idx);
    DEBUG(DEBUG_LED_EN1, "idx: %d, status: %d, timerLedEvt? %s\r\n", pin_idx, lt->led_status, lt->timerLedEvt?"true":"NULL");
}

void light_blink_all_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime) {
    DEBUG(DEBUG_LED_EN1, "light_blink_all_start\r\n");
    for (uint8_t i = 0; i < device->button_num; i++) {
        light_blink_start(times, ledOnTime, ledOffTime, i);
    }
}

void light_blink_all_stop() {
    DEBUG(DEBUG_LED_EN1, "light_blink_all_stop\r\n");
    for (uint8_t i = 0; i < device->button_num; i++) {
        light_blink_stop(i);
    }
}

bool light_idle() {

    light_t *lt = NULL;

    for (uint8_t i = 0; i < device->button_num; i++) {
        lt = &light[i];
        if (lt->timerLedEvt) {
            return true;
        }
    }
    return false;
}
