#include "app_main.h"

static bool first_start = true;
static uint8_t zb_modelId[17] = {15,'T','S','0','0','4','1','-','M','0','0','0','-','S','l','D',0};

device_model_t device_model = DEVICE_MODEL;
device_object_t device_object[DEVICE_MODEL_MAX];
device_object_t *device = &device_object[DEVICE_MODEL_1];
device_settings_t device_settings;

static void device_gpio_init(device_gpio_t *device_gpio) {

    drv_gpio_func_set(device_gpio->gpio);
    drv_gpio_output_en(device_gpio->gpio, device_gpio->output);
    drv_gpio_input_en(device_gpio->gpio, device_gpio->input);
    if (device_gpio->pull) drv_gpio_up_down_resistor(device_gpio->gpio, device_gpio->pull);
}

static void device_model_init() {

    device = &device_object[device_model];
//    gpio_init(TRUE);
//    /* ADC */
//#if VOLTAGE_DETECT_ENABLE
//    drv_adc_init();
//    drv_adc_mode_pin_set(DRV_ADC_VBAT_MODE, VOLTAGE_DETECT_ADC_PIN);
//    drv_adc_enable(ON);
//#endif
    device_gpio_init(&device->button_gpio[0]);
    device_gpio_init(&device->led_gpio[0]);
#if UART_PRINTF_MODE
    device_gpio_init(&device->debug_gpio);
    gpio_write(device->debug_gpio.gpio, 1);
#endif
    light_init();

    zb_modelId[9] = 0x30 + (device_model + 1) / 100;
    zb_modelId[10] = 0x30 + ((device_model + 1) % 100) / 10;
    zb_modelId[11] = 0x30 + ((device_model + 1) % 100) % 10;

    switch(device_model) {
        case DEVICE_MODEL_1:
            zb_modelId[6] = '1';
            break;
        default:
            zb_modelId[6] = '1';
            break;
    }
    memcpy(g_zcl_basicAttrs.modelId, zb_modelId, 16);
    g_zcl_onOffCfgAttrs[0].device_model = device_model+1;
    button_init();
}

#if UART_PRINTF_MODE
static void print_setting_sr(nv_sts_t st, device_settings_t *device_settings_tmp, bool save) {

    DEBUG(DEBUG_SAVE_EN, "Settings %s. Return: %s\r\n", save?"saved":"restored", st==NV_SUCC?"Ok":"Error");

    for (uint8_t i = 0; i < DEVICE_BUTTON_MAX; i++) {
        DEBUG(DEBUG_SAVE_EN, "switchActions%d:     0x%02x\r\n", i, device_settings_tmp->switchActions[i]);
        DEBUG(DEBUG_SAVE_EN, "switchType%d:        0x%02x\r\n", i, device_settings_tmp->switchType[i]);
    }

}
#endif

nv_sts_t device_settings_default() {

    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    DEBUG(UART_PRINTF_MODE, "Saved device default settings\r\n");

    for (uint8_t i = 0; i < DEVICE_BUTTON_MAX; i++) {
        device_settings.switchActions[i] = ZCL_SWITCH_ACTION_TOGGLE;
        device_settings.switchType[i] = ZCL_SWITCH_TYPE_TOGGLE;
        device_settings.defaultMoveRate[i] = DEFAULT_MOVE_RATE;
        device_settings.scene[i].groupId = 0;
        device_settings.scene[i].sceneId = 0;
    }

    device_settings.crc = checksum((uint8_t*)&device_settings, sizeof(device_settings_t)-1);
    st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(device_settings_t), (uint8_t*)&device_settings);

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t device_settings_restore() {

    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    device_settings_t device_settings_tmp;

    st = nv_flashReadNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(device_settings_t), (uint8_t*)&device_settings_tmp);

    if (st == NV_SUCC && device_settings_tmp.crc == checksum((uint8_t*)&device_settings_tmp, sizeof(device_settings_t)-1)) {

        DEBUG(UART_PRINTF_MODE, "Restored device settings\r\n");
#if UART_PRINTF_MODE
        print_setting_sr(st, &device_settings_tmp, false);
#endif

    } else {
        /* default config */
        DEBUG(UART_PRINTF_MODE, "Default device settings \r\n");

        for (uint8_t i = 0; i < DEVICE_BUTTON_MAX; i++) {
            device_settings_tmp.switchActions[i] = ZCL_SWITCH_ACTION_TOGGLE;
            device_settings_tmp.switchType[i] = ZCL_SWITCH_TYPE_TOGGLE;
            device_settings_tmp.defaultMoveRate[i] = DEFAULT_MOVE_RATE;
            device_settings_tmp.scene[i].groupId = 0;
            device_settings_tmp.scene[i].sceneId = 0;
        }
    }

    memcpy(&device_settings, &device_settings_tmp, (sizeof(device_settings_t)));
    for (uint8_t i = 0; i < DEVICE_BUTTON_MAX; i++) {
        g_zcl_onOffCfgAttrs[i].custom_swtichType = device_settings.switchType[i];
        g_zcl_onOffCfgAttrs[i].switchActions = device_settings.switchActions[i];
        g_zcl_levelAttrs[i].defaultMoveRate = device_settings.defaultMoveRate[i];
        g_zcl_sceneAttrs[i].customScene = device_settings.scene[i].sceneId;
        g_zcl_sceneAttrs[i].customGroup = device_settings.scene[i].groupId;
    }

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}

nv_sts_t device_settings_save() {
    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    DEBUG(UART_PRINTF_MODE, "Saved device settings\r\n");

    device_settings.crc = checksum((uint8_t*)&device_settings, sizeof(device_settings_t)-1);
    st = nv_flashWriteNew(1, NV_MODULE_APP,  NV_ITEM_APP_USER_CFG, sizeof(device_settings_t), (uint8_t*)&device_settings);

#else
    st = NV_ENABLE_PROTECT_ERROR;
#endif

    return st;
}
void device_model_restore() {

    config_model_t model_cfg;

    flash_read_page(DEVICE_MODEL_CFG_ADDR, sizeof(config_model_t), (uint8_t*)&model_cfg);

    if (model_cfg.id == ID_DEVICE_MODEL_CFG) {
        device_model = model_cfg.device_model;
        DEBUG(UART_PRINTF_MODE, "Model restore: M%03d\r\n", device_model+1);
        device_model_init();
    } else {
        device_model = DEVICE_MODEL;
        DEBUG(UART_PRINTF_MODE, "Default model: M%03d\r\n", device_model+1);
        device_model_save(device_model);
    }
}

void device_model_save(uint8_t model) {
    config_model_t model_cfg;

    model_cfg.id = ID_DEVICE_MODEL_CFG;
    device_model = model_cfg.device_model = model;

    flash_erase(DEVICE_MODEL_CFG_ADDR);
    flash_write(DEVICE_MODEL_CFG_ADDR, sizeof(config_model_t), (uint8_t*)&(model_cfg));

    DEBUG(UART_PRINTF_MODE, "Model save: M%03d\r\n", device_model+1);

    device_model_init();
}

void device_init() {
    uint8_t devi = DEVICE_MODEL_1;
    if (first_start) {
        first_start = false;
        /* TS0203 Zbeacon Tuya  - model_1 */
        device_object[devi].button_num = 1;
        device_object[devi].button_gpio[0].gpio = GPIO_PC2;
        device_object[devi].button_gpio[0].input = ON;
        device_object[devi].button_gpio[0].output = OFF;
        device_object[devi].button_gpio[0].func = AS_GPIO;
        device_object[devi].button_gpio[0].pull = PM_PIN_PULLUP_1M;
        device_object[devi].led_gpio[0].gpio = GPIO_PC4;
        device_object[devi].led_gpio[0].input = OFF;
        device_object[devi].led_gpio[0].output = ON;
        device_object[devi].led_gpio[0].func = AS_GPIO;
        device_object[devi].led_on = 1;
        device_object[devi].led_off = 0;
        device_object[devi].debug_gpio.gpio = GPIO_PB1;
        device_object[devi].debug_gpio.input = OFF;
        device_object[devi].debug_gpio.output = ON;
        device_object[devi].debug_gpio.func = AS_GPIO;
        device_object[devi++].debug_gpio.pull = PM_PIN_PULLUP_1M;
        device_model_restore();
    } else {
        device_model_init();
    }
}

