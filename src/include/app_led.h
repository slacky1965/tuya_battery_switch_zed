#ifndef SRC_INCLUDE_APP_LED_H_
#define SRC_INCLUDE_APP_LED_H_

#define LED_ON(d)       (d?1:0)
#define LED_OFF(d)      (d?1:0)

typedef struct {
    ev_timer_event_t *timerLedEvt;

    uint32_t led_pin;
    bool     led_status;

    uint16_t ledOnTime;
    uint16_t ledOffTime;
    uint8_t  oriSta;     //original state before blink
    uint8_t  sta;        //current state in blink
    int8_t   times;      //blink times
} light_t;

void light_blink_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime, uint8_t pin_idx);
void light_blink_stop(uint8_t pin_idx);

void light_blink_all_start(uint8_t times, uint16_t ledOnTime, uint16_t ledOffTime);
void light_blink_all_stop();

void light_init(void);
void light_on(uint8_t pin_idx);
void light_off(uint8_t pin_idx);
bool light_idle();

void led_on(uint8_t pin_idx);
void led_off(uint8_t pin_idx);

#endif /* SRC_INCLUDE_APP_LED_H_ */
