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
	echo "Usage: build-checker-installer.sh <path to Qt> [<path to QReal>] [<path to directory with fields>] [<path to directory with examples>]"
	echo "<path to Qt> --- root of your Qt 5.5 installation"
	echo "<path to QReal> --- path to QReal sources root (with completed release build of qrealRobots.pro)."
	echo "                                   Defaults to \"../..\""
	echo "<path to \"fields\" directory> --- path to directory with prepared fields."
	echo "                                   Defaults to \"<path to QReal/qrtest/trikStudioSimulatorTests/fields/fields>.\""
	echo "<path to \"examples\" directory> --- path to directory with example saves."
	echo "                                   Defaults to \"<path to QReal/qrtest/trikStudioSimulatorTests/solutions>.\""
	echo "Example: ./build-checker-installer.sh ~/Qt ~/stepic-examples ~/qreal"
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

mkdir -p trikStudio-checker/bin
cd trikStudio-checker/bin

# Copying required Qt libraries
cp -rf $qtDir/5.5/gcc_64/plugins/iconengines .

mkdir imageformats
cp -f  $qtDir/5.5/gcc_64/plugins/imageformats/libqsvg.so imageformats/

mkdir platforms
cp -f $qtDir/5.5/gcc_64/plugins/platforms/libqminimal.so platforms/

cp -f $qtDir/5.5/gcc_64/lib/libicudata.so.54* .
cp -f $qtDir/5.5/gcc_64/lib/libicui18n.so.54* .
cp -f $qtDir/5.5/gcc_64/lib/libicuuc.so.54* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Core.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Gui.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Network.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5PrintSupport.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Script.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Svg.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Widgets.so* .
cp -f $qtDir/5.5/gcc_64/lib/libQt5Xml.so* .

# Copying QReal librariesbuild-checker-installer
cp -f $qRealDir/bin/release/changelog.txt .
cp -f $qRealDir/bin/release/libqrgui-brand-manager.so* .
cp -f $qRealDir/bin/release/libqrgui-controller.so* .
cp -f $qRealDir/bin/release/libqrgui-dialogs.so* .
cp -f $qRealDir/bin/release/libqrgui-editor.so* .
cp -f $qRealDir/bin/release/libqrgui-facade.so* .
cp -f $qRealDir/bin/release/libqrgui-models.so* .
cp -f $qRealDir/bin/release/libqrgui-mouse-gestures.so* .
cp -f $qRealDir/bin/release/libqrgui-plugin-manager.so* .
cp -f $qRealDir/bin/release/libqrgui-preferences-dialog.so* .
cp -f $qRealDir/bin/release/libqrgui-text-editor.so* .
cp -f $qRealDir/bin/release/libqrgui-thirdparty.so* .
cp -f $qRealDir/bin/release/libqrgui-tool-plugin-interface.so* .
cp -f $qRealDir/bin/release/libqrkernel.so* .
cp -f $qRealDir/bin/release/libqrrepo.so* .
cp -f $qRealDir/bin/release/libqrtext.so* .
cp -f $qRealDir/bin/release/libqrutils.so* .
cp -f $qRealDir/bin/release/libqscintilla2.so* .
cp -f $qRealDir/bin/release/libqslog.so* .
cp -f $qRealDir/bin/release/libqrtext.so* .

mkdir -p translations/ru
cp -f $qRealDir/bin/release/translations/ru/qrgui_dialogs_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_editor_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_hotKeyManager_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_mainWindow_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_models_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_mouseGestures_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_pluginsManager_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_preferencesDialog_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_system_facade_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_textEditor_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_thirdparty_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrgui_toolPluginInterface_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrtext_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qrutils_ru.qm ./translations/ru/
cp -f $qRealDir/bin/release/translations/ru/qt_ru.qm ./translations/ru/

# Copying TRIKStudio plugins
cp -f $qRealDir/bin/release/libqextserialport.so* .
cp -f $qRealDir/bin/release/librobots-2d-model.so* .
cp -f $qRealDir/bin/release/librobots-interpreter-core.so* .
cp -f $qRealDir/bin/release/librobots-trik-kit-interpreter-common.so* .
cp -f $qRealDir/bin/release/librobots-kit-base.so* .
cp -f $qRealDir/bin/release/librobots-trik-kit.so* .
cp -f $qRealDir/bin/release/librobots-utils.so* .

mkdir -p plugins/editors
cp -f   $qRealDir/bin/release/plugins/editors/librobotsMetamodel.so ./plugins/editors/

mkdir -p plugins/tools
cp -f $qRealDir/bin/release/plugins/tools/librobots-plugin.so ./plugins/tools/

mkdir -p plugins/tools/kitPlugins
cp -f $qRealDir/bin/release/plugins/tools/kitPlugins/librobots-trik-v6-interpreter.so ./plugins/tools/kitPlugins/
cp -f $qRealDir/bin/release/plugins/tools/kitPlugins/librobots-trik-v62-interpreter.so ./plugins/tools/kitPlugins/

mkdir -p translations/ru/plugins/robots
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/interpreterCore_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/kitBase_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/robotsMetamodel_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/robotsPlugin_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/robots_utils_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/trikKitInterpreterCommon_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/trikV6KitInterpreter_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/trikV62KitInterpreter_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/twoDModel_ru.qm ./translations/ru/plugins/robots/
cp -f $qRealDir/bin/release/translations/ru/plugins/robots/twoDModelRunner_ru.qm ./translations/ru/plugins/robots/

# Copying checker itself
cp -f $qRealDir/bin/release/2D-model .
cp -f $qRealDir/bin/release/patcher .
cp -f $qRealDir/bin/release/check-solution.sh .

cd ..
cp -f $qRealDir/bin/release/checker.sh .

# Copying fields and examples
cp -r $fieldsDir ./fields
cp -r $examplesDir ./examples

cd ..

tar cvfzh checker.tar.gz trikStudio-checker
rm -rf trikStudio-checker
