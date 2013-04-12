# Application condition
func.id == max_used_id and not cur_node_is_processed

# Reaction
function_code = func.Body

function_code = function_code.replace("Sensor1", port_values[0] + "1)");
function_code = function_code.replace("Sensor2", port_values[1] + "2)");
function_code = function_code.replace("Sensor3", port_values[2] + "3)");
function_code = function_code.replace("Sensor4", port_values[3] + "4)");

variables = set()

for left_part in function_code.split(';'):
  if left_part != "":
    part_code = left_part.strip() + ";\n"
    if not func.Init:
      code.append([part_code])
      if waitFor.id not in id_to_pos_in_code:
        id_to_pos_in_code[waitFor.id] = len(code) - 1
    else:
      init_code.append(part_code)
  if left_part.find('=') != -1:
    left_part = left_part[: left_part.find('=')]
    if left_part[len(left_part) - 1] in '+-=*/><':
      left_part = left_part[: len(left_part) - 1]
    left_part = left_part.strip()
    variables.add(left_part)

for variable in variables:
  variables_code.append("static int " + variable + ";\n")

cur_node_is_processed = True
