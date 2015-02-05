contains(QT_VERSION, ^4\\.[0-5]\\..*){
	message("Cannot build qreal using Qt version $${QT_VERSION}")
	error("Use at least Qt 4.6")
}

TEMPLATE = subdirs
SUBDIRS = \
	qrmc \
	qrxc \
	qrgui \
	qrrepo \
	plugins \
	qrkernel \
	qrutils \
	qrtext \
	thirdparty \
	qrtranslations \

qrkernel.depends = thirdparty
qrutils.depends = qrkernel qrtext
qrrepo.depends = qrkernel qrutils
qrxc.depends = qrutils
qrmc.depends = qrrepo
qrmc.file = $$PWD/qrmc/qrmc-lib.pro

qrgui.depends = \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrmc \
	thirdparty \

plugins.depends = \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrmc \
	qrgui \
	qrtext \

qrtext.depends = qrkernel
