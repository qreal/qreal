while (ecrobot_get_nxtcolorsensor_id(@@PORT@@) != @@COLOR@@) {
	wait(4);
}