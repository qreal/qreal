
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.Up, true ->
            exit.Set() |> ignore
)

