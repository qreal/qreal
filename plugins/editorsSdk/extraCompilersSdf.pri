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

defineTest(addExtraCompiler) {
	eval($${1}.commands = $$FAKE_COMMAND)
	eval($${1}.input = QREAL_XML)
	eval($${1}.output = $${2})
	eval($${1}.variable_out = GENERATED_RESOURCES)

	export($${1}.output)
	export($${1}.input)
	export($${1}.commands)
	export($${1}.variable_out)

	QMAKE_EXTRA_COMPILERS += $${1}

	export(QMAKE_EXTRA_COMPILERS)

	return(true)
}

SDF_FILES = $$files($$PWD/../$$QREAL_EDITOR_NAME$$quote(/generated/shapes/*.sdf))

for(sdfFile, SDF_FILES) {
	FILE_NAME_LONG = $${sdfFile}
	COMPILER_NAME_LONG = $$basename(FILE_NAME_LONG)
	COMPILER_NAME = $$section(COMPILER_NAME_LONG,".",0,0)
	FILE_NAME = $$quote(generated/shapes/)$$COMPILER_NAME$$quote(.sdf)
	addExtraCompiler($$COMPILER_NAME, $$FILE_NAME)
}
