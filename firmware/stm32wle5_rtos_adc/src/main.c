/*
 * Copyright (c) 2023
 * Regis Rousseau, INSA Lyon, CITI Lab.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/sys/printk.h>
#include <zephyr/sys/util.h>
#include <zephyr/logging/log.h>
#include <zephyr/lorawan/lorawan.h>
#include <zephyr/storage/flash_map.h>
#include <zephyr/fs/nvs.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/flash.h>

#include "app_sensor.h"
#include "app_lorawan.h"
#include "app_nvs.h"

#if !DT_NODE_EXISTS(DT_PATH(zephyr_user)) || \
	!DT_NODE_HAS_PROP(DT_PATH(zephyr_user), io_channels)
#error "No suitable devicetree overlay specified"
#endif

LOG_MODULE_REGISTER(lorawan_class_a);

static void dl_callback(uint8_t port, bool data_pending,
			int16_t rssi, int8_t snr,
			uint8_t len, const uint8_t *data)
{
	LOG_INF("Port %d, Pending %d, RSSI %ddB, SNR %ddBm", port, data_pending, rssi, snr);
	if (data) {
		LOG_HEXDUMP_INF(data, len, "Payload: ");
	}
}

static void lorwan_datarate_changed(enum lorawan_datarate dr)
{
	uint8_t unused, max_size;

	lorawan_get_payload_sizes(&unused, &max_size);
	LOG_INF("New Datarate: DR_%d, Max Payload %d", dr, max_size);
}

int main(void)
{
    int ret;
	static struct nvs_fs fs;

    uint16_t gsone_val;
	uint16_t gsone_max_val = 1000; //to do 
    uint16_t payload[2];

	struct sensor_value bat_val;
	int rc;
	const struct device *const bat_dev = DEVICE_DT_GET_ONE(st_stm32_vbat);

    const struct device *lora_dev;
	struct lorawan_join_config join_cfg;
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;
	uint8_t join_eui[] = LORAWAN_JOIN_EUI;
	uint8_t app_key[] = LORAWAN_APP_KEY;

	struct lorawan_downlink_cb downlink_cb = {
		.port = LW_RECV_PORT_ANY,
		.cb = dl_callback
	};

	lora_dev = DEVICE_DT_GET(DT_ALIAS(lora0));
	if (!device_is_ready(lora_dev)) {
		LOG_ERR("%s: device not ready.", lora_dev->name);
		return 0;
	}

#if defined(CONFIG_LORAMAC_REGION_EU868)
	/* If more than one region Kconfig is selected, app should set region
	 * before calling lorawan_start()
	 */
	ret = lorawan_set_region(LORAWAN_REGION_EU868);
	if (ret < 0) {
		LOG_ERR("lorawan_set_region failed: %d", ret);
		return 0;
	}
#endif

	ret = lorawan_start();
	if (ret < 0) {
		LOG_ERR("lorawan_start failed: %d", ret);
		return 0;
	}

	lorawan_register_downlink_callback(&downlink_cb);
	lorawan_register_dr_changed_callback(lorwan_datarate_changed);

	join_cfg.mode = LORAWAN_ACT_OTAA;
	join_cfg.dev_eui = dev_eui;
	join_cfg.otaa.join_eui = join_eui;
	join_cfg.otaa.app_key = app_key;
	join_cfg.otaa.nwk_key = app_key;

	LOG_INF("Joining network over OTAA");
	ret = lorawan_join(&join_cfg);
	if (ret < 0) {
		LOG_ERR("lorawan_join_network failed: %d", ret);
		return 0;
	}

#ifdef CONFIG_LORAWAN_APP_CLOCK_SYNC
	lorawan_clock_sync_run();
#endif

	app_sensor_init();
	nvs_initialise(&fs);

	LOG_INF("Sending data...");

	if (!device_is_ready(bat_dev)) {
		printk("VBAT sensor is not ready\n");
		return;
	}

	do {
        gsone_val = app_sensor_read();
        payload[0] = gsone_val >> 8;
        payload[1] = gsone_val;

		/* fetch sensor samples */
		rc = sensor_sample_fetch(bat_dev);
		if (rc) {
			printk("Failed to fetch sample (%d)\n", rc);
			return;
		}

		rc = sensor_channel_get(bat_dev, SENSOR_CHAN_VOLTAGE, &bat_val);
		if (rc) {
			printk("Failed to get data (%d)\n", rc);
			return;
		}

		if (gsone_val > gsone_max_val) {

			ret = lorawan_send(2, (uint8_t *)&payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);
			
			if (ret == -EAGAIN) {
				LOG_ERR("lorawan_send failed: %d. Continuing...", ret);
				k_sleep(DELAY);
				continue;
			}

			if (ret < 0) {
				LOG_ERR("lorawan_send failed: %d", ret);
				return 0;
			}
			LOG_INF("Data sent!");
		}
		else {
			/* load data to memory flash*/
			(void)nvs_write_data(&fs, ADDRESS_ID, &gsone_val, sizeof(gsone_val));
			//nvs_write(&fs, ADDRESS_ID, &gsone_val, sizeof(gsone_val));
		}
		k_sleep(DELAY);
	}
	while (sensor_value_to_double(&bat_val) > 1.2);

return 0;
}