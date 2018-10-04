# Copyright 2015-2016 Kirill Nikolsky, Dmitry Mordvinov
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

contains(QT_VERSION, ^4.*){
	message("Cannot build qreal using Qt version $${QT_VERSION}")
	error("Use at least Qt 5.3")
}

include(qreal.pri)

SUBDIRS += \
	qrtest \

qrtest.depends = \
	qrmc \
	qrxc \
	qrgui \
	qrrepo \
	qrkernel \
	qrutils \
	thirdparty \
	plugins \

qrtest.file = $$PWD/qrtest/qrrobotsguitest.pro

plugins.file = $$PWD/plugins/robots.pro
