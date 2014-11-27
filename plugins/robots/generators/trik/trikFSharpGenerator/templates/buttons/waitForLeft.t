
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.Left, true ->
            exit.Set() |> ignore
)

