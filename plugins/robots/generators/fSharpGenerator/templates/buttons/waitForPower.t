
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.Power, true ->
            exit.Set() |> ignore
)

