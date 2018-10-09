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
	if (event == Ev.ALTITUDE_REACHED) then
		action[curr_state]()
	end

	if (event == Ev.POINT_REACHED) then
		action[curr_state]()
	end

	if (event == Ev.COPTER_LANDED) then
		sleep(2)
		action[curr_state]()
	end
end

-- вызов функции из таблицы состояний, соответствующей первому состоянию
action[curr_state]()
