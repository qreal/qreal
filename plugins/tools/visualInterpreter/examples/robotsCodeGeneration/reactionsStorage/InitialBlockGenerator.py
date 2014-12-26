template = "#include \"kernel.h\"\n#include \"ecrobot_interface.h\"\n@@BALANCER@@\n@@VARIABLES@@\n\nvoid ecrobot_device_initialize(void)\n{\n@@INITHOOKS@@\n}\n\nvoid ecrobot_device_terminate(void)\n{\n@@TERMINATEHOOKS@@\n}\n\n/* nxtOSEK hook to be invoked from an ISR in category 2 */\nvoid user_1ms_isr_type2(void){ /* do nothing */ }\n\n@@CODE@@"

task_template = "TASK(OSEK_Task_Number_0)\n{\n@@CODE@@\n}"
template = template.replace("@@CODE@@", task_template)

number_of_ports = 4
port_values = [initBlock.port_1, initBlock.port_2, initBlock.port_3, initBlock.port_4]

for i in xrange(number_of_ports):
  init_ecrobot_color_sensor_port_s = "ecrobot_init_nxtcolorsensor(NXT_PORT_S"
  if port_values[i] == "Ультразвуковой сенсор":
    init_code.append("ecrobot_init_sonar_sensor(NXT_PORT_S" + str(i + 1) + ");\n")
    terminate_code.append("ecrobot_term_sonar_sensor(NXT_PORT_S" + str(i + 1) + ");\n")
    port_values[i] = "ecrobot_get_sonar_sensor(NXT_PORT_S"
  elif port_values[i] == "Сенсор цвета (все цвета)":
    init_code.append(init_ecrobot_color_sensor_port_s + str(i + 1) +", NXT_LIGHTSENSOR_WHITE);\n")
    terminate_code.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i + 1) + ");\n")
    port_values[i] = "ecrobot_get_light_sensor(NXT_PORT_S"
  elif port_values[i] == "Сенсор цвета (красный)":
    init_code.append(init_ecrobot_color_sensor_port_s + str(i + 1) + ", NXT_LIGHTSENSOR_RED);\n")
    terminate_code.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i + 1) + ");\n")
    port_values[i] = "ecrobot_get_light_sensor(NXT_PORT_S"
  elif port_values[i] == "Сенсор цвета (зеленый)":
    init_code.append(init_ecrobot_color_sensor_port_s + str(i + 1) + ", NXT_LIGHTSENSOR_GREEN);\n")
    terminate_code.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i + 1) + ");\n")
    port_values[i] = "ecrobot_get_light_sensor(NXT_PORT_S"
  elif port_values[i] == "Сенсор цвета (синий)":
    init_code.append(init_ecrobot_color_sensor_port_s + str(i + 1) + ", NXT_LIGHTSENSOR_BLUE);\n")
    terminate_code.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i + 1) + ");\n")
    port_values[i] = "ecrobot_get_light_sensor(NXT_PORT_S"
  elif port_values[i] == "Сенсор цвета (пассивный)":
    init_code.append(init_ecrobot_color_sensor_port_s + str(i + 1) + ", NXT_COLORSENSOR);\n")
    terminate_code.append("ecrobot_term_nxtcolorsensor(NXT_PORT_S" + str(i + 1) + ");\n")
    port_values[i] = "ecrobot_get_light_sensor(NXT_PORT_S"
  else:
    port_values[i] = "ecrobot_get_touch_sensor(NXT_PORT_S"

initBlock.id = max_used_id

cur_node_is_processed = True
