-- создание порта управления магнитом
local magnet = Gpio.new(Gpio.C, 3, Gpio.OUTPUT)
-- local magnet = Gpio.new(Gpio.A, 1, Gpio.OUTPUT) -- использовать для платы версии 1.1
