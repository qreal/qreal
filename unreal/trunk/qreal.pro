TEMPLATE	= subdirs
SUBDIRS		= qrxc qrgui qrxml qrrepo

qrgui.depends = qrxc qrxml qrrepo
qrxml.depends = qrxc
