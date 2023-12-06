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
    static struct nvs_fs fs;
    
    dev = DEVICE_DT_GET_ANY(bosch_bme280);
    if (dev == NULL) {
        printk("error: no bme280 device found\n");
		return 0;
	}

    if (!device_is_ready(dev)) {
		printk("error: bme280 is not ready\n");
		return 0;
	} else {
        printk("- found device: \"%s\", getting bme280 data\n", dev->name);
    }
    
    (void)nvs_delete(&fs, NVS_BME280_TEMP_ID);
    (void)nvs_delete(&fs, NVS_BME280_PRES_ID);
    (void)nvs_delete(&fs, NVS_BME280_HUM_ID);
    return 0;
}

int8_t app_bme280_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    struct sensor_value temp;
    struct sensor_value press;
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
    }printk(sizeof(temp));
    (void)nvs_write(&fs, NVS_BME280_TEMP_ID, &temp, sizeof(temp));

    ret = sensor_channel_get(dev, SENSOR_CHAN_PRESS, &press);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }
   (void)nvs_write(&fs, NVS_BME280_PRES_ID, &temp, sizeof(temp));

	ret = sensor_channel_get(dev, SENSOR_CHAN_HUMIDITY, &hum);
    if (ret < 0) {
        printk("can't read sensor channels. error: %d\n", ret);
	    return 0;
    }
   (void)nvs_write(&fs, NVS_BME280_HUM_ID, &hum, sizeof(hum));
    printk("- temp: %d.%6d degC; press = %d.%6d kpa; hum: %d.%6d percent\n", temp.val1, temp.val2, press.val1, press.val2, hum.val1, hum.val2);
    return 0;
}