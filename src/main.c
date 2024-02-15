/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/logging/log.h>
#include <dk_buttons_and_leds.h>
#include <zephyr/kernel.h>
#include "custom_files/motor_control.h"
#include "custom_files/remote.h"

// #define RUN_STATUS_LED DK_LED1
// #define RUN_LED_BLINK_INTERVAL 1000

#define LOG_MODULE_NAME app
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define CONN_STATUS_LED DK_LED2

void on_connected(struct bt_conn *conn, uint8_t err);
void on_disconnected(struct bt_conn *conn, uint8_t reason);
void on_notif_changed(enum bt_button_notifications_enabled status);
void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len);

struct bt_conn_cb bluetooth_callbacks = {
	.connected 	= on_connected,
	.disconnected 	= on_disconnected,
};

static struct bt_conn *current_conn;    // Used to keep track of current connection status.

/* Callbacks */

void on_connected(struct bt_conn *conn, uint8_t err)
{
    if (err) {
        LOG_ERR("connection failed, err %d", err);
    }
    current_conn = bt_conn_ref(conn);
    dk_set_led_on(CONN_STATUS_LED);
}

void on_disconnected(struct bt_conn *conn, uint8_t reason)
{
	LOG_INF("Disconnected (reason: %d)", reason);
	dk_set_led_off(CONN_STATUS_LED);
	if(current_conn) {
		bt_conn_unref(current_conn);
		current_conn = NULL;
	}
}

struct bt_remote_service_cb remote_callbacks = {
	.notif_changed = on_notif_changed,
	.data_received = on_data_received,
};

void on_notif_changed(enum bt_button_notifications_enabled status)
{
    if (status == BT_BUTTON_NOTIFICATIONS_ENABLED) {
        LOG_INF("Notifications enabled");
    } else {
        LOG_INF("Notifications disabled");
    }
}

void on_data_received(struct bt_conn *conn, const uint8_t *const data, uint16_t len)
{
    uint8_t temp_str[len+1];
    memcpy(temp_str, data, len);
    temp_str[len] = 0x00;

    LOG_INF("Received data on conn %p. Len: %d", (void *)conn, len);
    LOG_INF("Data: %s", temp_str);
	LOG_INF("Data: %s", *data);
	motor_set_speed((*data)*100);
}

void button_handler(uint32_t button_state, uint32_t has_changed) {
	// u_int32_t button_states = dk_get_buttons();
	uint8_t button_pressed = 0;
	LOG_INF("Test");
	if(button_state && has_changed) {
		switch(has_changed) {
			case DK_BTN1_MSK:
			    button_pressed = 1;
				motor_set_speed(1000);
				break;
			case DK_BTN2_MSK:
			    button_pressed = 2;
				motor_set_speed(1300);
				break;
			case DK_BTN3_MSK:
			    button_pressed = 3;
				motor_set_speed(1600);
				break;
			case DK_BTN4_MSK:
			    button_pressed = 4;
				motor_set_speed(1900);
				break;
		}
		LOG_INF("Button %d pressed", button_pressed);
		set_button_value(button_pressed);
	}
}

static void configure_dk_buttons(void) {
	int err_buttons;
	err_buttons = dk_buttons_init(button_handler);
	if(err_buttons) {
		LOG_ERR("Couldn't init buttons (err %d)", err_buttons);
	}
}



int main(void)
{
	configure_dk_buttons();
	motor_init();	
	bluetooth_init(&bluetooth_callbacks, &remote_callbacks);
	return 0;
}
