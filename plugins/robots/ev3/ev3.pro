# Copyright 2016 Dmitry Mordvinov
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
	ev3Common \
	ev3Interpreter \
	generators \
	examples \

editor.file = $$PWD/editor/ev3Metamodel.pro

ev3Interpreter.depends = ev3Common
generators.depends = ev3Common
