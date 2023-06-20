#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#include "app_sensor.h"

uint16_t temp_data[8];

/* Data of ADC io-channels specified in devicetree. */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

struct adc_sequence sequence = {
		.buffer = &temp_data,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(temp_data),
	};

int app_sensor_init(void) {
	int err;

	/* Configure channels individually prior to sampling. */
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
		if (!device_is_ready(adc_channels[i].dev)) {
			printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
			return 0;
		}

		err = adc_channel_setup_dt(&adc_channels[i]);
		if (err < 0) {
			printk("Could not setup channel #%d (%d)\n", i, err);
			return 0;
		}
	}
	return 0;
}

uint16_t app_sensor_read(void) {
	int err;
	
	for (size_t i = 0U; i < ARRAY_SIZE(adc_channels); i++) {
			printk("- %s, channel %d: ",
			       adc_channels[i].dev->name,
			       adc_channels[i].channel_id);

			(void)adc_sequence_init_dt(&adc_channels[i], &sequence);

			err = adc_read(adc_channels[i].dev, &sequence);
			if (err < 0) {
				printk("Could not read (%d)\n", err);
				continue;
			}
		return temp_data[0];	
	}
}
