@@SUBPROGRAMS_FORWARDING@@

@@THREADS_FORWARDING@@

@@VARIABLES@@

@@INITHOOKS@@

@@SUBPROGRAMS@@

@@THREADS@@

@@MAIN_CODE@@
}

-- функция обработки событий, автоматически вызывается автопилотом
function callback(event)
	print(event)
	if (event == Ev.ALTITUDE_REACHED) then
		wait_ap = false
		action[curr_state]()
	end

	if (event == Ev.POINT_REACHED) then
		wait_ap = false
		action[curr_state]()
	end

	if (event == Ev.COPTER_LANDED) then
		wait_ap = false
		sleep(2)
		action[curr_state]()
	end

	while ((not wait_ap) and curr_state ~= "NONE") do
		curr_state = "NONE"
		action[curr_state]()
	end
end

-- бесконечный цикл, автоматически вызывается автопилотом
function loop()
end

-- вызов функции из таблицы состояний, соответствующей первому состоянию
callback()
