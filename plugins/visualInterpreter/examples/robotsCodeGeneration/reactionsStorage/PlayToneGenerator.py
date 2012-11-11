import os

playToneCode = "ecrobot_sound_tone(" + playTone.Frequency + ", " + playTone.Duration + ", " + playTone.Volume + ");\n"

scriptDir = os.path.dirname(__file__)
relPath = "nxt-tools/program0.c"
absPath = os.path.join(scriptDir, relPath)

codeFile = open(absPath, 'r')
code = codeFile.read()

code = code.replace("@@CODE@@", playToneCode + "@@CODE@@")

codeFile.close()
codeFile = open(absPath, 'w')
codeFile.write(code)
codeFile.close()
