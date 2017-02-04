{$reference '../../trikSharp/Trik.Core.dll'}

const
@@CONSTANTS@@

var
    model : Trik.Model;
@@VARIABLES@@

@@SUBPROGRAMS@@

function ParseDataFromInfraredSensor(rawDistance : integer) : integer;
begin
if rawDistance < 260
    then 
        Result := round (165.59 * exp (- 0.00691486 * rawDistance))
    else
        Result := round (49.9384 * exp (- 0.00224107 * rawDistance));
end;

begin
    Randomize;
    model := Trik.Model.Create();
    // Если не используется сонар D1 или D2, удалите код, отвечающий за
    // инициализацию и остановку соответствующего сонара.
    model.Sonars[Trik.SonarPort.D1].Start();
    model.Sonars[Trik.SonarPort.D2].Start();
    @@MAIN_CODE@@
    model.Sonars[Trik.SonarPort.D1].Stop();
    model.Sonars[Trik.SonarPort.D2].Stop();
end.
