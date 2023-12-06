/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_nvs.h"
#include "app_lorawan.h"

int8_t app_nvs_init(struct nvs_fs *fs)
{
	struct flash_pages_info info;
	int8_t ret;

	fs->flash_device = NVS_PARTITION_DEVICE;
	if (!device_is_ready(fs->flash_device)) {
		printk("flash device %s is not ready\n", fs->flash_device->name);
		return 0;
	} else {
		printk("- found device: \"%s\", getting nvs memory\n", fs->flash_device->name);
	}

	fs->offset = NVS_PARTITION_OFFSET;
	ret = flash_get_page_info_by_offs(fs->flash_device, fs->offset, &info);
	if (ret) {
		printk("unable to get page info. error: %d\n", ret);
		return 0;
	}

	fs->sector_size = info.size;
	if (!fs->sector_size || fs->sector_size % info.size) {
		printk("invalid sector size\n");
		return 0;
	}

	fs->sector_count = 2U;
	ret = nvs_mount(fs);
	if (ret) {
		printk("flash init failed. error: %d\n", ret);
		return 0;
	}
	return 0;
}

int8_t app_nvs_init_param(struct nvs_fs *fs, uint16_t id, void *data)
{
	int8_t ret;

	ret = nvs_read(fs, id, data, sizeof(data));
	if (ret > 0) {
		printk("ID: %d, address: %s\n", id, data);
	} else {
		printk("no address found, adding %s at id %d\n", data, id);
		(void)nvs_write(fs, id, data, sizeof(data));	
	}
	return 0;
}

int8_t app_nvs_handler(struct nvs_fs *fs, const struct device *dev)
{
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
	
		ret = lora_send(dev, (uint8_t *)&payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);
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

	return 0;
}