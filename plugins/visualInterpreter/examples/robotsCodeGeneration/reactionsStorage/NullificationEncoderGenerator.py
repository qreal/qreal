import os

nullificationCode = "nxt_motor_set_count(NXT_PORT_" + encoder.Port + ", 0);\n"

relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", nullificationCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
