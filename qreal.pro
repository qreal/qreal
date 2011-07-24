contains(QT_VERSION, ^4\\.[0-5]\\..*){
	message("Cannot build qreal using Qt version $${QT_VERSION}")
	error("Use at least Qt 4.6")
}

TEMPLATE = subdirs
SUBDIRS = qrmc qrxc qrgui qrrepo plugins qrkernel qrutils qtpropertybrowser

qrutils.depends = qrkernel
qrrepo.depends = qrkernel qrutils
qrmc.depends = qrrepo
qrmc.file = qrmc\\qrmc-lib.pro
qrgui.depends = qrxc qrrepo qrkernel qtpropertybrowser qrutils plugins
plugins.depends = qrxc qrrepo qrkernel qrutils

qtpropertybrowser.file = qrgui\\thirdparty\\qtpropertybrowser\\qtpropertybrowser.pro
