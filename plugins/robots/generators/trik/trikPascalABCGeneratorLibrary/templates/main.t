{$reference '../../trikSharp/Trik.Core.dll'}

const
@@CONSTANTS@@

var
    model : Trik.Model;
@@VARIABLES@@

@@SUBPROGRAMS@@

begin
    Randomize;
    model := Trik.Model.Create();
    @@MAIN_CODE@@
end.
