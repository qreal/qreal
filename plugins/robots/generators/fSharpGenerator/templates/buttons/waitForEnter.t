let exit = new EventWaitHandle(false, EventResetMode.AutoReset)
button.ToObservable()
    |> Observable.subscribe(function
        | Button_Event_Code.Enter, true ->
            exit.Set() |> ignore
        | _ -> () )
