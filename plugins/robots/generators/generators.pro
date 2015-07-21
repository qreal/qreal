# Copyright 2007-2015 QReal Research Group, CyberTech Labs Ltd.
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
	generatorBase \
	trikGeneratorBase \
	trikQtsGeneratorLibrary \
	trikV6QtsGenerator \
#	trikV6FSharpGenerator \
	trikV6RuntimeUploaderPlugin \
	trikV62QtsGenerator \
#	trikV62FSharpGenerator \
	trikV62RuntimeUploaderPlugin \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \

trikGeneratorBase.subdir = $$PWD/trik/trikGeneratorBase
trikQtsGeneratorLibrary.subdir = $$PWD/trik/trikQtsGeneratorLibrary
trikV6QtsGenerator.subdir = $$PWD/trik/trikV6QtsGenerator
#trikV6FSharpGenerator.subdir = $$PWD/trik/trikV6FSharpGenerator
trikV6RuntimeUploaderPlugin.subdir = $$PWD/trik/trikV6RuntimeUploaderPlugin
trikV62QtsGenerator.subdir = $$PWD/trik/trikV62QtsGenerator
#trikV62FSharpGenerator.subdir = $$PWD/trik/trikV62FSharpGenerator
trikV62RuntimeUploaderPlugin.subdir = $$PWD/trik/trikV62RuntimeUploaderPlugin
nxtGeneratorBase.subdir = $$PWD/nxt/nxtGeneratorBase
nxtOsekCGenerator.subdir = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.subdir = $$PWD/nxt/nxtRussianCGenerator

trikQtsGeneratorLibrary.depends = trikGeneratorBase
trikV6QtsGenerator.depends = trikQtsGeneratorLibrary
#trikV6FSharpGenerator.depends = trikV6GeneratorBase
trikV62QtsGenerator.depends = trikQtsGeneratorLibrary
#trikV62FSharpGenerator.depends = trikV62GeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
