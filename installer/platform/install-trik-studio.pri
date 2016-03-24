# Copyright 2016 Anton Midyukov
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

unix:!macx {
	isEmpty(PREFIX) {
		PREFIX = /usr/local
	}

	contains(QMAKE_HOST.arch, x86_64) {LIBDIR="$$PREFIX/lib64"} else {LIBDIR="$$PREFIX/lib"}
	BINDIR = $$PREFIX/bin
	DATADIR = $$PREFIX/share
	PROGRAMDIR = $$DATADIR/trikStudio
	PLUGINDIR = $$LIBDIR/trikStudio

	#MAKE INSTALL

	res.path = $$PROGRAMDIR
	res.extra = install -Dp -m0755 bin/release/qreal $(INSTALL_ROOT)$$BINDIR/trikStudio;\
	test -d $(INSTALL_ROOT)$$LIBDIR/ || mkdir -p $(INSTALL_ROOT)$$LIBDIR/;\
	cp -a bin/release/*.so* $(INSTALL_ROOT)$$LIBDIR;\
	test -d $(INSTALL_ROOT)$$PLUGINDIR || mkdir -p $(INSTALL_ROOT)$$PLUGINDIR;\
	cp -a bin/release/plugins/* $(INSTALL_ROOT)$$PLUGINDIR/;\
	cp -a bin/release/translations $(INSTALL_ROOT)$$PROGRAMDIR/;\
	cp -a bin/release/help $(INSTALL_ROOT)$$PROGRAMDIR/;\
	cp -a bin/release/images $(INSTALL_ROOT)$$PROGRAMDIR/;\
	cp -a bin/release/examples $(INSTALL_ROOT)$$PROGRAMDIR/;\
	cp -a bin/release/splashscreen.png $(INSTALL_ROOT)$$PROGRAMDIR/images;\
	test -d $(INSTALL_ROOT)/etc || mkdir -p $(INSTALL_ROOT)/etc;\
	echo 'pathToLogs=\~/.config/logs/' > $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToImages=$$PROGRAMDIR/images/iconset1/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToHelp=$$PROGRAMDIR/help/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToTranslations=$$PROGRAMDIR/translations/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToTrikRuntime=$$PROGRAMDIR/trikRuntime/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToPascalRuntime=$$PROGRAMDIR/trikSharp/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToGeneratorRoot=$$PROGRAMDIR/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToExamples=$$PROGRAMDIR/examples/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToEditorPlugins=$$PLUGINDIR/editors/" >> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToToolPlugins=$$PLUGINDIR/tools/">> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToKitPlugins=$$PLUGINDIR/tools/kitPlugins/">> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	echo "pathToSplashscreen=$$PROGRAMDIR/images/splashscreen.png">> $(INSTALL_ROOT)/etc/trikStudio.conf;\
	test -d $(INSTALL_ROOT)$$DATADIR/icons/hicolor/16x16/apps || mkdir -p $(INSTALL_ROOT)$$DATADIR/icons/hicolor/16x16/apps;\
	cp -a installer/images/trik-studio-16x16.png $(INSTALL_ROOT)$$DATADIR/icons/hicolor/16x16/apps/trikStudio.png;\
	test -d $(INSTALL_ROOT)$$DATADIR/icons/hicolor/32x32/apps || mkdir -p $(INSTALL_ROOT)$$DATADIR/icons/hicolor/32x32/apps;\
	cp -a installer/images/trik-studio-32x32.png $(INSTALL_ROOT)$$DATADIR/icons/hicolor/32x32/apps/trikStudio.png;\
	test -d $(INSTALL_ROOT)$$DATADIR/icons/hicolor/48x48/apps || mkdir -p $(INSTALL_ROOT)$$DATADIR/icons/hicolor/48x48/apps/;\
	cp -a installer/images/trik-studio-48x48.png $(INSTALL_ROOT)$$DATADIR/icons/hicolor/48x48/apps/trikStudio.png;\
	test -d $(INSTALL_ROOT)$$DATADIR/applications || mkdir -p $(INSTALL_ROOT)$$DATADIR/applications;\
	cp -a installer/platform/trikStudio.desktop.in $(INSTALL_ROOT)$$DATADIR/applications/trikStudio.desktop

	INSTALLS += res
}
