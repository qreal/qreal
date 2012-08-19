#ifndef RTW_HEADER_balancer_h_
#define RTW_HEADER_balancer_h_
#ifndef balancer_COMMON_INCLUDES_
# define balancer_COMMON_INCLUDES_
#include "rtwtypes.h"
#include "rt_SATURATE.h"
#endif                                 /* balancer_COMMON_INCLUDES_ */

#include "balancer_types.h"

/* Macros for accessing real-time model data structure */
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((void*) 0)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((void) 0)
#endif

#ifndef rtmGetStopRequested
# define rtmGetStopRequested(rtm)      ((void*) 0)
#endif

#define CMD_MAX                        100.0F
#define DEG2RAD                        0.01745329238F
#define EXEC_PERIOD                    0.00400000019F

/* Model entry point functions */
extern void balance_init(void);

/* Customized model step function */
extern void balance_control(F32 args_cmd_forward, F32 args_cmd_turn,
  F32 args_gyro, F32 args_gyro_offset, F32 args_theta_m_l,
  F32 args_theta_m_r, F32 args_battery, S8 *ret_pwm_l, S8
  *ret_pwm_r);

/*-
 * The generated code includes comments that allow you to trace directly
 * back to the appropriate location in the model.  The basic format
 * is <system>/block_name, where system is the system number (uniquely
 * assigned by Simulink) and block_name is the name of the block.
 *
 * Use the MATLAB hilite_system command to trace the generated code back
 * to the model.  For example,
 *
 * hilite_system('<S3>')    - opens system 3
 * hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
 *
 * Here is the system hierarchy for this model
 *
 * '<Root>' : balancer
 * '<S1>'   : balancer/Control
 * '<S2>'   : balancer/Control/Cal_PWM
 * '<S3>'   : balancer/Control/Cal_Reference
 * '<S4>'   : balancer/Control/Cal_x1
 * '<S5>'   : balancer/Control/DiscreteIntegrator
 * '<S6>'   : balancer/Control/Cal_PWM/Cal_vol_max
 * '<S7>'   : balancer/Control/Cal_Reference/DiscreteIntegrator
 * '<S8>'   : balancer/Control/Cal_Reference/LowPathFilter
 * '<S9>'   : balancer/Control/Cal_x1/DiscreteDerivative
 * '<S10>'  : balancer/Control/Cal_x1/DiscreteIntegrator
 * '<S11>'  : balancer/Control/Cal_x1/LowPathFilter
 */
#endif                                 /* RTW_HEADER_balancer_h_ */
/******************************** END OF FILE ********************************/
