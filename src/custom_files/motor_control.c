#include "motor_control.h"

#define LOG_MODULE_NAME motor_control
LOG_MODULE_REGISTER(LOG_MODULE_NAME);

#define PWM_PERIOD_NS 2E7
#define DUTY_CYCLE 15E5

#define SERVO_MOTOR DT_NODELABEL(servo) 

static const struct pwm_dt_spec pwm_servo = PWM_DT_SPEC_GET(SERVO_MOTOR);


void motor_init() {
    int err = 0;
    LOG_INF("Initialising motor control.");
    if (!device_is_ready(pwm_servo.dev)) {
        LOG_ERR("Error: PWM device %s is not ready",
                pwm_servo.dev->name);
        return -EBUSY;
	}

    err = pwm_set_dt(&pwm_servo, PWM_PERIOD_NS, DUTY_CYCLE);
    if(err) {
        LOG_ERR("pwm_set_dt returned %d", err);
    }

    return err;
}

void motor_set_speed(int speed) {
    int speed_to_pwm = PWM_USEC(speed);
    if ((speed_to_pwm >= DT_PROP(SERVO_MOTOR, min_pulse)) && (speed_to_pwm <= DT_PROP(SERVO_MOTOR, max_pulse))) {
        pwm_set_dt(&pwm_servo, PWM_PERIOD_NS, speed_to_pwm);
    }
    else {
        LOG_ERR("Speed %d is out of range", speed);
    }
}