@rem Copyright 2017 QReal Research Group. Licensed under the Apache License, Version 2.0
@rem
@rem Script that is used by Pioneer HTTP communicator to compile a program before uploading it to a robot.
@rem Takes 2 parameters:
@rem %1 - file name (with path) of a .lua program to compile;
@rem %2 - Absolute path to Lua compiler that shall be used (leave it blank to use compiler from PATH).

%2luac -o "%1c" "%1"
