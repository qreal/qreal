/**
 *******************************************************************************
 **	FILE NAME : balancer_param.c
 **
 **	ABSTRUCT  : NXTway-GS balance control parameters
 **
 ** NOTE: These parameters definitely affect the balance control of NXTway-GS.
 *******************************************************************************
 **/

#include "ecrobot_interface.h"

/*============================================================================
 * DATA DEFINITIONS
 *===========================================================================*/
F32 A_D = 0.8F;    	   /* low pass filter gain for motors average count */
F32 A_R = 0.996F;      /* low pass filter gain for motors target count */

/* 
 * NOTE: When NXT standard tires are used for NXTway-GS, a compiler macro (NXT_STD_TIRE)
 *       has to be added to USER_DEF macro definition in user Makefile
 *       E.g. USER_DEF = NXT_STD_TIRE
 */
#ifdef NXT_STD_TIRE
	/* servo control state feedback gain for NXT standard tire */
	F32 K_F[4] = {-0.834434F, -38.1749F, -1.0985F, -3.55477F};
#else
	/* servo control state feedback gain for RCX Motorcycle tire */
	F32 K_F[4] = {-0.870303F, -31.9978F, -1.1566F, -2.78873F};
#endif

F32 K_I = -0.44721F;   /* servo control integral gain */
F32 K_PHIDOT = 25.0F;  /* turn target speed gain */
F32 K_THETADOT = 7.5F; /* forward target speed gain */

const F32 BATTERY_GAIN = 0.001089F;	/* battery voltage gain for motor PWM outputs */
const F32 BATTERY_OFFSET = 0.625F;	/* battery voltage offset for motor PWM outputs */
/******************************** END OF FILE ********************************/
