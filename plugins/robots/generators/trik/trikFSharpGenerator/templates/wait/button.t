
button.ToObservable()
    |> Observable.subscribe(function
        | ButtonEventCode.@@BUTTON@@, true ->
            exit.Set() |> ignore
)

