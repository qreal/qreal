#!/bin/sh
LD_LIBRARY_PATH=/home/dvvrd/qt-git/qtbase/lib${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}
export LD_LIBRARY_PATH
QT_PLUGIN_PATH=/home/dvvrd/qt-git/qtbase/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec /home/dvvrd/qt-git/qtbase/bin/uic "$@"
