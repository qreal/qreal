TEMPLATE = subdirs

CONFIG += c++11

SUBDIRS += \
	qrmc \
	qrxc \
	qrrepo \
	qrkernel \
	qrutils \
	qrtest/editorPluginTestingFramework \

qrutils.depends = qrkernel
qrrepo.depends = qrkernel qrutils
qrxc.depends = qrutils
qrmc.depends = qrrepo
qrmc.file = qrmc/qrmc-lib.pro

editorPluginTestingFramework.depends = \
	qrmc \
	qrxc \
	qrrepo \
	qrkernel \
	qrutils \
