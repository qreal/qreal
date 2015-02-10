TEMPLATE = subdirs

SUBDIRS = \
	qrgui \
	qrrepo \
	qrkernel \
	qrutils \
	qrtext \
	thirdparty \

qrutils.depends = qrkernel qrtext
qrrepo.depends = qrkernel qrutils
qrtext.depends = qrkernel

qrgui.depends = \
	qrrepo \
	qrutils \
	qrkernel \
	thirdparty \
