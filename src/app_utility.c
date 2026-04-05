#include "tl_common.h"
#include "zcl_include.h"

#include "app_main.h"

uint32_t mcuBootAddrGet(void);

void start_message() {
#ifdef ZCL_OTA
        APP_DEBUG(UART_PRINTF_MODE, "OTA mode enabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#else
        APP_DEBUG(UART_PRINTF_MODE, "OTA mode desabled. MCU boot from address: 0x%x\r\n", mcuBootAddrGet());
#endif

#if UART_PRINTF_MODE
    const uint8_t version[] = ZCL_BASIC_SW_BUILD_ID;
    APP_DEBUG(UART_PRINTF_MODE, "Firmware version: %s\r\n", version+1);
    APP_DEBUG(UART_PRINTF_MODE, "Model: %d\r\n", device_model+1);
#endif
}

int32_t delayedMcuResetCb(void *arg) {

    //printf("mcu reset\r\n");
    zb_resetDevice();
    return -1;
}

int32_t delayedFactoryResetCb(void *arg) {

    zb_resetDevice2FN();
    zb_deviceFactoryNewSet(true);

//    factory_reset = true;
    g_appCtx.timerFactoryReset = NULL;

//    printf("Cb Factory new: %s\r\n", zb_isDeviceFactoryNew()?"yes":"no");


    return -1;
}

int32_t delayedFullResetCb(void *arg) {

    //printf("full reset\r\n");
    return -1;
}

static int32_t set_pollRateCb(void *args) {

    APP_DEBUG(DEBUG_PM_EN, "set_pollRateCb\r\n");

    g_appCtx.not_sleep = false;

//    zb_setPollRate(POLL_RATE * 60);

    g_appCtx.timerSetPollRateEvt = NULL;
    return -1;
}

void app_setPollRate(uint32_t sec) {

    APP_DEBUG(DEBUG_PM_EN, "app_setPollRate(). sec: %d\r\n", sec);

    g_appCtx.not_sleep = true;

    if (g_appCtx.ota) {
        if (g_appCtx.timerSetPollRateEvt) {
            TL_ZB_TIMER_CANCEL(&g_appCtx.timerSetPollRateEvt);
        }
        return;
    }

    zb_setPollRate(POLL_RATE * 3);

    if (g_appCtx.timerSetPollRateEvt) {
        TL_ZB_TIMER_CANCEL(&g_appCtx.timerSetPollRateEvt);
    }
    g_appCtx.timerSetPollRateEvt = TL_ZB_TIMER_SCHEDULE(set_pollRateCb, NULL, sec);

}

int32_t net_steer_start_offCb(void *args) {

    g_appCtx.net_steer_start = false;

    light_blink_all_stop();

    return -1;
}

uint8_t checksum(uint8_t *data, uint16_t length) {

    uint8_t crc8 = 0;

    for(uint8_t i = 0; i < length; i++) {
        crc8 += data[i];
    }

    return crc8;
}


