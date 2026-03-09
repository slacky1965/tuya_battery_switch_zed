#ifndef SRC_INCLUDE_APP_DEVICE_H_
#define SRC_INCLUDE_APP_DEVICE_H_

#ifndef DEVICE_MODEL
#define DEVICE_MODEL    DEVICE_MODEL_1
#endif

#define DEVICE_MODEL_CFG_ADDR  0x75000
#define ID_DEVICE_MODEL_CFG    0x65651410

typedef enum {
    DEVICE_MODEL_1 = 0,
    DEVICE_MODEL_2,
    DEVICE_MODEL_3,
    DEVICE_MODEL_MAX
} device_model_t;

typedef struct __attribute__((packed)) {
    uint32_t id;
    uint8_t  device_model;
} config_model_t;

typedef struct __attribute__((packed)) {
    uint32_t            gpio;
    uint8_t             input;
    uint8_t             output;
    GPIO_FuncTypeDef    func;
    GPIO_PullTypeDef    pull;
} device_gpio_t;

typedef struct __attribute__((packed)) {
    uint8_t             device_en :1;
    uint8_t             led_on :1;
    uint8_t             led_off :1;
    uint8_t             button_num :5;
    device_gpio_t       button_gpio[DEVICE_BUTTON_MAX];
    device_gpio_t       led_gpio[DEVICE_BUTTON_MAX];
    uint8_t             switch_debounce;
    device_gpio_t       debug_gpio;
} device_object_t;


typedef struct __attribute__((packed)) {
    /*
     * custom switchType
     * 0x00 - toggle
     * 0x01 - momentary
     * 0x02 - multifunction
     * 0x03 - level move up
     * 0x04 - level move down
     * 0x05 - level move up with onoff
     * 0x06 - level move down with onoff
     * 0x07 - level stop
     */
    uint8_t switchType[DEVICE_BUTTON_MAX];
    uint8_t switchActions[DEVICE_BUTTON_MAX];
    uint8_t defaultMoveRate[DEVICE_BUTTON_MAX];
    app_scene_t scene[DEVICE_BUTTON_MAX];
    uint8_t crc;
} device_settings_t;

extern device_model_t device_model;
extern device_object_t *device;
extern device_settings_t device_settings;
extern bool model_in_flash;

void device_model_restore();
void device_model_save(uint8_t model);
void device_init();
nv_sts_t device_settings_restore();
nv_sts_t device_settings_save();
nv_sts_t device_settings_default();

#endif /* SRC_INCLUDE_APP_DEVICE_H_ */
