let exit = new EventWaitHandle(false, EventResetMode.AutoReset)
button.ToObservable()
    |> Observable.subscribe(function
        | Button_Event_Code.Right, true ->
            exit.Set() |> ignore
        | _ -> () )
