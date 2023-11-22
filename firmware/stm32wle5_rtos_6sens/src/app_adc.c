/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_adc.h"

uint16_t sp_buf_ch13;

static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels, DT_SPEC_AND_COMMA)
};

struct adc_sequence adc_ch13_seq = {
	.buffer 		= &sp_buf_ch13,
	.buffer_size	= sizeof(sp_buf_ch13),
};

int8_t app_adc_init(void)
{
	int8_t i = 0;

	if (!device_is_ready(adc_channels[i].dev)) {
		printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
		return 0;
	}
	printk("- found device: \"%s\", getting analog data\n", adc_channels[i].dev->name);
	return 0;
}

uint16_t app_adc_handler(void)
{
	int8_t err;
	int8_t i = 0;

	err = adc_channel_setup_dt(&adc_channels[i]);
	if (err < 0) {
		printk("could not setup channel %d. error: %d\n", i, err);
		return 0;
	}
	printk("- %s, channel %d: ", adc_channels[i].dev->name, adc_channels[i].channel_id);

	(void)adc_sequence_init_dt(&adc_channels[i], &adc_ch13_seq);
	err = adc_read(adc_channels[i].dev, &adc_ch13_seq);
	printk("%d", sp_buf_ch13);

	err = adc_raw_to_millivolts_dt(&adc_channels[i], &sp_buf_ch13);
	printk(" = %d mV\n", sp_buf_ch13);
	return sp_buf_ch13;
}