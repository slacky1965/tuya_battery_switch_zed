#ifndef SRC_COMMON_BOARD_TS004X_H_
#define SRC_COMMON_BOARD_TS004X_H_

/**************************** Configure UART ***************************************/
#if UART_PRINTF_MODE
#define DEBUG_INFO_TX_PIN       GPIO_PB1
#define DEBUG_BAUDRATE          115200
#endif /* UART_PRINTF_MODE */

/********************* Configure External Battery GPIO ******************************/
#define VOLTAGE_DETECT_PIN      GPIO_PC5

/***************************** Configure LED  ***************************************/

#define LED_ON                  1
#define LED_OFF                 0
#define LED1                    GPIO_PC4
#define PC4_FUNC                AS_GPIO
#define PC4_OUTPUT_ENABLE       ON
#define PC4_INPUT_ENABLE        OFF
#define PC4_DATA_OUT            LED_OFF

/************************* Configure BUTTON GPIO ***************************************/
#define MAX_BUTTON_NUM      6

#define BUTTON1_GPIO            GPIO_PC2
#define PC2_INPUT_ENABLE        ON
#define PC2_OUTPUT_ENABLE       OFF
#define PC2_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_10K

#define BUTTON2_GPIO            GPIO_PB5
#define PB5_INPUT_ENABLE        ON
#define PB5_OUTPUT_ENABLE       OFF
#define PB5_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PB5     PM_PIN_PULLUP_10K

#define BUTTON3_GPIO            GPIO_PC2
#define PC2_INPUT_ENABLE        ON
#define PC2_OUTPUT_ENABLE       OFF
#define PC2_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PC2     PM_PIN_PULLUP_10K

#define BUTTON4_GPIO            GPIO_PC3
#define PC3_INPUT_ENABLE        ON
#define PC3_OUTPUT_ENABLE       OFF
#define PC3_FUNC                AS_GPIO
#define PULL_WAKEUP_SRC_PC3     PM_PIN_PULLUP_10K

//#define BUTTON5_GPIO            GPIO_PB4
//#define PB4_INPUT_ENABLE        ON
//#define PB4_OUTPUT_ENABLE       OFF
//#define PB4_FUNC                AS_GPIO
//#define PULL_WAKEUP_SRC_PA0     PM_PIN_PULLUP_10K
//
//#define BUTTON6_GPIO            GPIO_PB4
//#define PB4_INPUT_ENABLE        ON
//#define PB4_OUTPUT_ENABLE       OFF
//#define PB4_FUNC                AS_GPIO
//#define PULL_WAKEUP_SRC_PA0     PM_PIN_PULLUP_10K

enum{
    VK_SW1 = 0x01,
    VK_SW2 = 0x02,
    VK_SW3 = 0x03,
    VK_SW4 = 0x04,
};

#define KB_MAP_NORMAL           { \
                                    {VK_SW1,}, \
                                    {VK_SW2,}, \
                                    {VK_SW3,}, \
                                    {VK_SW4,}, \
                                }

#define KB_MAP_NUM              KB_MAP_NORMAL
#define KB_MAP_FN               KB_MAP_NORMAL

#define KB_DRIVE_PINS           {0}
#define KB_SCAN_PINS            {BUTTON1_GPIO, BUTTON2_GPIO, BUTTON3_GPIO, BUTTON4_GPIO}


#endif /* SRC_COMMON_BOARD_TS004X_H_ */
