-- Uncomment this for execution against debug stub.
-- local ap = require "ap.lua"
-- local Ev = require "Ev.lua"

@@SUBPROGRAMS_FORWARDING@@

@@THREADS_FORWARDING@@

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

math.randomseed(@@RAND_SEED@@)

ap.push(Ev.MCE_PREFLIGHT)
action[curr_state]()
