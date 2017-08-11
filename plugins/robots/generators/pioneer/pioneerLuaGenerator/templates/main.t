@@SUBPROGRAMS_FORWARDING@@

@@THREADS_FORWARDING@@

@@VARIABLES@@

@@INITHOOKS@@

@@SUBPROGRAMS@@

@@THREADS@@

@@MAIN_CODE@@
}

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

function loop()
end

action[curr_state]()
