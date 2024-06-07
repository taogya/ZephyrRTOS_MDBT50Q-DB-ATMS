/*
 * Copyright (c) 2017 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/adc.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/__assert.h>
#include <string.h>

/* size of stack area used by each thread */
#define STACKSIZE 1024

/* scheduling priority used by each thread */
#define PRIORITY 7

#define LED0_NODE DT_ALIAS(led0)
#define LED1_NODE DT_ALIAS(led1)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

#if !DT_NODE_HAS_STATUS(LED1_NODE, okay)
#error "Unsupported board: led1 devicetree alias is not defined"
#endif

struct printk_data_led_t {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	uint32_t led;
	uint32_t cnt;
};

struct printk_data_adc_t {
	void *fifo_reserved; /* 1st word reserved for use by fifo */
	int16_t buf;
	int32_t val_mv;
};

K_FIFO_DEFINE(printk_fifo_led);
K_FIFO_DEFINE(printk_fifo_adc);

struct led {
	struct gpio_dt_spec spec;
	uint8_t num;
};

static const struct led led0 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED0_NODE, gpios, {0}),
	.num = 0,
};

static const struct led led1 = {
	.spec = GPIO_DT_SPEC_GET_OR(LED1_NODE, gpios, {0}),
	.num = 1,
};

static const struct adc_dt_spec adc_channel = ADC_DT_SPEC_GET(DT_PATH(zephyr_user));

void blink(const struct led *led, uint32_t sleep_ms, uint32_t id)
{
	const struct gpio_dt_spec *spec = &led->spec;
	int cnt = 0;
	int ret;

	if (!device_is_ready(spec->port)) {
		printk("Error: %s device is not ready\n", spec->port->name);
		return;
	}

	ret = gpio_pin_configure_dt(spec, GPIO_OUTPUT);
	if (ret != 0) {
		printk("Error %d: failed to configure pin %d (LED '%d')\n",
			ret, spec->pin, led->num);
		return;
	}

	while (1) {
		k_msleep(sleep_ms);

		gpio_pin_set(spec->port, spec->pin, cnt % 2);

		struct printk_data_led_t tx_data = { .led = id, .cnt = cnt };

		size_t size = sizeof(struct printk_data_led_t);
		char *mem_ptr = k_malloc(size);
		__ASSERT_NO_MSG(mem_ptr != 0);

		memcpy(mem_ptr, &tx_data, size);

		k_fifo_put(&printk_fifo_led, mem_ptr);

		cnt++;
	}
}

void blink0(void)
{
	blink(&led0, 100, 0);
}

void blink1(void)
{
	blink(&led1, 1000, 1);
}

void adc_in(void)
{
	int err = 0;
	uint32_t sleep_ms = 500;

	err = adc_channel_setup_dt(&adc_channel);
	if(err < 0) {
		printk("ADC channel setup failed (%d)\n", err);
		return;
	}

	if (!adc_is_ready_dt(&adc_channel)) {
		printk("ADC controller devivce %s not ready\n", adc_channel.dev->name);
		return;
	}

	int16_t buf = 0;
	struct adc_sequence sequence = {
		.buffer = &buf,
		/* buffer size in bytes, not number of samples */
		.buffer_size = sizeof(buf),
		//Optional
		//.calibrate = true,
	};
	err = adc_sequence_init_dt(&adc_channel, &sequence);
	if (err < 0) {
		printk("Could not initalize sequnce (%d)\n", err);
		return;
	}
	
	while (1) {
		k_msleep(sleep_ms);

		err = adc_read(adc_channel.dev, &sequence);
		if (err < 0) {
			printk("Could not read (%d)\n", err);
			continue;
		}

		int32_t val_mv = 0;
		err = adc_raw_to_millivolts_dt(&adc_channel, &val_mv);
		/* conversion to mV may not be supported, skip if not */
		if (err < 0) {
			printk("Value in mV not available (%d)\n", err);
			continue;
		} else {
			struct printk_data_adc_t tx_data = { .buf = buf, .val_mv = val_mv };

			size_t size = sizeof(struct printk_data_adc_t);
			char *mem_ptr = k_malloc(size);
			__ASSERT_NO_MSG(mem_ptr != 0);

			memcpy(mem_ptr, &tx_data, size);

			k_fifo_put(&printk_fifo_adc, mem_ptr);
		}
	}
}

void uart_out(void)
{
	while (1) {
		struct printk_data_led_t *rx_data_led = k_fifo_get(&printk_fifo_led, K_FOREVER);
		struct printk_data_adc_t *rx_data_adc = k_fifo_get(&printk_fifo_adc, K_NO_WAIT);
		if(rx_data_adc == NULL) {
			printk("Toggled led%d; counter=%d\n",
			       rx_data_led->led, rx_data_led->cnt);
		} else {
			printk("Toggled led%d; counter=%d, adc_raw=%d, adc_mv=%d\n",
			       rx_data_led->led, rx_data_led->cnt, rx_data_adc->buf, rx_data_adc->val_mv);
		}
		k_free(rx_data_led);
		k_free(rx_data_adc);
	}
}

K_THREAD_DEFINE(blink0_id, STACKSIZE, blink0, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(blink1_id, STACKSIZE, blink1, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(adc_in_id, STACKSIZE, adc_in, NULL, NULL, NULL,
		PRIORITY, 0, 0);
K_THREAD_DEFINE(uart_out_id, STACKSIZE, uart_out, NULL, NULL, NULL,
		PRIORITY, 0, 0);
