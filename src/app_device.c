#include "app_main.h"

static bool first_start = true;
static uint8_t zb_modelId[17] = {15,'T','S','0','0','4','1','-','M','0','0','0','-','S','l','D',0};

device_model_t device_model = DEVICE_MODEL;
device_object_t device_object[DEVICE_MODEL_MAX];
device_object_t *device = &device_object[DEVICE_MODEL_1];
device_settings_t device_settings;
bool model_in_flash = false;

static void device_gpio_init(device_gpio_t *device_gpio) {

    drv_gpio_func_set(device_gpio->gpio);
    drv_gpio_output_en(device_gpio->gpio, device_gpio->output);
    drv_gpio_input_en(device_gpio->gpio, device_gpio->input);
    if (device_gpio->pull) drv_gpio_up_down_resistor(device_gpio->gpio, device_gpio->pull);
}

static void device_model_init() {

    device = &device_object[device_model];
    device->device_en = ON;
    if (!model_in_flash) {
        for (uint8_t i = 0; i < device->button_num; i++) {
            device_gpio_init(&device->button_gpio[i]);
            device_gpio_init(&device->led_gpio[i]);
        }
#if UART_PRINTF_MODE && DEBUG_GPIO_EN
    } else {
        for (uint8_t i = 0; i < device->button_num; i++) {
            device_gpio_init(&device->button_gpio[i]);
            device_gpio_init(&device->led_gpio[i]);
        }
#endif
    }
#if UART_PRINTF_MODE
    device_gpio_init(&device->debug_gpio);
    gpio_write(device->debug_gpio.gpio, 1);
#endif
    light_init();

    if (first_start) {
        first_start = false;
        zb_modelId[9] = 0x30 + (device_model + 1) / 100;
        zb_modelId[10] = 0x30 + ((device_model + 1) % 100) / 10;
        zb_modelId[11] = 0x30 + ((device_model + 1) % 100) % 10;

        switch(device_model) {
            case DEVICE_MODEL_1:
            case DEVICE_MODEL_2:
            case DEVICE_MODEL_5:
                zb_modelId[6] = '1';    // key num
                break;
            case DEVICE_MODEL_3:
                zb_modelId[6] = '2';
                break;
            case DEVICE_MODEL_4:
                zb_modelId[6] = '4';
                break;
            default:
                zb_modelId[6] = '1';
                break;
        }
        memcpy(g_zcl_basicAttrs.modelId, zb_modelId, 16);
        g_zcl_onOffCfgAttrs[0].device_model = device_model+1;
        button_init();
    }
}

#if UART_PRINTF_MODE
static void print_setting_sr(nv_sts_t st, device_settings_t *device_settings_tmp, bool save) {

    APP_DEBUG(DEBUG_SAVE_EN, "Settings %s. Return: %s\r\n", save?"saved":"restored", st==NV_SUCC?"Ok":"Error");

    for (uint8_t i = 0; i < DEVICE_BUTTON_MAX; i++) {
        APP_DEBUG(DEBUG_SAVE_EN, "switchActions%d:     0x%02x\r\n", i, device_settings_tmp->switchActions[i]);
        APP_DEBUG(DEBUG_SAVE_EN, "switchType%d:        0x%02x\r\n", i, device_settings_tmp->switchType[i]);
        APP_DEBUG(DEBUG_SAVE_EN, "levelMin%d:          %d\r\n", i, device_settings_tmp->levelMin[i]);
        APP_DEBUG(DEBUG_SAVE_EN, "levelMax%d:          %d\r\n", i, device_settings_tmp->levelMax[i]);
        APP_DEBUG(DEBUG_SAVE_EN, "defaultMoveRate%d:   %d\r\n", i, device_settings_tmp->defaultMoveRate[i]);
        APP_DEBUG(DEBUG_SAVE_EN, "transitionTime%d:    %d\r\n", i, device_settings_tmp->transitionTime[i]);
        APP_DEBUG(DEBUG_SAVE_EN, "Scene%d:             %d\r\n", i, device_settings_tmp->scene[i].sceneId);
        APP_DEBUG(DEBUG_SAVE_EN, "SceneGroup%d:        %d\r\n", i, device_settings_tmp->scene[i].groupId);
    }

}
#endif

static void set_device_setting_default(device_settings_t *settings) {

    APP_DEBUG(DEBUG_SAVE_EN, "set_device_setting_default\r\n");

    memset(settings, 0, sizeof(device_settings_t));

    for (uint8_t i = 0; i < device->button_num; i++) {
        if (device_model == DEVICE_MODEL_4) {
            if (i == 0 || i == 1) settings->switchActions[i] = ZCL_SWITCH_ACTION_OFF_ON;
            if (i == 2 || i == 3) settings->switchActions[i] = ZCL_SWITCH_ACTION_ON_OFF;
        } else {
            settings->switchActions[i] = ZCL_SWITCH_ACTION_TOGGLE;
            settings->switchType[i] = ZCL_SWITCH_TYPE_TOGGLE;
        }
        settings->defaultMoveRate[i] = DEFAULT_MOVE_RATE;
        settings->defaultMoveRate[i] = DEFAULT_MOVE_RATE;
        settings->levelMin[i] = ZCL_LEVEL_ATTR_MIN_LEVEL;
        settings->levelMax[i] = ZCL_LEVEL_ATTR_MAX_LEVEL;
        settings->transitionTime[i] = LEVEL_TRANSITION_TIME;
        settings->scene[i].groupId = 0;
        settings->scene[i].sceneId = 0;
    }

}

nv_sts_t device_settings_default() {

    nv_sts_t st = NV_SUCC;

#if NV_ENABLE

    APP_DEBUG(UART_PRINTF_MODE, "Saved device default settings\r\n");

    set_device_setting_default(&device_settings);

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

        APP_DEBUG(UART_PRINTF_MODE, "Restored device settings\r\n");
#if UART_PRINTF_MODE
        print_setting_sr(st, &device_settings_tmp, false);
#endif

    } else {
        /* default config */
        APP_DEBUG(UART_PRINTF_MODE, "Default device settings \r\n");
        set_device_setting_default(&device_settings_tmp);
    }

    memcpy(&device_settings, &device_settings_tmp, (sizeof(device_settings_t)));
    for (uint8_t i = 0; i < device->button_num; i++) {
        g_zcl_onOffCfgAttrs[i].custom_swtichType = device_settings.switchType[i];
        g_zcl_onOffCfgAttrs[i].switchActions = device_settings.switchActions[i];
        g_zcl_levelAttrs[i].defaultMoveRate = device_settings.defaultMoveRate[i];
        g_zcl_levelAttrs[i].minLevel = device_settings.levelMin[i];
        g_zcl_levelAttrs[i].maxLevel = device_settings.levelMax[i];
        g_zcl_levelAttrs[i].transitionTime = device_settings.transitionTime[i];
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

    APP_DEBUG(UART_PRINTF_MODE, "Saved device settings\r\n");

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
        APP_DEBUG(UART_PRINTF_MODE, "Model restore: M%03d\r\n", device_model+1);
        device_model_init();
    } else {
        device_model = DEVICE_MODEL;
        APP_DEBUG(UART_PRINTF_MODE, "Default model: M%03d\r\n", device_model+1);
        device_model_save(device_model);
    }
}

void device_model_save(uint8_t model) {
    config_model_t model_cfg;

    model_cfg.id = ID_DEVICE_MODEL_CFG;
    device_model = model_cfg.device_model = model;

    flash_erase(DEVICE_MODEL_CFG_ADDR);
    flash_write(DEVICE_MODEL_CFG_ADDR, sizeof(config_model_t), (uint8_t*)&(model_cfg));

    APP_DEBUG(UART_PRINTF_MODE, "Model save: M%03d\r\n", device_model+1);

    device_model_init();
}

void device_init() {
    uint8_t devi = DEVICE_MODEL_1;
    if (first_start) {
        /* TS0041 _TZ3000_an5rjiwd Tuya - model_1 */
        device_object[devi].device_en = OFF;
        device_object[devi].button_num = 1;
        device_object[devi].button_gpio[0].gpio = GPIO_PC2;
        device_object[devi].button_gpio[0].input = ON;
        device_object[devi].button_gpio[0].output = OFF;
        device_object[devi].button_gpio[0].func = AS_GPIO;
        device_object[devi].button_gpio[0].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_debounce = DEBOUNCE_BUTTON;
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

        /* TS0601 TZ3000_ja5osu5g Loginovo  - model_2 */
        device_object[devi].device_en = OFF;
        device_object[devi].button_num = 1;
        device_object[devi].button_gpio[0].gpio = GPIO_PB6;
        device_object[devi].button_gpio[0].input = ON;
        device_object[devi].button_gpio[0].output = OFF;
        device_object[devi].button_gpio[0].func = AS_GPIO;
        device_object[devi].button_gpio[0].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_debounce = DEBOUNCE_BUTTON;
        device_object[devi].led_gpio[0].gpio = GPIO_PC4;
        device_object[devi].led_gpio[0].input = OFF;
        device_object[devi].led_gpio[0].output = ON;
        device_object[devi].led_gpio[0].func = AS_GPIO;
        device_object[devi].led_on = 0;
        device_object[devi].led_off = 1;
        device_object[devi].debug_gpio.gpio = GPIO_PB1;
        device_object[devi].debug_gpio.input = OFF;
        device_object[devi].debug_gpio.output = ON;
        device_object[devi].debug_gpio.func = AS_GPIO;
        device_object[devi++].debug_gpio.pull = PM_PIN_PULLUP_1M;

        /* TS0042 _TZ3000_h68nee3e Tuya (EKF)  - model_3 */
        device_object[devi].device_en = OFF;
        device_object[devi].button_num = 2;
        device_object[devi].button_gpio[0].gpio = GPIO_PB5;
        device_object[devi].button_gpio[0].input = ON;
        device_object[devi].button_gpio[0].output = OFF;
        device_object[devi].button_gpio[0].func = AS_GPIO;
        device_object[devi].button_gpio[0].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_gpio[1].gpio = GPIO_PA1;
        device_object[devi].button_gpio[1].input = ON;
        device_object[devi].button_gpio[1].output = OFF;
        device_object[devi].button_gpio[1].func = AS_GPIO;
        device_object[devi].button_gpio[1].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_debounce = DEBOUNCE_BUTTON;
        device_object[devi].led_gpio[0].gpio = GPIO_PC1;
        device_object[devi].led_gpio[0].input = OFF;
        device_object[devi].led_gpio[0].output = ON;
        device_object[devi].led_gpio[0].func = AS_GPIO;
        device_object[devi].led_gpio[1].gpio = GPIO_PC2;
        device_object[devi].led_gpio[1].input = OFF;
        device_object[devi].led_gpio[1].output = ON;
        device_object[devi].led_gpio[1].func = AS_GPIO;
        device_object[devi].led_on = 1;
        device_object[devi].led_off = 0;
        device_object[devi].debug_gpio.gpio = GPIO_PB1;
        device_object[devi].debug_gpio.input = OFF;
        device_object[devi].debug_gpio.output = ON;
        device_object[devi].debug_gpio.func = AS_GPIO;
        device_object[devi++].debug_gpio.pull = PM_PIN_PULLUP_1M;

        /* TS0044 HOBEIAN Tuya - model_4 */
        device_object[devi].device_en = OFF;
        device_object[devi].button_num = 4;
        device_object[devi].button_gpio[0].gpio = GPIO_PC4;
        device_object[devi].button_gpio[0].input = ON;
        device_object[devi].button_gpio[0].output = OFF;
        device_object[devi].button_gpio[0].func = AS_GPIO;
        device_object[devi].button_gpio[0].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_gpio[1].gpio = GPIO_PD3;
        device_object[devi].button_gpio[1].input = ON;
        device_object[devi].button_gpio[1].output = OFF;
        device_object[devi].button_gpio[1].func = AS_GPIO;
        device_object[devi].button_gpio[1].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_gpio[2].gpio = GPIO_PB5;
        device_object[devi].button_gpio[2].input = ON;
        device_object[devi].button_gpio[2].output = OFF;
        device_object[devi].button_gpio[2].func = AS_GPIO;
        device_object[devi].button_gpio[2].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_gpio[3].gpio = GPIO_PD7;
        device_object[devi].button_gpio[3].input = ON;
        device_object[devi].button_gpio[3].output = OFF;
        device_object[devi].button_gpio[3].func = AS_GPIO;
        device_object[devi].button_gpio[3].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_debounce = DEBOUNCE_BUTTON * 2;
        device_object[devi].led_gpio[0].gpio = GPIO_PC3;
        device_object[devi].led_gpio[0].input = OFF;
        device_object[devi].led_gpio[0].output = ON;
        device_object[devi].led_gpio[0].func = AS_GPIO;
        device_object[devi].led_gpio[1].gpio = GPIO_PD4;
        device_object[devi].led_gpio[1].input = OFF;
        device_object[devi].led_gpio[1].output = ON;
        device_object[devi].led_gpio[1].func = AS_GPIO;
        device_object[devi].led_gpio[2].gpio = GPIO_PB6;
        device_object[devi].led_gpio[2].input = OFF;
        device_object[devi].led_gpio[2].output = ON;
        device_object[devi].led_gpio[2].func = AS_GPIO;
        device_object[devi].led_gpio[3].gpio = GPIO_PA0;
        device_object[devi].led_gpio[3].input = OFF;
        device_object[devi].led_gpio[3].output = ON;
        device_object[devi].led_gpio[3].func = AS_GPIO;
        device_object[devi].led_on = 0;
        device_object[devi].led_off = 1;
        device_object[devi].debug_gpio.gpio = GPIO_PB1;
        device_object[devi].debug_gpio.input = OFF;
        device_object[devi].debug_gpio.output = ON;
        device_object[devi].debug_gpio.func = AS_GPIO;
        device_object[devi++].debug_gpio.pull = PM_PIN_PULLUP_1M;

        /* TS0041 _TZ3000_itb0omhv Tuya - model_5 */
        device_object[devi].device_en = OFF;
        device_object[devi].button_num = 1;
        device_object[devi].button_gpio[0].gpio = GPIO_PC2;
        device_object[devi].button_gpio[0].input = ON;
        device_object[devi].button_gpio[0].output = OFF;
        device_object[devi].button_gpio[0].func = AS_GPIO;
        device_object[devi].button_gpio[0].pull = PM_PIN_PULLUP_1M;
        device_object[devi].button_debounce = DEBOUNCE_BUTTON;
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

        if (model_in_flash) {
            device_model_init();
        } else {
            device_model_restore();
        }
//    } else {
//        device_model_init();
    }
}

