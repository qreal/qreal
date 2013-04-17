if len(if_nodes) > 0:
  cur_condition_id = if_nodes[0]
  cond = conditions[cur_condition_id]
  
  if element2.id > cur_condition_id:
    branch_end[cur_condition_id] = len(code) - 1
    ifGeneration()
  elif element2.id <= cur_condition_id and (len(if_nodes) == 1 or element2.id >= if_nodes[1]):
    if branch_type[cur_condition_id] == 2:
      code[branch_end[cur_condition_id]].insert(0, "break;\n")
      ifGeneration()
    else:
      branch_end_type[cur_condition_id] = 2
      cond = "!(" + cond+ ")"
      code[id_to_pos_in_code[cur_condition_id]].insert(0,"if (" + cond + ") {\n")
      code[id_to_pos_in_code[cur_condition_id]].insert(1,"break;\n")
      code[id_to_pos_in_code[cur_condition_id]].insert(2,"}\n")

code[id_to_pos_in_code[element2.id]].insert(0, "while (true) {\n")
code[id_to_pos_in_code[element1.id]].append("}\n")

processed_loops[element1.id] = element2.id

save()
