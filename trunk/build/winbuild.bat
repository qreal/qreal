set PATH=%PATH%;C:\Qt\4.2.3\bin;c:\mingw\bin
@call qmake 
@call make 2> make.err | tee make.log