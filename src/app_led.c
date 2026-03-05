#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

light_t light[DEVICE_BUTTON_MAX];

void led_on(uint8_t pin_idx) {
//    printf("LED is ON\r\n");
    drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_ON(device->led_on));
}

void led_off(uint8_t pin_idx) {
//    printf("LED is OFF\r\n");
    drv_gpio_write(device->led_gpio[pin_idx].gpio, LED_OFF(device->led_off));
}

void light_on(uint8_t pin_idx) {
//    printf("light is ON\r\n");
    led_on(pin_idx);
}

void light_off(uint8_t pin_idx) {
//    printf("light is OFF\r\n");
    led_off(pin_idx);
}

void light_init(void) {
    TL_SETSTRUCTCONTENT(light, 0);
//    led_off(LED1);

}

static int32_t lightTimerCb(void *args) {
    uint32_t interval = 0;
    uint8_t pin_idx = (uint8_t)((uint32_t)args);

    if(light[pin_idx].sta == light[pin_idx].oriSta){
        light[pin_idx].times--;
        if(light[pin_idx].times <= 0){
            light[pin_idx].timerLedEvt = NULL;
            return -1;
        }
    }

    light[pin_idx].sta = !light[pin_idx].sta;
    if(light[pin_idx].sta){
        light_on(pin_idx);
        interval = light[pin_idx].ledOnTime;
    }else{
        light_off(pin_idx);
        interval = light[pin_idx].ledOffTime;
    }

    return interval;
}

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t pin_idx) {
//    printf("light_blink_start, times: %d, onTime: %d, offTime: %d\r\n", times, ledOnTime, ledOffTime);
    uint32_t interval = 0;
    light[pin_idx].times = times;

    if(!light[pin_idx].timerLedEvt){
        if(light[pin_idx].oriSta){
            light_off(pin_idx);
            light[pin_idx].sta = 0;
            interval = ledOffTime;
        }else{
            light_on(pin_idx);
            light[pin_idx].sta = 1;
            interval = ledOnTime;
        }
        light[pin_idx].ledOnTime = ledOnTime;
        light[pin_idx].ledOffTime = ledOffTime;

//        printf("timerLedEvt = NULL, interval: %d\r\n", interval);

        light[pin_idx].timerLedEvt = TL_ZB_TIMER_SCHEDULE(lightTimerCb, (void*)((uint32_t)pin_idx), interval);
    }
}

void light_blink_stop(uint8_t pin_idx) {
//    printf("light_blink_stop\r\n");

    if(light[pin_idx].timerLedEvt){
        TL_ZB_TIMER_CANCEL(&(light[pin_idx].timerLedEvt));

        light[pin_idx].times = 0;
        if(light[pin_idx].oriSta){
            light_on(pin_idx);
        }else{
            light_off(pin_idx);
        }
    }
}

void light_blink_all_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime) {
    for (uint8_t i = 0; i < device->button_num; i++) {
        light_blink_start(times, ledOnTime, ledOffTime, i);
    }
}

void light_blink_all_stop() {
    for (uint8_t i = 0; i < device->button_num; i++) {
        light_blink_stop(i);
    }
}
