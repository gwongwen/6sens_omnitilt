/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_vbat.h"
#include "app_nvs.h"

int8_t app_vbat_init(const struct device *dev)
{
    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);
    if (dev == NULL) {
        printk("error: no stm32 vbat device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: stm32 vbat is not ready\n");
		return 0;
	}
    printk("- found device: \"%s\", getting vbat data\n", dev->name);
    return 0;
}

int8_t app_vbat_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value sp_buf_ch14;
    int8_t ret;
    int8_t i = 0;
    float val_mv;

    dev = DEVICE_DT_GET_ONE(st_stm32_vbat);

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) {        
	    printk("error: stm32 vbat sample is not up to date\n");
	    return 0;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_VOLTAGE, &sp_buf_ch14);
    if (ret < 0) {
        printk("error: can't read sensor channels\n");
	    return 0;
    }
    //nvs_write(&fs, STM32_VBAT_ID, &bat, sizeof(bat));
    printk("- %s, current voltage: %d.%2d\n", dev->name, sp_buf_ch14.val1, sp_buf_ch14.val2);
    return 0;
}

