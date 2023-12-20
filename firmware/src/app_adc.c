/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#include "app_adc.h"
#include "app_lorawan.h"

#define DT_SPEC_AND_COMMA(node_id, prop, idx)   ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

uint16_t sp_chan0;			// geophone connected to ADC pin 13 - see device tree file
int8_t adc_isr_cur_ind;		// index used by Interrupt Service Routine

/* data of ADC io-channels specified in devicetree */
static const struct adc_dt_spec adc_channels[] = {
	DT_FOREACH_PROP_ELEM(DT_PATH(zephyr_user), io_channels,
			     DT_SPEC_AND_COMMA)
};

//static const struct adc_dt_spec adc_chan0 = ADC_DT_SPEC_GET_BY_IDX(DT_PATH(zephyr_user), 1);

struct adc_sequence adc_chan0_seq = {
	.buffer 		= &sp_chan0,
	.buffer_size	= sizeof(sp_chan0),
};

/* prepare the geodata sample buffers for periodic sampling */
int8_t app_adc_init(void)
{
	int8_t i = 0;

	if (!device_is_ready(adc_channels[i].dev)) {
		printk("ADC controller device %s not ready\n", adc_channels[i].dev->name);
		return 0;
	} else {
		printk("- found device: \"%s\", getting analog data\n", adc_channels[i].dev->name);
	}
	adc_isr_cur_ind = 0;
	return 0;
}

/* Interrupt Service Routine for sampling the geophone data */
int8_t app_adc_handler(const struct device *dev)
{
	int8_t ret;
	int8_t i = 0;
	uint8_t adc_buffer[ADC_BUFFER_SIZE];
	uint8_t dev_eui[] = LORAWAN_DEV_EUI;

	struct payload_config {
		uint8_t id;
		uint8_t *adc_val;
	};
	struct payload_config payload;
	payload.id = dev_eui;
	payload.adc_val = &adc_buffer;

	ret = adc_channel_setup_dt(&adc_channels[i]);
	if (ret < 0) {
		printk("could not setup channel %d. error: %d\n", i, ret);
		return 0;
	} else {
		printk("- %s, channel %d: ", adc_channels[i].dev->name, adc_channels[i].channel_id);
	}
	
	(void)adc_sequence_init_dt(&adc_channels[i], &adc_chan0_seq);
	ret = adc_read(adc_channels[i].dev, &adc_chan0_seq);
	ret = adc_raw_to_millivolts_dt(&adc_channels[i], &sp_chan0);
	printk(" = %d mV\n", sp_chan0);

	if (sp_chan0 > GEODATA_DC_OFFSET ) { 
		adc_buffer[adc_isr_cur_ind] = sp_chan0 >> 8;
		adc_buffer[adc_isr_cur_ind+1] = sp_chan0;
		adc_isr_cur_ind++;
		if (adc_isr_cur_ind == ADC_BUFFER_SIZE) {
			ret = lorawan_send(dev, &payload, sizeof(payload), LORAWAN_MSG_UNCONFIRMED);
				if (ret < 0) {
					printk("LoRa send failed\n");;
				}
			adc_isr_cur_ind = 0;
		}
	}
	return 0;
}