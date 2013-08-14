TEMPLATE = subdirs

SUBDIRS = \
	qrgui \
	qrrepo \
	qrkernel \
	qrutils \
	thirdparty \

qrutils.depends = qrkernel
qrrepo.depends = qrkernel qrutils

qrgui.depends = \
	qrrepo \
	qrutils \
	qrkernel \
	thirdparty \
