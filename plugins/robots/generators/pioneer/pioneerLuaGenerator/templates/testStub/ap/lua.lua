local ap = {};

function ap.push(value)
	print("ap.push(" .. value ..")");
end; 

function ap.goToPoint(p)
	print("ap.goToPoint: lat = " .. p.latitude .. ", lon = " .. p.longitude ..", alt = " .. p.altitude)
end;

return ap;