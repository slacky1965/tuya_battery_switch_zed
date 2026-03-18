#include "app_main.h"

#if PM_ENABLE
/**
 *  @brief Definition for wakeup source and level for PM
 */

static drv_pm_pinCfg_t pin_PmCfg[DEVICE_BUTTON_MAX];
//= {
//    {
//        BUTTON1_GPIO,
//        PM_WAKEUP_LEVEL_LOW
//    },
//    {
//        BUTTON2_GPIO,
//        PM_WAKEUP_LEVEL_LOW
//    },
//    {
//        BUTTON3_GPIO,
//        PM_WAKEUP_LEVEL_LOW
//    },
//    {
//        BUTTON4_GPIO,
//        PM_WAKEUP_LEVEL_LOW
//    },
////    {
////        BUTTON5_GPIO,
////        PM_WAKEUP_LEVEL_LOW
////    },
////    {
////        BUTTON6_GPIO,
////        PM_WAKEUP_LEVEL_LOW
////    },
//};

void app_wakeupPinConfig() {
    for (uint8_t i = 0; i < device->button_num; i++) {
        pin_PmCfg[i].wakeupPin = device->button_gpio[i].gpio;
        pin_PmCfg[i].wakeupLevel = PM_WAKEUP_LEVEL_LOW;
    }
    drv_pm_wakeupPinConfig(pin_PmCfg, device->button_num /*sizeof(pin_PmCfg)/sizeof(drv_pm_pinCfg_t)*/);
}


void app_wakeupPinLevelChange() {
    drv_pm_wakeupPinLevelChange(pin_PmCfg, device->button_num);
}

void app_lowPowerEnter() {

//    printf("app_lowPowerEnter(). g_appCtx.not_sleep: %d\r\n", g_appCtx.not_sleep);

    uint32_t durationMs = 0;

    app_wakeupPinLevelChange();

    if (g_appCtx.not_sleep) {
        /* SDK deep sleep with SRAM retention */
        drv_pm_lowPowerEnter();
    } else /*if (zb_isDeviceJoinedNwk())*/{
        /* app deep sleep with SRAM retention */
        if (tl_stackBusy() || !zb_isTaskDone()) {
            APP_DEBUG(DEBUG_PM_EN, "Stack or Task busy. Return from deep sleep start\r\n");
            return;
        }

        apsCleanToStopSecondClock();

        uint32_t r = drv_disable_irq();
        rf_paShutDown();

        durationMs = g_appCtx.timerBatteryEvt->timeout /*TIME_LONG_DEEP_SLEEP * 1000*/;

        APP_DEBUG(DEBUG_PM_EN, "Long deep sleep start with time: %d sec\r\n", durationMs / 1000);

        drv_pm_longSleep(PM_SLEEP_MODE_DEEP_WITH_RETENTION, PM_WAKEUP_SRC_PAD | PM_WAKEUP_SRC_TIMER, durationMs);

        drv_restore_irq(r);

    }
}

#endif


