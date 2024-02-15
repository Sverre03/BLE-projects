#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

//Bluetooth configs
#include <zephyr/bluetooth/bluetooth.h>
#include <zephyr/bluetooth/uuid.h>
#include <zephyr/bluetooth/gatt.h>
#include <zephyr/bluetooth/hci.h>

#define BT_UUID_REMOTE_SERV_VAL \
BT_UUID_128_ENCODE(0xf2d60001, 0x4c1e, 0x4433, 0xa428, 0xbc46ce930e5e)
#define BT_UUID_REMOTE_SERVICE  BT_UUID_DECLARE_128(BT_UUID_REMOTE_SERV_VAL)

/** @brief UUID of the Button Characteristic. **/
#define BT_UUID_REMOTE_BUTTON_CHRC_VAL \
	BT_UUID_128_ENCODE(0xf2d60002, 0x4c1e, 0x4433, 0xa428, 0xbc46ce930e5e)
#define BT_UUID_REMOTE_BUTTON_CHRC 	BT_UUID_DECLARE_128(BT_UUID_REMOTE_BUTTON_CHRC_VAL)

#define BT_UUID_REMOTE_MESSAGE_CHRC_VAL \
	BT_UUID_128_ENCODE(0xf2d60003, 0x4c1e, 0x4433, 0xa428, 0xbc46ce930e5e)
#define BT_UUID_REMOTE_MESSAGE_CHRC BT_UUID_DECLARE_128(BT_UUID_REMOTE_MESSAGE_CHRC_VAL)

enum bt_button_notifications_enabled {
	BT_BUTTON_NOTIFICATIONS_ENABLED,
	BT_BUTTON_NOTIFICATIONS_DISABLED,
};

/* This code snippet belongs to remote.h */
struct bt_remote_service_cb {
	void (*notif_changed)(enum bt_button_notifications_enabled status);
	void (*data_received)(struct bt_conn *conn, const uint8_t *const data, uint16_t len);
};

int bluetooth_init(struct bt_conn_cb *cb, struct bt_remote_service_cb *remote_cb);
void set_button_value(uint8_t value);
void on_sent(struct bt_conn *conn, void *user_data);
static ssize_t on_write(struct bt_conn *conn,
			  const struct bt_gatt_attr *attr,
			  const void *buf,
			  uint16_t len,
			  uint16_t offset,
			  uint8_t flags);
int send_button_notification(struct bt_conn *conn, uint8_t value);