import os

max_used_id = -1
cur_node_is_processed = False

conditions = {}
if_nodes = []
if_nodes_with_2_branches = []
branch_end = {}
branch_end_type = {}
branch_type = {}

code = []
init_code = []
terminate_code = []
variables_code = []
balancer_code = ''
id_to_pos_in_code = {}

processed_loops = {}
processed_ends = []

def ifGeneration():
  global if_nodes
  global code
  global id_to_pos_in_code
  global branch_type
  global branch_end
  global branch_end_type
  global if_nodes_with_2_branches
  
  if len(if_nodes) > 0:
    while len(if_nodes) > 0 and if_nodes[0] not in if_nodes_with_2_branches:
      cond = conditions[if_nodes[0]]
      code[id_to_pos_in_code[if_nodes[0]]].insert(0, "if (" + cond + ") {\n")
      code.append(["}\n"])
      del if_nodes[0]
    
    if len(if_nodes) > 0:
      if branch_type[if_nodes[0]] == 1:
        cond = conditions[if_nodes[0]]
        code[id_to_pos_in_code[if_nodes[0]]].insert(0, "if (" + cond + ") {\n")
        code.append(["}\n"])
        
        branch_end[if_nodes[0]] = len(code) - 1
        branch_end_type[if_nodes[0]] = 1
      elif branch_type[if_nodes[0]] == 2:
        while len(if_nodes) > 0 and (branch_type[if_nodes[0]] == 2 or if_nodes[0] not in if_nodes_with_2_branches):
          if if_nodes[0] not in if_nodes_with_2_branches:
            cond = conditions[if_nodes[0]]
            code[id_to_pos_in_code[if_nodes[0]]].insert(0, "if (" + cond + ") {\n")
            code.append(["}\n"])
          elif branch_end_type[if_nodes[0]] == 1:
            code[branch_end[if_nodes[0]]].append("else {\n")
            code.append(["}\n"])
          del if_nodes[0]
        
        if len(if_nodes) > 0:
          cond = conditions[if_nodes[0]]
          code[id_to_pos_in_code[if_nodes[0]]].insert(0, "if (" + cond + ") {\n")
          code.append(["}\n"])
          branch_end[if_nodes[0]] = len(code) - 1
          branch_end_type[if_nodes[0]] = 1

def indentString(i):
  res = ''
  while i > 0:
    res = res + '\t'
    i -= 1
  return res

def collectPortNames(ports, port_names):
  if ports.find("A") != -1:
    port_names.append("NXT_PORT_A")
  if ports.find("B") != -1:
    port_names.append("NXT_PORT_B")
  if ports.find("C") != -1:
    port_names.append("NXT_PORT_C")

def convertCondition(sign):
  if sign == "меньше":
    return "<"
  elif sign == "больше":
    return ">"
  elif sign == "не меньше":
    return ">="
  elif sign == "не больше":
    return "<="
  else:
    return "=="

def containsInitCode(code):
  global init_code
  
  for c in init_code:
    if c <= code:
      return True
  return False

def save():
  global template
  global code
  global script_dir
  
  final_code = ''
  indent = 1
  for strings in code:
    for s in strings:
      if s:
        if (s.find('}') > -1 and s.find('{}') == -1):
          indent -= 1
        final_code = final_code + indentString(indent) + s
        if (s.find('{') > -1 and s.find('{}') == -1):
          indent += 1
  
  final_init_code = ''
  for s1 in init_code:
    final_init_code = final_init_code + "\t" + s1
  
  final_terminate_code = ''
  for s2 in terminate_code:
    final_terminate_code = final_terminate_code + "\t" + s2
  
  final_variables_code = ''
  for s3 in variables_code:
    final_variables_code = final_variables_code + s3
  
  s = template.replace("@@CODE@@", final_code)
  s = s.replace("@@BALANCER@@", balancer_code)
  s = s.replace("@@VARIABLES@@", final_variables_code)
  s = s.replace("@@INITHOOKS@@", final_init_code)
  s = s.replace("@@TERMINATEHOOKS@@", final_terminate_code)
  
  dir_path = os.path.join(__script_dir__, "nxt-tools")
  if not os.path.exists(dir_path):
    os.mkdir(dir_path)
  file_path = os.path.join(dir_path, "program0.c")
  
  code_file = open(file_path, 'w')
  code_file.write(s)
  code_file.close()
