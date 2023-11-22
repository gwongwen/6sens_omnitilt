/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_rtc.h"
#include "app_nvs.h"

int8_t app_rtc_init(const struct device *dev)
{
    dev = DEVICE_DT_GET(TIMER);   // get sensor device

    if (dev == NULL) {
        printk("no timer device found. error: %d\n", dev);
		return 0;
	}
    if (!device_is_ready(dev)) {
		printk("timer is not ready. error: %d\n", dev);
		return 0;
	}
    printk("- found device: \"%s\", getting rtc time\n", dev->name);
    return 0;
}

int8_t app_rtc_handler(const struct device *dev)
{
    static struct nvs_fs fs;
    int8_t ret = 0;

    time_t compile_timestamp = 1678984720;//11699437198; // timestamp local time GMT+1
    struct tm   ttm;
    //struct timespec time_now_ts;
    
    dev = DEVICE_DT_GET(TIMER);

    uint32_t *ticks = k_malloc(sizeof(uint32_t));
    memset(ticks, 0, sizeof(uint32_t));

    ret = counter_get_value(dev, ticks);

    if(ret < 0) {
        printk("counter get value failed.\n\r");
        return 0;
    } else {
            time_t time_now = compile_timestamp + *ticks + 3600;
            ttm = * gmtime(&time_now);
            printk("counter value: %d\n\r", *ticks);
            printk("current time: %02d:%02d:%02d\n\r", ttm.tm_hour, ttm.tm_min, ttm.tm_sec);
        }
    //nvs_write(&fs, TIMER_RTC_ID , &ttm, sizeof(ttm));
    return 0;
}



