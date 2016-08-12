# Copyright 2012-2016 Yurii Litvinov, Dmitry Mordvinov
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

# Here must be the call to QRMC console generator

TEMPLATE = subdirs

SUBDIRS = \
	doc \
	images \
	common \
	nxt \
	ev3 \
	trik \

common.file = $$PWD/common/robotsMetamodel.pro
nxt.file = $$PWD/nxt/nxtMetamodel.pro
ev3.file = $$PWD/ev3/ev3Metamodel.pro
trik.file = $$PWD/trik/trikMetamodel.pro

nxt.depends = common
ev3.depends = common
trik.depends = common
