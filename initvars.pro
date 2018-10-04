include($$PWD/global.pri)
TEMPLATE=subdirs
SUBDIRS=
#DESTDIR = $$absolute_path($$DESTDIR)

#message($$DESTDIR)
QMAKE_SUBSTITUTES += .qmake.cache.in
