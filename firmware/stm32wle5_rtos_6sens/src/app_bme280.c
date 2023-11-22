/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

 #include "app_bme280.h"
 #include "app_nvs.h"

int8_t app_bme280_init(const struct device *dev)
{
    dev = DEVICE_DT_GET_ANY(bosch_bme280);
    if (dev == NULL) {
        printk("error: no bme280 device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: bme280 is not ready\n");
		return 0;
	}
    printk("- found device: \"%s\", getting bme280 data\n", dev->name);
    return 0;
}

int8_t app_bme280_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value temp;
    struct sensor_value hum;
    int8_t ret;

    dev = DEVICE_DT_GET_ANY(bosch_bme280);

	ret = sensor_sample_fetch(dev);
    if (ret < 0 && ret != -EBADMSG) { 
	    printk("bme280 device sample is not up to date. error: %d\n", ret);
	    return 0;
    }

	ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temp);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }
    //nvs_write(&fs, BME280_TEMP_ID, &temp, sizeof(temp));

	ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }
    //nvs_write(&fs, BME280_HUM_ID, &hum, sizeof(hum));
    printk("- temp: %d.%2d degC; hum: %d.%2d percent\n", temp.val1, temp.val2, hum.val1, hum.val2);
    return 0;
}