/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"
#include "app_nvs.h"

int8_t bat_isr_cur_ind;		// index used by Interrupt Service Routine

/* prepare the STM32 VBAT internal adc sample buffers and flash memory for periodic sampling */
int8_t app_vbat_init(const struct device *dev)
{
    static struct nvs_fs fs;
    
	 dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
    if (dev == NULL) {
        printk("error: no stm32 vbat device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: stm32 vbat is not ready\n");
		return 0;
	} else {
        printk("- found device: \"%s\", getting vbat data\n", dev->name);
    }

	bat_isr_cur_ind = 0;
    (void)nvs_delete(&fs, NVS_STM32_VBAT_ID);
    return 0;
}

/* Interrupt Service Routine for sampling the battery level data */
int8_t app_vbat_handler(const struct device *dev)
{
    int8_t ret;
    uint8_t val_mv[BAT_BUFFER_SIZE];
	struct sensor_value sp_chan1;
    static struct nvs_fs fs;

   dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
        ret = sensor_sample_fetch(dev);
        if (ret < 0 && ret != -EBADMSG) {        
	        printk("stm32 vbat sample is not up to date. error: %d\n", ret);
	        return 0;
        }

        ret = sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &sp_chan1);
        if (ret < 0) {
            printk("can't read sensor channels. error: %d\n", ret);
	        return 0;
        }

        printk("- %s, current voltage: %d.%2d\n", dev->name, sp_chan1.val1, sp_chan1.val2);

    if (bat_isr_cur_ind < BAT_BUFFER_SIZE) {
        val_mv[bat_isr_cur_ind] = (sensor_value_to_milli(&sp_chan1)/10) >> 8;
		val_mv[bat_isr_cur_ind+1] = (sensor_value_to_milli(&sp_chan1)/10);
		bat_isr_cur_ind++;
        if (bat_isr_cur_ind == BAT_BUFFER_SIZE) {
            (void)nvs_write(&fs, NVS_STM32_VBAT_ID, &val_mv, sizeof(val_mv));
            bat_isr_cur_ind = 0;
        }
    }
    return 0;
}
