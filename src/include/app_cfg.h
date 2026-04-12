/********************************************************************************************************
 * @file    app_cfg.h
 *
 * @brief   This is the header file for app_cfg
 *
 * @author  Zigbee Group
 * @date    2021
 *
 * @par     Copyright (c) 2021, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *			All rights reserved.
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *
 *******************************************************************************************************/

#ifndef SRC_INCLUDE_APP_CFG_H_
#define SRC_INCLUDE_APP_CFG_H_

#include "app_types.h"

#define ON                      1
#define OFF                     0

#ifndef MCU_CORE_8258
#define MCU_CORE_8258   1
#endif

/* for reporting */
#define REPORTING_MIN           10              /* 10 sec                   */
#define REPORTING_MAX           3600            /* 60 min                   */
#define REPORTING_BATTERY_MIN   3600            /* 1 hour                   */
#define REPORTING_BATTERY_MAX   21600           /* 6 hours                  */
#define REPORTING_MAX_MAX       65000           /* 18 hour                  */

/* for polling */
#define LONG_POLL               REPORTING_MIN
#define TIMEOUT_NET             TIMEOUT_30MIN

#define ZB_APP_TX_POWER_IDX             RF_POWER_INDEX_P1p99dBm

/* for OTA */
#define APP_OTA_PERIODIC_QUERY_INTERVAL (1080 * 60)     /* start the OTA request after the set seconds */

#define DEVICE_BUTTON_MAX   6

/**********************************************************************
 * Product Information
 * max 24 symbols
 */

#define ZCL_BASIC_MFG_NAME     {10,'S','l','a','c','k','y','-','D','I','Y'}
#define ZCL_BASIC_MODEL_ID     {12,'T','S','0','0','4','1','-','z','-','S','l','D'}

/**********************************************************************
 * Version configuration
 */
#include "version_cfg.h"

/* Debug mode config */
#ifndef UART_PRINTF_MODE
#define	UART_PRINTF_MODE                ON
#endif
#define USB_PRINTF_MODE         		OFF

#define DEBUG_BUTTON_EN                 ON
#define DEBUG_SAVE_EN                   OFF
#define DEBUG_REPORTING_EN              OFF
#define DEBUG_BATTERY_EN                OFF
#define DEBUG_PM_EN                     ON
#define DEBUG_OTA_EN                    OFF
#define DEBUG_ONOFF_EN                  ON
#define DEBUG_LEVEL_EN                  ON
#define DEBUG_SCENE_EN                  OFF
#define DEBUG_GPIO_EN                   OFF
#define DEBUG_LED_EN                    OFF
#define DEBUG_ZCL_CB_EN                 OFF
#define DEBUG_ZB_CB_EN                  OFF
#define DEBUG_BDB_EN                    OFF
#define DEBUG_FINDBIND_EN               ON

#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PB1
#define DEBUG_BAUDRATE          115200
#endif /* UART_PRINTF_MODE */

/* PM */
#define PM_ENABLE						ON

/* PA */
#define PA_ENABLE						OFF

/* BDB */
#define TOUCHLINK_SUPPORT				OFF
#define FIND_AND_BIND_SUPPORT			ON

/* Board define */
#if defined(MCU_CORE_826x)
#if !PA_ENABLE
    #define BOARD                       BOARD_826x_DONGLE
#else
    #define BOARD                       BOARD_826x_DONGLE_PA
#endif
    #define CLOCK_SYS_CLOCK_HZ          32000000
#elif defined(MCU_CORE_8258)
#if (CHIP_TYPE == TLSR_8258_1M)
    #define FLASH_CAP_SIZE_1M           1
#endif
    #define CLOCK_SYS_CLOCK_HZ          48000000
    #define NV_ITEM_APP_USER_CFG        (NV_ITEM_APP_GP_TRANS_TABLE + 1)    // see sdk/proj/drivers/drv_nv.h
#elif defined(MCU_CORE_8278)
    #define FLASH_CAP_SIZE_1M           1
    #define BOARD                       BOARD_8278_DONGLE//BOARD_8278_EVK
    #define CLOCK_SYS_CLOCK_HZ          48000000
#elif defined(MCU_CORE_B91)
    #define FLASH_CAP_SIZE_1M           1
    #define BOARD                       BOARD_B91_DONGLE//BOARD_B91_EVK
    #define CLOCK_SYS_CLOCK_HZ          48000000
#else
    #error "MCU is undefined!"
#endif

///* Board include */
//#include "board_ts004x.h"
//
/* Voltage detect module */
/* If VOLTAGE_DETECT_ENABLE is set,
 * 1) if MCU_CORE_826x is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * and there is no need to configure the detection IO port;
 * 2) if MCU_CORE_8258 or MCU_CORE_8278 is defined, the DRV_ADC_VBAT_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be in a floating state.
 * 3) if MCU_CORE_B91 is defined, the DRV_ADC_BASE_MODE mode is used by default,
 * we need to configure the detection IO port, and the IO must be connected to the target under test,
 * such as VCC.
 */
#define VOLTAGE_DETECT_ENABLE                       ON
#if defined(MCU_CORE_826x)
    #define VOLTAGE_DETECT_ADC_PIN                  0
#elif defined(MCU_CORE_8258) || defined(MCU_CORE_8278)
    #define VOLTAGE_DETECT_ADC_PIN                  GPIO_PC5
#elif defined(MCU_CORE_B91)
    #define VOLTAGE_DETECT_ADC_PIN                  ADC_GPIO_PB0
#endif

/* Watch dog module */
#define MODULE_WATCHDOG_ENABLE						ON

/* UART module */
#define	MODULE_UART_ENABLE							OFF

#if (ZBHCI_USB_PRINT || ZBHCI_USB_CDC || ZBHCI_USB_HID || ZBHCI_UART)
	#define ZBHCI_EN								1
#endif

/**********************************************************************
 * ZCL cluster support setting
 */
#define ZCL_GROUP_SUPPORT                           ON
#define ZCL_SCENE_SUPPORT                           ON
#define ZCL_POWER_CFG_SUPPORT                       ON
#define ZCL_OTA_SUPPORT                             ON
#define ZCL_ON_OFF_SUPPORT                          ON
#define ZCL_ON_OFF_SWITCH_CFG_SUPPORT               ON
#define ZCL_MULTISTATE_INPUT_SUPPORT                ON
#define ZCL_LEVEL_CTRL_SUPPORT                      ON

/**********************************************************************
 * Stack configuration
 */
#include "stack_cfg.h"


/**********************************************************************
 * EV configuration
 */
typedef enum{
	EV_POLL_ED_DETECT,
	EV_POLL_HCI,
    EV_POLL_IDLE,
	EV_POLL_MAX,
}ev_poll_e;

#endif /* SRC_INCLUDE_APP_CFG_H_ */
