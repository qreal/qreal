# Copyright 2007-2015 QReal Research Group and Dmitry Mordvinov
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

TEMPLATE = subdirs

SUBDIRS = \
	editor \
	examples \
	common \
	interpreters \
	generators \
	utils \
	checker \
	thirdparty \
	#$$PWD/thirdparty/trikRuntime/trikScriptMinimal.pro \
	qslog \
	trikKernel \
	trikNetwork \
	trikControl \
	trikHal \
	trikScriptRunner \
	$$PWD/thirdparty/trikRuntime/translations \

qslog.file = $$PWD/thirdparty/trikRuntime/qslog/QsLogSharedLibrary.pro
#qslog.file = $$PWD/../../thirdparty/qslog/QsLogSharedLibrary.pro

trikScriptRunner.subdir = $$PWD/thirdparty/trikRuntime/trikScriptRunner
trikKernel.subdir = $$PWD/thirdparty/trikRuntime/trikKernel
trikNetwork.subdir = $$PWD/thirdparty/trikRuntime/trikNetwork
trikControl.subdir = $$PWD/thirdparty/trikRuntime/trikControl
trikHal.subdir = $$PWD/thirdparty/trikRuntime/trikHal

utils.depends = thirdparty
common.depends = thirdparty utils
interpreters.depends = common thirdparty utils trikScriptRunner trikControl
generators.depends = common utils
checker.depends = interpreters
trikControl.depends = trikKernel trikHal qslog
trikKernel.depends = qslog
trikNetwork.depends = trikKernel qslog
trikScriptRunner.depends = trikControl trikKernel trikNetwork qslog
trikHal.depends = qslog trikKernel
