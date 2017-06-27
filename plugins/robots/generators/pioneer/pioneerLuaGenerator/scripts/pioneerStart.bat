@rem Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
@rem
@rem Script for executing a program on a quadcopter.
@rem
@rem Takes 2 parameters:
@rem %1 - connection type (can be "--address" or "--serial");
@rem %2 - IP address with port, if "--address" is specified as $1, or COM port name if "--serial" is specified.

%~dp0controller.exe %1 %2 --runLuaScript
