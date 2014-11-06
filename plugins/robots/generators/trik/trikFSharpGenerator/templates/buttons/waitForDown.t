
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.Down, true ->
            exit.Set() |> ignore
)

