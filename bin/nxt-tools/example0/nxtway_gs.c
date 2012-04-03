#include "kernel.h"
#include "kernel_id.h"
#include "ecrobot_interface.h"

#include "balancer.h" 

void ecrobot_device_initialize(void)
{
}

void ecrobot_device_terminate(void)
{
	nxt_motor_set_speed(NXT_PORT_B, 0, 1);
	nxt_motor_set_speed(NXT_PORT_C, 0, 1);
}

void user_1ms_isr_type2(void)
{
}

TASK(OSEK_Task_ts1)
{
	static U32 gyro_offset;
	static U32 avg_cnt;
	static U32 cal_start_time;
	S8 cmd_forward, cmd_turn;
	S8 pwm_l, pwm_r;

	gyro_offset = 0;
	avg_cnt = 0;
	balance_init();                       
	cal_start_time = ecrobot_get_systick_ms();

	while (true) {
		gyro_offset += (U32)ecrobot_get_gyro_sensor(NXT_PORT_S4);
		avg_cnt++;
		if ((ecrobot_get_systick_ms() - cal_start_time) >= 1000U) {
			gyro_offset /= avg_cnt;
			ecrobot_sound_tone(440U, 500U, 30U); /* beep a tone */
			break;
		}
		systick_wait_ms(4);
	}

	while (true) {
		cmd_forward = 0;
		cmd_turn = 0;

		balance_control(
			(F32)cmd_forward,
			(F32)cmd_turn,
			(F32)ecrobot_get_gyro_sensor(NXT_PORT_S4),
			(F32)gyro_offset,
			(F32)nxt_motor_get_count(NXT_PORT_B),
			(F32)nxt_motor_get_count(NXT_PORT_C),
			(F32)ecrobot_get_battery_voltage(),
			&pwm_l,
			&pwm_r);
		nxt_motor_set_speed(NXT_PORT_B, pwm_l, 1);
		nxt_motor_set_speed(NXT_PORT_C, pwm_r, 1);
		systick_wait_ms(4);
	}
}