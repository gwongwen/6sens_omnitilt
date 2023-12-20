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

/* definition of constantes */
#define GEODATA_DC_OFFSET       2048    // DC value, at the analog input (2^12)/2
#define ADC_BUFFER_SIZE         2*21    // max payload size 51 bytes for SF9-DR0
                                        // 21 samples in uint16 format with ID (8 bytes) :50 bytes

/* declaration of functions using a function statement */
int8_t app_adc_init();
int8_t app_adc_handler(const struct device *dev);

#endif /* APP_SENSOR_H */
