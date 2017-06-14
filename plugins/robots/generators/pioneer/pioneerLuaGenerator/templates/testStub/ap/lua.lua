local ap = {};

function ap.push(value)
	print("ap.push(" .. value ..")");
end; 

function ap.goToPoint(p)
	print("ap.goToPoint: lat = " .. p.latitude .. ", lon = " .. p.longitude ..", alt = " .. p.altitude)
end;

function sleep(sec)
	require "socket"
	-- Oh well, scripting language without proper "sleep" function.
	socket.select(nil, nil, sec)
end

return ap;