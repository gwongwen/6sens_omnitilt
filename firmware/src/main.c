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

void nvs_work_handler(struct k_work *work_nvs)
{
	const struct device *lora_dev = NULL;

	printk("NVS handler called\n");

	app_nvs_handler(lora_dev);
}
K_WORK_DEFINE(nvs_work, nvs_work_handler);

void nvs_timer_handler(struct k_timer *nvs_dum)
{
	k_work_submit(&nvs_work);
}
K_TIMER_DEFINE(nvs_timer, nvs_timer_handler, NULL);

void btph_work_handler(struct k_work *work_btph)
{
	const struct device *bat_dev = NULL;
	const struct device *bme280_dev = NULL;

	printk("BTPH handler called\n");
	
	app_vbat_handler(bat_dev);
	app_bme280_handler(bme280_dev);
}
K_WORK_DEFINE(btph_work, btph_work_handler);

void btph_timer_handler(struct k_timer *btph_dum)
{
	k_work_submit(&btph_work);
}
K_TIMER_DEFINE(btph_timer, btph_timer_handler, NULL);

void adc_work_handler(struct k_work *work_adc)
{
	printk("ADC handler called\n");
	int8_t ret;
	uint16_t raw_val;
	uint8_t buffer[40];
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	const struct device *lora_dev;

	struct payload_form {
		uint8_t id;
		uint8_t val;
	};
	struct payload_form payload;
	payload.id = dev_eui;
	payload.val = buffer;

	raw_val = app_adc_handler(); 

	if (raw_val < GEODATA_DC_OFFSET ) {
		for (int8_t i = 0; i < BUFFER_SIZE; i++) {
			buffer[i] = raw_val >> 8;
			buffer[i+1] = raw_val;
		}
		ret = lorawan_send(lora_dev, &payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);
		if (ret < 0) {
			printk("LoRa send failed\n");
			return 0;
		}
	}
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
	const struct device *timer_rtc_dev = NULL;
	const struct device *bat_dev = NULL;
	const struct device *bme280_dev = NULL;
	const struct device *lora_dev = NULL;

	app_nvs_init(&fs);
	app_rtc_init(timer_rtc_dev);
	app_vbat_init(bat_dev);
	app_bme280_init(bme280_dev);
	app_adc_init();
	printk("\n");

//	app_lorawan_init(lora_dev);
//	time_t get_time = app_lorawan_get_time(lora_dev);
	
	k_timer_start(&adc_timer, K_MSEC(5), K_MSEC(5));
	k_timer_start(&btph_timer, K_MSEC(5000), K_MSEC(5000));
	k_timer_start(&nvs_timer, K_MSEC(5000*48), K_MSEC(5000*48));
	return 0;
}