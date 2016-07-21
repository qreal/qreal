// WARNING: Unreadable code! Close your eyes and run away!

// Don`t tell me I didn`t warn you. Now a little tips to understand this.
// This is an asm-like code to deal with native EV3 software. All your nice TRIK Studio
// mathematical one-line expressions are now big and slow pieces of ... code.
// For example "a = 100 * 200 - 50" will be translated into
//
//    MUL32(100, 100, _int_temp_result_1) /* _temp_result_* is something like asm register here */
//    SUB32(_int_temp_result_1, 50, _int_temp_result_2)
//    MOVE32_32(_temp_result_2, a)
//
// No temporary variables count optimization is performed for now, so suffer.
// Control flow is emulated with goto statements (kill me please).
// Note: 8, 16, 32-bit and float mathematical functions accept arguments only in corresponding bitness,
// so many temporary variables for type casting will be met there.

// Again, reading the code below may harm your mind. If something behaves strangely please contact developers.

@@VARIABLES@@

vmthread MAIN
{
@@CONSTANTS_INITIALIZATION@@

	DATA32 timer
	DATA8 _temp_sensor_value_8
	DATAF _temp_sensor_value_f

@@MAIN_CODE@@

__programEnd:
}

@@THREADS@@

@@SUBPROGRAMS@@
