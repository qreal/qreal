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

TEMPLATE = subdirs

SUBDIRS = \
	interpreterCore \
	nullKitInterpreter \
	nxtKitInterpreter \
	trikKitInterpreterCommon \
	trikV6KitInterpreter \
	trikV62KitInterpreter \
	ev3KitInterpreter \
	robotsPlugin \
	pioneerKitInterpreter \

robotsPlugin.depends = interpreterCore
twoDModelRunner.depends = interpreterCore
trikV6KitInterpreter.depends = trikKitInterpreterCommon
trikV62KitInterpreter.depends = trikKitInterpreterCommon
pioneerKitInterpreter.depends = interpreterCore
