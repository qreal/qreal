import os

terminateCode = "TerminateTask();\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", terminateCode)
code = code.replace("@@BALANCER@@", "")
code = code.replace("@@VARIABLES@@", "")
code = code.replace("@@INITHOOKS@@", "")
code = code.replace("@@TERMINATEHOOKS@@", "")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
