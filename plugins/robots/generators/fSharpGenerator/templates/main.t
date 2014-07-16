open Trik
open Trik.Helpers
open System.Threading

use model = new Model()
let button = new Button("/dev/input/event0")
let exit = new EventWaitHandle(false, EventResetMode.AutoReset)

@@VARIABLES@@

@@SUBPROGRAMS@@

[<EntryPoint>]
let main _ =
@@MAIN_CODE@@
    0

