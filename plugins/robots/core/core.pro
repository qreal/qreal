# Copyright 2013-2016 CyberTech Labs Ltd, Dmitry Mordvinov, Yurii Litvinov
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
	kitBase \
	interpreterCore \
	robotsPlugin \
	generatorBase \
	twoDModel \
	nullKitInterpreter \

editor.file = $$PWD/editor/robotsMetamodel.pro

interpreterCore.depends = kitBase twoDModel
robotsPlugin.depends = interpreterCore
generatorBase.depends = kitBase
twoDModel.depends = kitBase
nullKitInterpreter.depends = kitBase
