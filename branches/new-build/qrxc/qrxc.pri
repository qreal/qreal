win32 { 
    QMAKE_QRXC = $$PWD/release/qrxc.exe
} else {
    QMAKE_QRXC = $$PWD/qrxc
}

qrxc_impl.name = QRXC Impl
qrxc_impl.depends = plugin_template.cpp $$QMAKE_QRXC
qrxc_impl.commands = $$QMAKE_QRXC -h ${QMAKE_FILE_OUT_BASE}.h -o ${QMAKE_FILE_OUT_BASE}.cpp ${QMAKE_FILE_IN}
qrxc_impl.input = QREAL_XML
qrxc_impl.output = ${QMAKE_FILE_IN_BASE}.cpp
qrxc_impl.variable_out = SOURCES

qrxc_decl.name = QRXC Decl
qrxc_decl.depends = plugin_template.h $$QMAKE_QRXC
qrxc_decl.commands = $$QMAKE_QRXC -h ${QMAKE_FILE_OUT_BASE}.h -o ${QMAKE_FILE_OUT_BASE}.cpp ${QMAKE_FILE_IN}
qrxc_decl.input = QREAL_XML
qrxc_decl.output = ${QMAKE_FILE_IN_BASE}.h
qrxc_decl.variable_out = QRXC_HEADERS

load(moc)
qrxc_moc.name = QRXC MOC
qrxc_moc.commands = $$moc_header.commands
qrxc_moc.output = $$moc_header.output
qrxc_moc.depends = $$qrxc_decl.output
qrxc_moc.input = QRXC_HEADERS
qrxc_moc.variable_out = SOURCES

QMAKE_EXTRA_COMPILERS   *=      qrxc_decl qrxc_impl qrxc_moc

