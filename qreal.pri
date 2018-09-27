# Copyright 2007-2015 QReal Research Group
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

contains(QT_VERSION, ^4\\.[0-9]\\..*){
	message("Cannot build using Qt version $${QT_VERSION}")
	error("Use at least Qt 5.1")
}

TEMPLATE = subdirs
SUBDIRS = \
	initvars.pro \
	qrmc \
	qrxc \
	qrgui \
	qrrepo \
	plugins \
	qrkernel \
	qrgraph \
	qrutils \
	qrtext \
	thirdparty \
	qrtranslations \

thirdparty.depends = initvars.pro
qrkernel.depends = thirdparty
qrgraph.depends = qrkernel
qrutils.depends = qrkernel qrtext
qrrepo.depends = qrkernel qrutils
qrxc.depends = qrutils
qrmc.depends = qrrepo
qrmc.file = $$PWD/qrmc/qrmc-lib.pro

qrgui.depends = \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrgraph \
	qrmc \
	thirdparty \

plugins.depends = \
	qrxc \
	qrrepo \
	qrutils \
	qrkernel \
	qrgraph \
	qrmc \
	qrgui \
	qrtext \

qrtext.depends = qrkernel

qrtranslations.depends = initvars.pro

OTHER_FILES += \
	$$PWD/.travis.yml \
	$$PWD/appveyor.yml \
	$$PWD/global.pri \
