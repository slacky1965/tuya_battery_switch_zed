//#ifndef SRC_INCLUDE_APP_GPIO_H_
//#define SRC_INCLUDE_APP_GPIO_H_

#pragma once

#include "app_scene.h"
#include "app_device.h"

static inline void app_gpio_analog_resistance_init(void)
{
    analog_write (0x0e,  PULL_WAKEUP_SRC_PA0 |
                        (PULL_WAKEUP_SRC_PA1<<2) |
                        (PULL_WAKEUP_SRC_PA2<<4) |
                        (PULL_WAKEUP_SRC_PA3<<6));

    analog_write (0x0f,  PULL_WAKEUP_SRC_PA4 |
                        (PULL_WAKEUP_SRC_PA5<<2) |
                        (PULL_WAKEUP_SRC_PA6<<4) |
                        (PULL_WAKEUP_SRC_PA7<<6));


    analog_write (0x10,  PULL_WAKEUP_SRC_PB0 |
                        (PULL_WAKEUP_SRC_PB1<<2) |
                        (PULL_WAKEUP_SRC_PB2<<4) |
                        (PULL_WAKEUP_SRC_PB3<<6));

    analog_write (0x11,  PULL_WAKEUP_SRC_PB4 |
                        (PULL_WAKEUP_SRC_PB5<<2) |
                        (PULL_WAKEUP_SRC_PB6<<4) |
                        (PULL_WAKEUP_SRC_PB7<<6));


    analog_write (0x12,  PULL_WAKEUP_SRC_PC0 |
                        (PULL_WAKEUP_SRC_PC1<<2) |
                        (PULL_WAKEUP_SRC_PC2<<4) |
                        (PULL_WAKEUP_SRC_PC3<<6));

    analog_write (0x13,  PULL_WAKEUP_SRC_PC4 |
                        (PULL_WAKEUP_SRC_PC5<<2) |
                        (PULL_WAKEUP_SRC_PC6<<4) |
                        (PULL_WAKEUP_SRC_PC7<<6));


    analog_write (0x14,  PULL_WAKEUP_SRC_PD0 |
                        (PULL_WAKEUP_SRC_PD1<<2) |
                        (PULL_WAKEUP_SRC_PD2<<4) |
                        (PULL_WAKEUP_SRC_PD3<<6));

    analog_write (0x15,  PULL_WAKEUP_SRC_PD4 |
                        (PULL_WAKEUP_SRC_PD5<<2) |
                        (PULL_WAKEUP_SRC_PD6<<4) |
                        (PULL_WAKEUP_SRC_PD7<<6));
}



static inline void app_gpio_init(int anaRes_init_en) {

    if (*(uint32_t*)(DEVICE_MODEL_CFG_ADDR) == ID_DEVICE_MODEL_CFG) {
        uint8_t model = *(uint8_t*)(DEVICE_MODEL_CFG_ADDR+4);
        if (model >= DEVICE_MODEL_1 && model < DEVICE_MODEL_MAX) {
            model_in_flash = true;
            device_model = model;
        } else {
            model_in_flash = false;
            device_model = DEVICE_MODEL;
        }
    } else {
        model_in_flash = false;
        device_model = DEVICE_MODEL;
    }

    if (device_model == DEVICE_MODEL_1) {

        /* TS0041 TZ3000_an5rjiwd Tuya  - model_1 */

        reg_gpio_pa_setting1 = 0xFF8080;
        reg_gpio_pa_setting2 = 0x7FFF00;

        //PB group
        //ie
        analog_write(areg_gpio_pb_ie, 0);
        //oen
        reg_gpio_pb_oen = 0xFD;
        //dataO
        reg_gpio_pb_out = 0x02;
        //ds
        analog_write(areg_gpio_pb_ds, 0xFF);
        //func
        reg_gpio_pb_gpio = 0xFF;

        //PC group
        //ie
        analog_write(areg_gpio_pc_ie, 0x04);
        //oen
        reg_gpio_pc_oen = 0xCF;
        //dataO
        reg_gpio_pc_out = 0x20;
        //ds
        analog_write(areg_gpio_pc_ds, 0xFF);
        reg_gpio_pc_gpio = 0xFF;

        //PD group
        reg_gpio_pd_setting1 = 0xFF0000;
        reg_gpio_pd_setting2 = 0xFFFF00;

    } else if (device_model == DEVICE_MODEL_2) {

        /* TS0601 TZ3000_ja5osu5g Loginovo  - model_2 */

        reg_gpio_pa_setting1 = 0xFF8080;
        reg_gpio_pa_setting2 = 0x7FFF00;

        //PB group
        //ie
        analog_write(areg_gpio_pb_ie, 0x40);
        //oen
        reg_gpio_pb_oen = 0xFD;
        //dataO
        reg_gpio_pb_out = 0x02;
        //ds
        analog_write(areg_gpio_pb_ds, 0xFF);
        //func
        reg_gpio_pb_gpio = 0xFF;

        //PC group
        //ie
        analog_write(areg_gpio_pc_ie, 0x00);
        //oen
        reg_gpio_pc_oen = 0xCF;
        //dataO
        reg_gpio_pc_out = 0x30;
        //ds
        analog_write(areg_gpio_pc_ds, 0xFF);
        reg_gpio_pc_gpio = 0xFF;

        //PD group
        reg_gpio_pd_setting1 = 0xFF0000;
        reg_gpio_pd_setting2 = 0xFFFF00;

    } else if (device_model == DEVICE_MODEL_3) {

        /* TS0042 _TZ3000_h68nee3e Tuya (EKF)  - model_3 */

        reg_gpio_pa_setting1 = 0xFF8282;
        reg_gpio_pa_setting2 = 0x7FFF00;

        //PB group
        //ie
        analog_write(areg_gpio_pb_ie, 0x20);
        //oen
        reg_gpio_pb_oen = 0xFD;
        //dataO
        reg_gpio_pb_out = 0x02;
        //ds
        analog_write(areg_gpio_pb_ds, 0xFF);
        //func
        reg_gpio_pb_gpio = 0xFF;

        //PC group
        //ie
        analog_write(areg_gpio_pc_ie, 0x00);
        //oen
        reg_gpio_pc_oen = 0xD9;
        //dataO
        reg_gpio_pc_out = 0x20;
        //ds
        analog_write(areg_gpio_pc_ds, 0xFF);
        reg_gpio_pc_gpio = 0xFF;

        //PD group
        reg_gpio_pd_setting1 = 0xFF0000;
        reg_gpio_pd_setting2 = 0xFFFF00;

//    } else if (device_model == DEVICE_MODEL_4) {
//    } else if (device_model == DEVICE_MODEL_5) {
//    } else if (device_model == DEVICE_MODEL_6) {
    } else {
        reg_gpio_pa_setting1 =
            (PA0_INPUT_ENABLE<<8)   | (PA1_INPUT_ENABLE<<9) | (PA2_INPUT_ENABLE<<10)    | (PA3_INPUT_ENABLE<<11) |
            (PA4_INPUT_ENABLE<<12)  | (PA5_INPUT_ENABLE<<13)    | (PA6_INPUT_ENABLE<<14)    | (PA7_INPUT_ENABLE<<15) |
            ((PA0_OUTPUT_ENABLE?0:1)<<16)   | ((PA1_OUTPUT_ENABLE?0:1)<<17) | ((PA2_OUTPUT_ENABLE?0:1)<<18) | ((PA3_OUTPUT_ENABLE?0:1)<<19) |
            ((PA4_OUTPUT_ENABLE?0:1)<<20)   | ((PA5_OUTPUT_ENABLE?0:1)<<21) | ((PA6_OUTPUT_ENABLE?0:1)<<22) | ((PA7_OUTPUT_ENABLE?0:1)<<23) |
            (PA0_DATA_OUT<<24)  | (PA1_DATA_OUT<<25)    | (PA2_DATA_OUT<<26)    | (PA3_DATA_OUT<<27) |
            (PA4_DATA_OUT<<28)  | (PA5_DATA_OUT<<29)    | (PA6_DATA_OUT<<30)    | (PA7_DATA_OUT<<31) ;
        reg_gpio_pa_setting2 =
            (PA0_DATA_STRENGTH<<8)      | (PA1_DATA_STRENGTH<<9)| (PA2_DATA_STRENGTH<<10)   | (PA3_DATA_STRENGTH<<11) |
            (PA4_DATA_STRENGTH<<12) | (PA5_DATA_STRENGTH<<13)   | (PA6_DATA_STRENGTH<<14)   | (PA7_DATA_STRENGTH<<15) |
            (PA0_FUNC==AS_GPIO ? BIT(16):0) | (PA1_FUNC==AS_GPIO ? BIT(17):0)| (PA2_FUNC==AS_GPIO ? BIT(18):0)| (PA3_FUNC==AS_GPIO ? BIT(19):0) |
            (PA4_FUNC==AS_GPIO ? BIT(20):0) | (PA5_FUNC==AS_GPIO ? BIT(21):0)| (PA6_FUNC==AS_GPIO ? BIT(22):0)| (PA7_FUNC==AS_GPIO ? BIT(23):0);

        //PB group
        //ie
        analog_write(areg_gpio_pb_ie,   (PB0_INPUT_ENABLE<<0)   | (PB1_INPUT_ENABLE<<1) | (PB2_INPUT_ENABLE<<2) | (PB3_INPUT_ENABLE<<3) |
                                        (PB4_INPUT_ENABLE<<4)   | (PB5_INPUT_ENABLE<<5) | (PB6_INPUT_ENABLE<<6) | (PB7_INPUT_ENABLE<<7) );
        //oen
        reg_gpio_pb_oen =
            ((PB0_OUTPUT_ENABLE?0:1)<<0)    | ((PB1_OUTPUT_ENABLE?0:1)<<1) | ((PB2_OUTPUT_ENABLE?0:1)<<2)   | ((PB3_OUTPUT_ENABLE?0:1)<<3) |
            ((PB4_OUTPUT_ENABLE?0:1)<<4)    | ((PB5_OUTPUT_ENABLE?0:1)<<5) | ((PB6_OUTPUT_ENABLE?0:1)<<6)   | ((PB7_OUTPUT_ENABLE?0:1)<<7);
        //dataO
        reg_gpio_pb_out =
            (PB0_DATA_OUT<<0)   | (PB1_DATA_OUT<<1) | (PB2_DATA_OUT<<2) | (PB3_DATA_OUT<<3) |
            (PB4_DATA_OUT<<4)   | (PB5_DATA_OUT<<5) | (PB6_DATA_OUT<<6) | (PB7_DATA_OUT<<7) ;

        //ds
        analog_write(areg_gpio_pb_ds,   (PB0_DATA_STRENGTH<<0)  | (PB1_DATA_STRENGTH<<1)  | (PB2_DATA_STRENGTH<<2)  | (PB3_DATA_STRENGTH<<3) |
                                        (PB4_DATA_STRENGTH<<4)  | (PB5_DATA_STRENGTH<<5)  | (PB6_DATA_STRENGTH<<6)  | (PB7_DATA_STRENGTH<<7) );
        //func
        reg_gpio_pb_gpio =
            (PB0_FUNC==AS_GPIO ? BIT(0):0)  | (PB1_FUNC==AS_GPIO ? BIT(1):0)| (PB2_FUNC==AS_GPIO ? BIT(2):0)| (PB3_FUNC==AS_GPIO ? BIT(3):0) |
            (PB4_FUNC==AS_GPIO ? BIT(4):0)  | (PB5_FUNC==AS_GPIO ? BIT(5):0)| (PB6_FUNC==AS_GPIO ? BIT(6):0)| (PB7_FUNC==AS_GPIO ? BIT(7):0);

        //PC group
        //ie
        analog_write(areg_gpio_pc_ie,   (PC0_INPUT_ENABLE<<0)   | (PC1_INPUT_ENABLE<<1) | (PC2_INPUT_ENABLE<<2) | (PC3_INPUT_ENABLE<<3) |
                                        (PC4_INPUT_ENABLE<<4)   | (PC5_INPUT_ENABLE<<5) | (PC6_INPUT_ENABLE<<6) | (PC7_INPUT_ENABLE<<7) );

        //oen
        reg_gpio_pc_oen =
            ((PC0_OUTPUT_ENABLE?0:1)<<0)    | ((PC1_OUTPUT_ENABLE?0:1)<<1) | ((PC2_OUTPUT_ENABLE?0:1)<<2)   | ((PC3_OUTPUT_ENABLE?0:1)<<3) |
            ((PC4_OUTPUT_ENABLE?0:1)<<4)    | ((PC5_OUTPUT_ENABLE?0:1)<<5) | ((PC6_OUTPUT_ENABLE?0:1)<<6)   | ((PC7_OUTPUT_ENABLE?0:1)<<7);
        //dataO
        reg_gpio_pc_out =
            (PC0_DATA_OUT<<0)   | (PC1_DATA_OUT<<1) | (PC2_DATA_OUT<<2) | (PC3_DATA_OUT<<3) |
            (PC4_DATA_OUT<<4)   | (PC5_DATA_OUT<<5) | (PC6_DATA_OUT<<6) | (PC7_DATA_OUT<<7) ;

        //ds
        analog_write(areg_gpio_pc_ds,   (PC0_DATA_STRENGTH<<0)  | (PC1_DATA_STRENGTH<<1)  | (PC2_DATA_STRENGTH<<2)  | (PC3_DATA_STRENGTH<<3) |
                                        (PC4_DATA_STRENGTH<<4)  | (PC5_DATA_STRENGTH<<5)  | (PC6_DATA_STRENGTH<<6)  | (PC7_DATA_STRENGTH<<7) );

        reg_gpio_pc_gpio =
            (PC0_FUNC==AS_GPIO ? BIT(0):0)  | (PC1_FUNC==AS_GPIO ? BIT(1):0)| (PC2_FUNC==AS_GPIO ? BIT(2):0)| (PC3_FUNC==AS_GPIO ? BIT(3):0) |
            (PC4_FUNC==AS_GPIO ? BIT(4):0)  | (PC5_FUNC==AS_GPIO ? BIT(5):0)| (PC6_FUNC==AS_GPIO ? BIT(6):0)| (PC7_FUNC==AS_GPIO ? BIT(7):0);

        //PD group
        reg_gpio_pd_setting1 =
            (PD0_INPUT_ENABLE<<8)   | (PD1_INPUT_ENABLE<<9) | (PD2_INPUT_ENABLE<<10)    | (PD3_INPUT_ENABLE<<11) |
            (PD4_INPUT_ENABLE<<12)  | (PD5_INPUT_ENABLE<<13)| (PD6_INPUT_ENABLE<<14)    | (PD7_INPUT_ENABLE<<15) |
            ((PD0_OUTPUT_ENABLE?0:1)<<16)   | ((PD1_OUTPUT_ENABLE?0:1)<<17) | ((PD2_OUTPUT_ENABLE?0:1)<<18) | ((PD3_OUTPUT_ENABLE?0:1)<<19) |
            ((PD4_OUTPUT_ENABLE?0:1)<<20)   | ((PD5_OUTPUT_ENABLE?0:1)<<21) | ((PD6_OUTPUT_ENABLE?0:1)<<22) | ((PD7_OUTPUT_ENABLE?0:1)<<23) |
            (PD0_DATA_OUT<<24)  | (PD1_DATA_OUT<<25)    | (PD2_DATA_OUT<<26)    | (PD3_DATA_OUT<<27) |
            (PD4_DATA_OUT<<28)  | (PD5_DATA_OUT<<29)    | (PD6_DATA_OUT<<30)    | (PD7_DATA_OUT<<31) ;
        reg_gpio_pd_setting2 =
            (PD0_DATA_STRENGTH<<8)  | (PD1_DATA_STRENGTH<<9)    | (PD2_DATA_STRENGTH<<10)   | (PD3_DATA_STRENGTH<<11) |
            (PD4_DATA_STRENGTH<<12) | (PD5_DATA_STRENGTH<<13)   | (PD6_DATA_STRENGTH<<14)   | (PD7_DATA_STRENGTH<<15) |
            (PD0_FUNC==AS_GPIO ? BIT(16):0) | (PD1_FUNC==AS_GPIO ? BIT(17):0)| (PD2_FUNC==AS_GPIO ? BIT(18):0)| (PD3_FUNC==AS_GPIO ? BIT(19):0) |
            (PD4_FUNC==AS_GPIO ? BIT(20):0) | (PD5_FUNC==AS_GPIO ? BIT(21):0)| (PD6_FUNC==AS_GPIO ? BIT(22):0)| (PD7_FUNC==AS_GPIO ? BIT(23):0);
    }



    //PE group
    reg_gpio_pe_ie = (PE0_INPUT_ENABLE<<0)  | (PE1_INPUT_ENABLE<<1)| (PE2_INPUT_ENABLE<<2)  | (PE3_INPUT_ENABLE<<3);
    reg_gpio_pe_oen = ((PE0_OUTPUT_ENABLE?0:1)<<0)  | ((PE1_OUTPUT_ENABLE?0:1)<<1) | ((PE2_OUTPUT_ENABLE?0:1)<<2)   | ((PE3_OUTPUT_ENABLE?0:1)<<3);
    reg_gpio_pe_out = (PE0_DATA_OUT<<0) | (PE1_DATA_OUT<<1) | (PE2_DATA_OUT<<2) | (PE3_DATA_OUT<<3);
    reg_gpio_pe_ds = (PE0_DATA_STRENGTH<<0) | (PE1_DATA_STRENGTH<<1)    | (PE2_DATA_STRENGTH<<2)    | (PE3_DATA_STRENGTH<<3);
    reg_gpio_pe_gpio = (PE0_FUNC==AS_GPIO ? BIT(0):0)   | (PE1_FUNC==AS_GPIO ? BIT(1):0)| (PE2_FUNC==AS_GPIO ? BIT(2):0)| (PE3_FUNC==AS_GPIO ? BIT(3):0);

    if(anaRes_init_en) {
        app_gpio_analog_resistance_init();
        if (device_model == DEVICE_MODEL_1) {
            // WakeUp src PB0-PB3
            analog_write (0x10, 0x04);
            // WakeUp src PC0-PC3
            analog_write (0x12, 0x50);
        } else if (device_model == DEVICE_MODEL_2) {
            // WakeUp src PB0-PB3
            analog_write (0x10, 0x04);
            // WakeUp src PB4-PB7
            analog_write (0x11, 0x10);
        } else if (device_model == DEVICE_MODEL_3) {
            // WakeUp src PA0-PA3
            analog_write (0x0e, 0x04);
            // WakeUp src PA4-PA7
            analog_write (0x0f, 0x40);
            // WakeUp src PB0-PB3
            analog_write (0x10, 0x04);
            // WakeUp src PB4-PB7
            analog_write (0x11, 0x04);
//        } else if (device_model == DEVICE_MODEL_4) {
        }
    }
}

//#endif /* SRC_INCLUDE_APP_GPIO_H_ */
