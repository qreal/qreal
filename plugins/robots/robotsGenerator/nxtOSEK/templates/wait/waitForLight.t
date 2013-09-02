while (!(ecrobot_get_light_sensor(@@PORT@@) * 100 / 1023 @@CONDITION@@ @@PERCENTS@@)) {
}
