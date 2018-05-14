-- переменная текущего состояния
local curr_state = "START"
local wait_ap = false

-- таблица функций, вызываемых в зависимости от состояния
action = {
	["START"] = function(x)