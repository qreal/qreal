win32 {
        CONFIG(debug, debug|release) {
                QMAKE_QRXC = $$PWD/debug/qrxc.exe
        } else:CONFIG(release, debug|release){
                QMAKE_QRXC = $$PWD/release/qrxc.exe
        } else {
                error(Windows build definitely needs to be fixed)
        }
} else {
        QMAKE_QRXC = $$PWD/qrxc
}
