#include <types.h>
#include <embox/test.h>
#include <unistd.h>
#include <drivers/nxt_buttons.h>
#include <drivers/nxt_sonar.h>
#include <util/math.h>

#include <drivers/nxt_motor.h>

EMBOX_TEST(box_around_test);

#define SONAR_PORT (&sensors[1])
#define MOTOR0 (&motors[0])
#define MOTOR1 (&motors[1])

#define ROBOT_WIDTH 15

#define MOTOR_POWER -100

static int sonar_treshold = 50;

/*
static void rotate_start(int motor_power0, int motor_power1) {
	motor_set_power(MOTOR0, motor_power0);
	motor_set_power(MOTOR1, motor_power1);
}

static void move_start(void) {
	motor_set_power(MOTOR0, MOTOR_POWER);
	motor_set_power(MOTOR1, MOTOR_POWER);
}

static void move_stop(void) {
	motor_set_power(MOTOR0, 0);
	motor_set_power(MOTOR1, 0);
}
*/

static int @@TEST_NAME@@(void) {
	@@CODE@@
}
