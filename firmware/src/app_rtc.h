/*
 * Copyright (c) 2023
 * Regis Rousseau
 * Univ Lyon, INSA Lyon, Inria, CITI, EA3720
 * SPDX-License-Identifier: Apache-2.0
 */
#ifndef APP_RTC_STM32_H_
#define APP_RTC_STM32_H_

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/types.h>
#include <zephyr/posix/time.h>
#include <zephyr/drivers/counter.h>

/* definition of constantes */
#define TIMER DT_INST(0, st_stm32_rtc)

/* declaration of functions using a function statement */
int8_t app_rtc_init(const struct device *dev);
int8_t app_rtc_handler(const struct device *dev);

#endif /* APP_RTC_STM32_H_ */
