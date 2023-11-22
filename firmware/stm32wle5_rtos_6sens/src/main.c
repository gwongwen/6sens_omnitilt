/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
 

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <stdio.h>

#include "app_bme280.h"
#include "app_nvs.h"
#include "app_rtc.h"
#include "app_adc.h"
#include "app_vbat.h"
#include "app_lorawan.h"

void rtc_work_handler(struct k_work *work_rtc)
{
	printk("RTC handler called\n");

	const struct device *bme280_dev = NULL;
	const struct device *bat_dev = NULL;
	const struct device *timer_rtc_dev = NULL;

	app_rtc_handler(timer_rtc_dev);
	//app_bme280_handler(bme280_dev);
	//app_vbat_handler(bat_dev);
}
K_WORK_DEFINE(rtc_work, rtc_work_handler);

void rtc_timer_handler(struct k_timer *rtc_dum)
{
	k_work_submit(&rtc_work);
}
K_TIMER_DEFINE(rtc_timer, rtc_timer_handler, NULL);

void adc_work_handler(struct k_work *work_adc)
{
	printk("ADC handler called\n");
	uint16_t raw_val = app_adc_handler();
	uint8_t payload[2];

	payload[0] = raw_val >> 8;
	payload[1] = raw_val;
	
	printk("- MSB payload: %d, LSB payload: %d\n", payload[0], payload[1]);
}
K_WORK_DEFINE(adc_work, adc_work_handler);

void adc_timer_handler(struct k_timer *adc_dum)
{
    k_work_submit(&adc_work);
}
K_TIMER_DEFINE(adc_timer, adc_timer_handler, NULL);

int main(void)
{
	static struct nvs_fs fs;
	const struct device *bme280_dev = NULL;
	const struct device *bat_dev = NULL;
	const struct device *timer_rtc_dev = NULL;
	const struct device *lora_dev = NULL;

	time_t get_time;

	app_lorawan_init(lora_dev);
	app_nvs_init(&fs);
	app_rtc_init(timer_rtc_dev);
	app_bme280_init(bme280_dev);
	app_vbat_init(bat_dev);
	app_adc_init();
	printk("\n");
	
	get_time = app_lorawan_get_time(lora_dev);
	printk("\n");

	//k_timer_start(&rtc_timer, K_MSEC(1000), K_MSEC(1000));
	//k_timer_start(&adc_timer, K_MSEC(1000), K_MSEC(1000));
	return 0;
}