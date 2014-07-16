
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.Right, true ->
            exit.Set() |> ignore
)

