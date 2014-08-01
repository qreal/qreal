
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.Enter, true ->
            exit.Set() |> ignore
)

