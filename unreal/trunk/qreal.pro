win32-msvc* {
    CONFIG(debug, debug|release) {
        QRXC = qrxc\debug\qrxc.exe
    } else:CONFIG(release, debug|release){
        QRXC = qrxc\release\qrxc.exe
    }
	!exists($$QRXC) {
		COMMAND = $$escape_expand(\"C:\Program Files\Microsoft Visual Studio 9.0\Common7\Tools\vsvars32.bat\") && cd qrxc && qmake -tp vc && vcbuild
		SYS = $$system($$COMMAND)
	}
}

TEMPLATE	= subdirs
SUBDIRS		= qrxc qrgui qrxml qrrepo

qrgui.depends = qrxc qrxml qrrepo
qrxml.depends = qrxc
