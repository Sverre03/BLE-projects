#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/pwm.h>
#define PWM_LED0 DT_ALIAS(pwm_led0)

void motor_init();
void motor_set_speed();