#!/bin/bash
# Copyright 2015 CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -o nounset
set -o errexit

function show_help {
	echo "Usage: build-checker-installer.sh <path to Qt> [<path to QReal>] [<path to directory with fields>] [<path to directory with examples>] [<path to directory with tasks>]"
	echo "<path to Qt> --- root of your Qt 5.5 installation"
	echo "<path to QReal> --- path to QReal sources root (with completed release build of qrealRobots.pro)."
	echo "                    Defaults to \"../..\""
	echo "<path to \"fields\" directory> --- path to directory with prepared fields."
	echo "                                   Defaults to \"<path to QReal/qrtest/trikStudioSimulatorTests/fields/fields>.\""
	echo "<path to \"examples\" directory> --- path to directory with example saves."
	echo "                                     Defaults to \"<path to QReal/qrtest/trikStudioSimulatorTests/solutions>.\""
	echo "<path to \"tasks\" directory> --- path to directory with saves with tasks."
	echo "                                  Defaults to \"<path to QReal/qrtest/trikStudioSimulatorTests/tasks>.\""
	echo "Example: ./build-checker-installer.sh ~/Qt ~/stepic-examples"
	exit 0
}

[ "$#" -lt 1 ] && show_help || :

cd "$(dirname "$0")"

qtDir=$(readlink -f $1)

if [ "$#" -gt 1 ]; then
	qRealDir=$(readlink -f $2)
else
	qRealDir=$(readlink -f ../..)
fi

if [ "$#" -gt 2 ]; then
	fieldsDir=$(readlink -f $3)
else
	fieldsDir=$(readlink -f $qRealDir/qrtest/trikStudioSimulatorTests/fields/fields)
fi

if [ "$#" -gt 3 ]; then
	examplesDir=$(readlink -f $4)
else
	examplesDir=$(readlink -f $qRealDir/qrtest/trikStudioSimulatorTests/solutions)
fi

if [ "$#" -gt 4 ]; then
	tasksDir=$(readlink -f $5)
else
	tasksDir=$(readlink -f $qRealDir/qrtest/trikStudioSimulatorTests/tasks)
fi

mkdir -p trikStudio-checker/bin
cd trikStudio-checker/bin

# Copying required Qt libraries
cp -rf $qtDir/5.5/gcc_64/plugins/iconengines .

mkdir imageformats
cp -fP  $qtDir/5.5/gcc_64/plugins/imageformats/libqsvg.so imageformats/

mkdir platforms
cp -fP $qtDir/5.5/gcc_64/plugins/platforms/libqminimal.so platforms/

cp -fP $qtDir/5.5/gcc_64/lib/libicudata.so.54* .
cp -fP $qtDir/5.5/gcc_64/lib/libicui18n.so.54* .
cp -fP $qtDir/5.5/gcc_64/lib/libicuuc.so.54* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Core.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Gui.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Network.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5PrintSupport.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Script.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Svg.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Widgets.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5Xml.so* .
cp -fP $qtDir/5.5/gcc_64/lib/libQt5DBus.so* .

# Copying QReal libraries
cp -fP $qRealDir/bin/release/changelog.txt .
cp -fP $qRealDir/bin/release/libqrgui-brand-manager.so* .
cp -fP $qRealDir/bin/release/libqrgui-controller.so* .
cp -fP $qRealDir/bin/release/libqrgui-dialogs.so* .
cp -fP $qRealDir/bin/release/libqrgui-editor.so* .
cp -fP $qRealDir/bin/release/libqrgui-facade.so* .
cp -fP $qRealDir/bin/release/libqrgui-models.so* .
cp -fP $qRealDir/bin/release/libqrgui-mouse-gestures.so* .
cp -fP $qRealDir/bin/release/libqrgui-plugin-manager.so* .
cp -fP $qRealDir/bin/release/libqrgui-preferences-dialog.so* .
cp -fP $qRealDir/bin/release/libqrgui-text-editor.so* .
cp -fP $qRealDir/bin/release/libqrgui-thirdparty.so* .
cp -fP $qRealDir/bin/release/libqrgui-tool-plugin-interface.so* .
cp -fP $qRealDir/bin/release/libqrkernel.so* .
cp -fP $qRealDir/bin/release/libqrrepo.so* .
cp -fP $qRealDir/bin/release/libqrtext.so* .
cp -fP $qRealDir/bin/release/libqrutils.so* .
cp -fP $qRealDir/bin/release/libqscintilla2.so* .
cp -fP $qRealDir/bin/release/libqslog.so* .
cp -fP $qRealDir/bin/release/libqrtext.so* .

mkdir -p translations/ru
cp -fP $qRealDir/bin/release/translations/ru/qrgui_dialogs_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_editor_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_hotKeyManager_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_mainWindow_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_models_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_mouseGestures_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_pluginsManager_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_preferencesDialog_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_system_facade_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_textEditor_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_thirdparty_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrgui_toolPluginInterface_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrtext_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qrutils_ru.qm ./translations/ru/
cp -fP $qRealDir/bin/release/translations/ru/qt_ru.qm ./translations/ru/

# Copying TRIKStudio plugins
cp -fP $qRealDir/bin/release/libqextserialport.so* .
cp -fP $qRealDir/bin/release/librobots-2d-model.so* .
cp -fP $qRealDir/bin/release/librobots-interpreter-core.so* .
cp -fP $qRealDir/bin/release/librobots-trik-kit-interpreter-common.so* .
cp -fP $qRealDir/bin/release/librobots-kit-base.so* .
cp -fP $qRealDir/bin/release/librobots-trik-kit.so* .
cp -fP $qRealDir/bin/release/librobots-utils.so* .

mkdir -p plugins/editors
cp -fP   $qRealDir/bin/release/plugins/editors/librobotsMetamodel.so ./plugins/editors/

mkdir -p plugins/tools
cp -fP $qRealDir/bin/release/plugins/tools/librobots-plugin.so ./plugins/tools/

mkdir -p plugins/tools/kitPlugins
cp -fP $qRealDir/bin/release/plugins/tools/kitPlugins/librobots-trik-v6-interpreter.so ./plugins/tools/kitPlugins/
cp -fP $qRealDir/bin/release/plugins/tools/kitPlugins/librobots-trik-v62-interpreter.so ./plugins/tools/kitPlugins/

mkdir -p translations/ru/plugins/robots
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/interpreterCore_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/kitBase_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/robotsMetamodel_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/robotsPlugin_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/robots_utils_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/trikKitInterpreterCommon_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/trikV6KitInterpreter_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/trikV62KitInterpreter_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/twoDModel_ru.qm ./translations/ru/plugins/robots/
cp -fP $qRealDir/bin/release/translations/ru/plugins/robots/twoDModelRunner_ru.qm ./translations/ru/plugins/robots/

# Copying checker itself
cp -fP $qRealDir/bin/release/2D-model .
cp -fP $qRealDir/bin/release/patcher .
cp -fP $qRealDir/bin/release/check-solution.sh .

cd ..
cp -fP $qRealDir/bin/release/checker.sh .

# Copying fields, examples and tasks
cp -r $fieldsDir ./fields
cp -r $examplesDir ./examples
cp -r $tasksDir ./tasks

# Packing
cd ..

tar cvfz trik_checker.tar.gz trikStudio-checker
rm -rf trikStudio-checker
