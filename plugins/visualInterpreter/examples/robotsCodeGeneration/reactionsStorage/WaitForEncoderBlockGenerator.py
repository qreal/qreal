import os

waitForEncoderCode = "while (nxt_motor_get_count(NXT_PORT_" + waitFor.Port + ") < " + waitFor.TachoLimit + ")\n{}\n"

relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", waitForEncoderCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
