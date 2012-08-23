contains(QT_VERSION, ^4\\.[0-5]\\..*){
	message("Cannot build qreal using Qt version $${QT_VERSION}")
	error("Use at least Qt 4.6")
}

TEMPLATE = subdirs
SUBDIRS = \
#	doc \
	qrmc \
	qrxc \
	qrgui \
	qrrepo \
	plugins \
	qrkernel \
	qrutils \
	thirdparty \

qrutils.depends = qrkernel
qrrepo.depends = qrkernel qrutils
qrxc.depends = qrutils
qrmc.depends = qrrepo
qrmc.file = qrmc/qrmc-lib.pro

qrgui.depends = \
#	doc \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrmc \

plugins.depends = \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrmc \

infile(qrtest/unitTests/unittestDeclaration.cfg, UNIT_TEST, TRUE) {
	SUBDIRS += \
		qrtest \
		qrxcTest \
		qrguiTest \

	qrtest.depends = \
		thirdparty \

	qrxcTest.file = qrxc/qrxcTest.pro
	qrguiTest.file = qrgui/qrguiTest.pro

	qrxcTest.depends = qrxc
	qrguiTest.depends = qrgui
}
