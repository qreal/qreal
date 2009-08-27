TEMPLATE	= subdirs
SUBDIRS		= qrxc qrgui qrxml

qrgui.depends = qrxc qrxml
qrxml.depends = qrxc
