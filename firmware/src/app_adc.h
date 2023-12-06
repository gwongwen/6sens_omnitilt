/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef APP_SENSOR_H
#define APP_SENSOR_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/adc.h>

#define DT_SPEC_AND_COMMA(node_id, prop, idx)   ADC_DT_SPEC_GET_BY_IDX(node_id, idx)
#define GEODATA_DC_OFFSET                       159     // DC value, at the analog input
#define LOWEST_FREQUENCY_REPORTED               10
#define HIGHEST_FREQUENCY_REPORTED              150
#define DEFAULT_AMPLITUDE_THRESHOLD             0.1     // default threshold for reporting amplitudes
#define BUFFER_SIZE                             40      //max payload size 51 with 8 ID bytes
 
int8_t app_adc_init();
uint16_t app_adc_handler();
void adc_work_handler(struct k_work *work_adc);

#endif /* APP_SENSOR_H */
