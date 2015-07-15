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
	generatorBase \
	trikV6GeneratorBase \
	trikV6QtsGenerator \
	trikV6FSharpGenerator \
	trikV6RuntimeUploaderPlugin \
	trikV62GeneratorBase \
	trikV62QtsGenerator \
	trikV62FSharpGenerator \
	trikV62RuntimeUploaderPlugin \
	nxtGeneratorBase \
	nxtOsekCGenerator \
	nxtRussianCGenerator \

trikV6GeneratorBase.subdir = $$PWD/trik/trikV6GeneratorBase
trikV6QtsGenerator.subdir = $$PWD/trik/trikV6QtsGenerator
trikV6FSharpGenerator.subdir = $$PWD/trik/trikV6FSharpGenerator
trikV6RuntimeUploaderPlugin.subdir = $$PWD/trik/trikV6RuntimeUploaderPlugin
trikV62GeneratorBase.subdir = $$PWD/trik/trikV62GeneratorBase
trikV62QtsGenerator.subdir = $$PWD/trik/trikV62QtsGenerator
trikV62FSharpGenerator.subdir = $$PWD/trik/trikV62FSharpGenerator
trikV62RuntimeUploaderPlugin.subdir = $$PWD/trik/trikV62RuntimeUploaderPlugin
nxtGeneratorBase.subdir = $$PWD/nxt/nxtGeneratorBase
nxtOsekCGenerator.subdir = $$PWD/nxt/nxtOsekCGenerator
nxtRussianCGenerator.subdir = $$PWD/nxt/nxtRussianCGenerator

trikV6GeneratorBase.depends = generatorBase
trikV6QtsGenerator.depends = trikV6GeneratorBase
trikV6FSharpGenerator.depends = trikV6GeneratorBase
trikV6RuntimeUploaderPlugin.depends = trikV6GeneratorBase
trikV62GeneratorBase.depends = generatorBase
trikV62QtsGenerator.depends = trikV62GeneratorBase
trikV62FSharpGenerator.depends = trikV62GeneratorBase
trikV62RuntimeUploaderPlugin.depends = trikV62GeneratorBase
nxtGeneratorBase.depends = generatorBase
nxtOsekCGenerator.depends = nxtGeneratorBase
nxtRussianCGenerator.depends = nxtGeneratorBase
