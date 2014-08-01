open Trik
open Trik.Junior
open System
open System.Threading

use randomNumber = new Random()


let model = new Model()
let button = new ButtonsPad("/dev/input/event0")
let exit = new EventWaitHandle(false, EventResetMode.AutoReset)
let robot = new Robot()


@@VARIABLES@@

@@SUBPROGRAMS@@

[<EntryPoint>]
let main _ =
@@MAIN_CODE@@
    0
