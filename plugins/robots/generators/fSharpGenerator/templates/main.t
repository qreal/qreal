open Trik
open Trik.Helpers
open System
open System.Threading

use randomNumber = new Random()

use model = new Model()
let button = new ButtonsPad("/dev/input/event0")
let exit = new EventWaitHandle(false, EventResetMode.AutoReset)

@@VARIABLES@@

@@SUBPROGRAMS@@

[<EntryPoint>]
let main _ =
@@MAIN_CODE@@
    0
