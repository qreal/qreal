#r "Trik.Observable.dll"

open Trik
open Trik.Helpers
open System.Threading

Helpers.I2C.init "/dev/i2c-2" 0x48 1
use model = new Model()
let button = new Button("/dev/input/event0")


@@VARIABLES@@

@@SUBPROGRAMS@@

[<EntryPoint>]
let main _ =
@@MAIN_CODE@@

