let exit = new EventWaitHandle(false, EventResetMode.AutoReset)
button.ToObservable()
    |> Observable.subscribe(function
        | Button_Event_Code.Up, true ->
            exit.Set() |> ignore
        | _ -> () )
