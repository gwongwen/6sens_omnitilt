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
	static struct nvs_fs fs;
	const struct device *lora_dev;
	int8_t ret;
	uint32_t val;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;

	struct payload_form {
		uint8_t id;
		char *bat;
		int32_t bat_val;
		char *temp;
		int32_t temp_val;
		char *press;
		int32_t press_val;
		char *hum;
		int32_t hum_val;
	};
	
	struct payload_form payload;
	payload.id = dev_eui;
	payload.bat = "B";
	payload.temp = "T";
	payload.press = "P";
	payload.hum = "H";

	/* 1 sample per 15min -> 96 samples per a day */
	for (int8_t i = 0; i < 95; i++) {
		ret = nvs_read(&fs, NVS_STM32_VBAT_ID, &val, sizeof(val));
		payload.bat_val = val;
		ret = nvs_read(&fs, NVS_BME280_TEMP_ID, &val, sizeof(val));
		payload.temp_val = val;
		ret = nvs_read(&fs, NVS_BME280_PRES_ID, &val, sizeof(val));
		payload.press_val = val;
		ret = nvs_read(&fs, NVS_BME280_HUM_ID, &val, sizeof(val));
		payload.hum_val = val;
	
		ret = lora_send(lora_dev, (uint8_t *)&payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);
		if (ret < 0) {
			printk("LoRa send failed\n");
			return 0;
		}
		k_sleep(K_MSEC(5000));
	}

	(void)nvs_delete(&fs, NVS_STM32_VBAT_ID);
	(void)nvs_delete(&fs, NVS_BME280_TEMP_ID);
    (void)nvs_delete(&fs, NVS_BME280_PRES_ID);
    (void)nvs_delete(&fs, NVS_BME280_HUM_ID);
}
K_WORK_DEFINE(nvs_work, nvs_work_handler);

void nvs_timer_handler(struct k_timer *nvs_dum)
{
	k_work_submit(&nvs_work);
}
K_TIMER_DEFINE(nvs_timer, nvs_timer_handler, NULL);

void btph_work_handler(struct k_work *work_btph)
{
	printk("BTPH handler called\n");

	const struct device *bat_dev = NULL;
	const struct device *bme280_dev = NULL;
	
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

		ret = lora_send(lora_dev, &payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);
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

/*	app_nvs_init(&fs);
	app_rtc_init(timer_rtc_dev);
	app_vbat_init(bat_dev);
	app_bme280_init(bme280_dev);
	app_adc_init();
	printk("\n");
*/
	app_lorawan_init(lora_dev);
	time_t get_time = app_lorawan_get_time(lora_dev);
	
//	k_timer_start(&adc_timer, K_MSEC(1000), K_MSEC(1000));
//	k_timer_start(&nvs_timer, K_MSEC(20000), K_MSEC(20000));
//	k_timer_start(&bpth_timer, K_MSEC(30000), K_MSEC(30000));
	return 0;
}