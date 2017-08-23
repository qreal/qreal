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
		action[curr_state]()
	end

	if (event == Ev.POINT_REACHED) then
		action[curr_state]()
	end

	if (event == Ev.COPTER_LANDED) then
		ap.push(Ev.ENGINES_DISARM)
		sleep(2)
		action[curr_state]()
	end

end

-- бесконечный цикл, автоматически вызывается автопилотом
function loop()
end

-- вызов функции из таблицы состояний, соответствующей первому состоянию
action[curr_state]()
