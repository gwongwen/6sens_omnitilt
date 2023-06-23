/*
 * Copyright (c) 2023
 * Regis Rousseau, INSA Lyon, CITI Lab.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef __GEOPHONE__H
#define __GEOPHONE__H

#define DT_SPEC_AND_COMMA(node_id, prop, idx) \
	ADC_DT_SPEC_GET_BY_IDX(node_id, idx),

int app_sensor_init(void);
uint16_t app_sensor_read(void);

#endif

