TEMPLATE = subdirs

SUBDIRS = \
	editor \
	robotsInterpreter \
	qextSerialPort \

qextSerialPort.file = thirdparty/qextserialport/qextserialport.pro
robotsInterpreter.depends = qextSerialPort
