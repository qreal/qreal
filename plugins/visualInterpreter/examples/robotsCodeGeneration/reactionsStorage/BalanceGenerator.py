import os

balanceCode = "balance_control((F32)" + balancer.forward + ", (F32)" + balancer.turn + ", (F32)ecrobot_get_gyro_sensor(NXT_PORT_S" + balancer.port + "), (F32)" + balancer.gyroOffset + ", (F32)nxt_motor_get_count(NXT_PORT_" + balancer.port1 + "), (F32)nxt_motor_get_count(NXT_PORT_" + balancer.port2 + "), (F32)ecrobot_get_battery_voltage(), &" + balancer.pwm1 + ", &" + balancer.pwm2 + ");\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", balanceCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
