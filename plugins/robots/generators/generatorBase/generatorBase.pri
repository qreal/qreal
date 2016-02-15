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

QT += widgets

includes(plugins/robots/generators/generatorBase \
		qrutils \
		qrtext \
		plugins/robots/common/kitBase \
)

links(qrkernel qslog qrutils qrrepo qrtext qrgui-tool-plugin-interface qrgui-text-editor robots-kit-base)

DEFINES += ROBOTS_GENERATOR_LIBRARY

TRANSLATIONS = \
	$$PWD/../../../../qrtranslations/ru/plugins/robots/generatorBase_ru.ts \
	$$PWD/../../../../qrtranslations/fr/plugins/robots/generatorBase_fr.ts \

HEADERS += \
	$$PWD/include/generatorBase/robotsGeneratorDeclSpec.h \
	$$PWD/include/generatorBase/robotsGeneratorPluginBase.h \
	$$PWD/include/generatorBase/masterGeneratorBase.h \
	$$PWD/include/generatorBase/generatorCustomizer.h \
	$$PWD/include/generatorBase/controlFlowGeneratorBase.h \
	$$PWD/include/generatorBase/generatorFactoryBase.h \
	$$PWD/include/generatorBase/templateParametrizedEntity.h \
	$$PWD/include/generatorBase/robotsDiagramVisitor.h \
	$$PWD/include/generatorBase/primaryControlFlowValidator.h \
	$$PWD/include/generatorBase/semanticTree/semanticTree.h \
	$$PWD/include/generatorBase/semanticTree/semanticNode.h \
	$$PWD/include/generatorBase/semanticTree/nonZoneNode.h \
	$$PWD/include/generatorBase/semanticTree/zoneNode.h \
	$$PWD/include/generatorBase/semanticTree/simpleNode.h \
	$$PWD/include/generatorBase/semanticTree/finalNode.h \
	$$PWD/include/generatorBase/semanticTree/conditionalNode.h \
	$$PWD/include/generatorBase/semanticTree/ifNode.h \
	$$PWD/include/generatorBase/semanticTree/switchNode.h \
	$$PWD/include/generatorBase/semanticTree/loopNode.h \
	$$PWD/include/generatorBase/semanticTree/forkNode.h \
	$$PWD/include/generatorBase/semanticTree/joinNode.h \
	$$PWD/include/generatorBase/semanticTree/rootNode.h \
	$$PWD/include/generatorBase/parts/variables.h \
	$$PWD/include/generatorBase/parts/subprograms.h \
	$$PWD/include/generatorBase/parts/threads.h \
	$$PWD/include/generatorBase/parts/engines.h \
	$$PWD/include/generatorBase/parts/initTerminateCodeGenerator.h \
	$$PWD/include/generatorBase/parts/sensors.h \
	$$PWD/include/generatorBase/parts/functions.h \
	$$PWD/include/generatorBase/parts/deviceVariables.h \
	$$PWD/include/generatorBase/lua/luaProcessor.h \
	$$PWD/include/generatorBase/lua/precedenceConverterInterface.h \
	$$PWD/include/generatorBase/lua/precedenceConverter.h \
	$$PWD/include/generatorBase/lua/reservedFunctionsConverter.h \
	$$PWD/include/generatorBase/gotoControlFlowGenerator.h \

HEADERS += \
	$$PWD/src/readableControlFlowGenerator.h \
	$$PWD/src/rules/semanticTransformationRule.h \
	$$PWD/src/rules/simpleRules/simpleBlockRuleBase.h \
	$$PWD/src/rules/simpleRules/simpleUnvisitedRule.h \
	$$PWD/src/rules/simpleRules/simpleVisitedOneZoneRule.h \
	$$PWD/src/rules/simpleRules/simpleVisitedRuleBase.h \
	$$PWD/src/rules/simpleRules/simpleMergedIfBranchesRule.h \
	$$PWD/src/rules/simpleRules/simpleIfInsideCycleRule.h \
	$$PWD/src/rules/ifRules/ifBlockRuleBase.h \
	$$PWD/src/rules/ifRules/ifWithBothUnvisitedRule.h \
	$$PWD/src/rules/ifRules/ifWithOneVisitedRule.h \
	$$PWD/src/rules/loopRules/loopBlockRuleBase.h \
	$$PWD/src/rules/loopRules/loopWithBothUnvisitedRule.h \
	$$PWD/src/rules/loopRules/loopWithIterationVisitedRule.h \
	$$PWD/src/rules/loopRules/loopWithNextVisitedRule.h \
	$$PWD/src/rules/forkRules/forkRule.h \
	$$PWD/src/rules/joinRules/joinRule.h \
	$$PWD/src/rules/switchRules/switchInitializationRule.h \
	$$PWD/src/rules/switchRules/mergedSwitchBranchesRule.h \

SOURCES += \
	$$PWD/src/robotsGeneratorPluginBase.cpp \
	$$PWD/src/masterGeneratorBase.cpp \
	$$PWD/src/generatorCustomizer.cpp \
	$$PWD/src/controlFlowGeneratorBase.cpp \
	$$PWD/src/readableControlFlowGenerator.cpp \
	$$PWD/src/robotsDiagramVisitor.cpp \
	$$PWD/src/primaryControlFlowValidator.cpp \
	$$PWD/src/generatorFactoryBase.cpp \
	$$PWD/src/templateParametrizedEntity.cpp \
	$$PWD/src/parts/variables.cpp \
	$$PWD/src/parts/subprograms.cpp \
	$$PWD/src/parts/threads.cpp \
	$$PWD/src/parts/engines.cpp \
	$$PWD/src/parts/initTerminateCodeGenerator.cpp \
	$$PWD/src/parts/sensors.cpp \
	$$PWD/src/parts/functions.cpp \
	$$PWD/src/parts/deviceVariables.cpp \
	$$PWD/src/semanticTree/semanticTree.cpp \
	$$PWD/src/semanticTree/semanticNode.cpp \
	$$PWD/src/semanticTree/nonZoneNode.cpp \
	$$PWD/src/semanticTree/zoneNode.cpp \
	$$PWD/src/semanticTree/simpleNode.cpp \
	$$PWD/src/semanticTree/finalNode.cpp \
	$$PWD/src/semanticTree/conditionalNode.cpp \
	$$PWD/src/semanticTree/ifNode.cpp \
	$$PWD/src/semanticTree/switchNode.cpp \
	$$PWD/src/semanticTree/loopNode.cpp \
	$$PWD/src/semanticTree/forkNode.cpp \
	$$PWD/src/semanticTree/joinNode.cpp \
	$$PWD/src/semanticTree/rootNode.cpp \
	$$PWD/src/rules/semanticTransformationRule.cpp \
	$$PWD/src/rules/simpleRules/simpleBlockRuleBase.cpp \
	$$PWD/src/rules/simpleRules/simpleUnvisitedRule.cpp \
	$$PWD/src/rules/simpleRules/simpleVisitedOneZoneRule.cpp \
	$$PWD/src/rules/simpleRules/simpleVisitedRuleBase.cpp \
	$$PWD/src/rules/simpleRules/simpleMergedIfBranchesRule.cpp \
	$$PWD/src/rules/simpleRules/simpleIfInsideCycleRule.cpp \
	$$PWD/src/rules/ifRules/ifBlockRuleBase.cpp \
	$$PWD/src/rules/ifRules/ifWithBothUnvisitedRule.cpp \
	$$PWD/src/rules/ifRules/ifWithOneVisitedRule.cpp \
	$$PWD/src/rules/loopRules/loopBlockRuleBase.cpp \
	$$PWD/src/rules/loopRules/loopWithBothUnvisitedRule.cpp \
	$$PWD/src/rules/loopRules/loopWithIterationVisitedRule.cpp \
	$$PWD/src/rules/loopRules/loopWithNextVisitedRule.cpp \
	$$PWD/src/rules/forkRules/forkRule.cpp \
	$$PWD/src/rules/joinRules/joinRule.cpp \
	$$PWD/src/rules/switchRules/switchInitializationRule.cpp \
	$$PWD/src/rules/switchRules/mergedSwitchBranchesRule.cpp \
	$$PWD/src/gotoControlFlowGenerator.cpp \

# Simple element generators & converters

HEADERS += \
	$$PWD/include/generatorBase/converters/templateParametrizedConverter.h \
	$$PWD/include/generatorBase/converters/templateParametrizedMultiConverter.h \
	$$PWD/include/generatorBase/converters/regexpMultiConverter.h \
	$$PWD/include/generatorBase/converters/enumConverterBase.h \
	$$PWD/include/generatorBase/converters/dynamicPathConverter.h \
	$$PWD/include/generatorBase/simpleGenerators/abstractSimpleGenerator.h \
	$$PWD/include/generatorBase/simpleGenerators/binding.h \
	$$PWD/include/generatorBase/simpleGenerators/bindingGenerator.h \
	$$PWD/include/generatorBase/simpleGenerators/waitForButtonGenerator.h \
	$$PWD/include/generatorBase/simpleGenerators/randomIdGenerator.h \

HEADERS += \
	$$PWD/src/converters/reservedVariablesConverter.h \
	$$PWD/src/converters/codeConverterBase.h \
	$$PWD/src/converters/intPropertyConverter.h \
	$$PWD/src/converters/floatPropertyConverter.h \
	$$PWD/src/converters/boolPropertyConverter.h \
	$$PWD/src/converters/stringPropertyConverter.h \
	$$PWD/src/converters/nameNormalizerConverter.h \
	$$PWD/src/converters/functionBlockConverter.h \
	$$PWD/src/converters/inequalitySignConverter.h \
	$$PWD/src/converters/breakModeConverter.h \
	$$PWD/src/converters/portNameConverter.h \
	$$PWD/src/converters/enginePortsConverter.h \
	$$PWD/src/converters/typeConverter.h \
	$$PWD/src/converters/switchConditionsMerger.h \
	$$PWD/src/simpleGenerators/nullGenerator.h \
	$$PWD/src/simpleGenerators/commentElementGenerator.h \
	$$PWD/src/simpleGenerators/functionElementGenerator.h \
	$$PWD/src/simpleGenerators/ifElementGenerator.h \
	$$PWD/src/simpleGenerators/infiniteLoopGenerator.h \
	$$PWD/src/simpleGenerators/forLoopGenerator.h \
	$$PWD/src/simpleGenerators/whileLoopGenerator.h \
	$$PWD/src/simpleGenerators/forkCallGenerator.h \
	$$PWD/src/simpleGenerators/joinGenerator.h \
	$$PWD/src/simpleGenerators/switchGenerator.h \
	$$PWD/src/simpleGenerators/enginesStopGenerator.h \
	$$PWD/src/simpleGenerators/timerGenerator.h \
	$$PWD/src/simpleGenerators/beepGenerator.h \
	$$PWD/src/simpleGenerators/playToneGenerator.h \
	$$PWD/src/simpleGenerators/finalNodeGenerator.h \
	$$PWD/src/simpleGenerators/nullificationEncoderGenerator.h \
	$$PWD/src/simpleGenerators/waitForColorIntensityBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForLightBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForSonarBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForEncoderBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForTouchSensorBlockGenerator.h \
	$$PWD/src/simpleGenerators/initialNodeGenerator.h \
	$$PWD/src/simpleGenerators/enginesGenerator.h \
	$$PWD/src/simpleGenerators/waitForSoundBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForGyroscopeBlockGenerator.h \
	$$PWD/src/simpleGenerators/waitForAccelerometerBlockGenerator.h \
	$$PWD/src/simpleGenerators/printTextBlockGenerator.h \
	$$PWD/src/simpleGenerators/clearScreenBlockGenerator.h \
	$$PWD/src/simpleGenerators/subprogramsSimpleGenerator.h \
	$$PWD/src/simpleGenerators/breakGenerator.h \
	$$PWD/src/simpleGenerators/continueGenerator.h \
	$$PWD/src/simpleGenerators/labelGenerator.h \
	$$PWD/src/simpleGenerators/gotoSimpleGenerator.h \
	$$PWD/src/simpleGenerators/variableInitGenerator.h \
	$$PWD/src/simpleGenerators/randomInitGenerator.h \
	$$PWD/src/simpleGenerators/sendMessageThreadsGenerator.h \
	$$PWD/src/simpleGenerators/receiveMessageThreadsGenerator.h \
	$$PWD/src/simpleGenerators/killThreadGenerator.h \
	$$PWD/src/simpleGenerators/getButtonCodeGenerator.h \
	$$PWD/src/lua/luaPrinter.h \

SOURCES += \
	$$PWD/src/converters/templateParametrizedConverter.cpp \
	$$PWD/src/converters/templateParametrizedMultiConverter.cpp \
	$$PWD/src/converters/regexpMultiConverter.cpp \
	$$PWD/src/converters/enumConverterBase.cpp \
	$$PWD/src/converters/dynamicPathConverter.cpp \
	$$PWD/src/converters/reservedVariablesConverter.cpp \
	$$PWD/src/converters/codeConverterBase.cpp \
	$$PWD/src/converters/intPropertyConverter.cpp \
	$$PWD/src/converters/floatPropertyConverter.cpp \
	$$PWD/src/converters/boolPropertyConverter.cpp \
	$$PWD/src/converters/stringPropertyConverter.cpp \
	$$PWD/src/converters/nameNormalizerConverter.cpp \
	$$PWD/src/converters/functionBlockConverter.cpp \
	$$PWD/src/converters/inequalitySignConverter.cpp \
	$$PWD/src/converters/breakModeConverter.cpp \
	$$PWD/src/converters/portNameConverter.cpp \
	$$PWD/src/converters/enginePortsConverter.cpp \
	$$PWD/src/converters/typeConverter.cpp \
	$$PWD/src/converters/switchConditionsMerger.cpp \
	$$PWD/src/simpleGenerators/abstractSimpleGenerator.cpp \
	$$PWD/src/simpleGenerators/nullGenerator.cpp \
	$$PWD/src/simpleGenerators/commentElementGenerator.cpp \
	$$PWD/src/simpleGenerators/functionElementGenerator.cpp \
	$$PWD/src/simpleGenerators/ifElementGenerator.cpp \
	$$PWD/src/simpleGenerators/infiniteLoopGenerator.cpp \
	$$PWD/src/simpleGenerators/forLoopGenerator.cpp \
	$$PWD/src/simpleGenerators/whileLoopGenerator.cpp \
	$$PWD/src/simpleGenerators/forkCallGenerator.cpp \
	$$PWD/src/simpleGenerators/joinGenerator.cpp \
	$$PWD/src/simpleGenerators/switchGenerator.cpp \
	$$PWD/src/simpleGenerators/bindingGenerator.cpp \
	$$PWD/src/simpleGenerators/binding.cpp \
	$$PWD/src/simpleGenerators/enginesStopGenerator.cpp \
	$$PWD/src/simpleGenerators/timerGenerator.cpp \
	$$PWD/src/simpleGenerators/beepGenerator.cpp \
	$$PWD/src/simpleGenerators/playToneGenerator.cpp \
	$$PWD/src/simpleGenerators/finalNodeGenerator.cpp \
	$$PWD/src/simpleGenerators/nullificationEncoderGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForColorIntensityBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForLightBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForSonarBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForEncoderBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForTouchSensorBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/initialNodeGenerator.cpp \
	$$PWD/src/simpleGenerators/enginesGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForSoundBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForGyroscopeBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForAccelerometerBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/waitForButtonGenerator.cpp \
	$$PWD/src/simpleGenerators/printTextBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/clearScreenBlockGenerator.cpp \
	$$PWD/src/simpleGenerators/subprogramsSimpleGenerator.cpp \
	$$PWD/src/simpleGenerators/breakGenerator.cpp \
	$$PWD/src/simpleGenerators/continueGenerator.cpp \
	$$PWD/src/simpleGenerators/labelGenerator.cpp \
	$$PWD/src/simpleGenerators/gotoSimpleGenerator.cpp \
	$$PWD/src/simpleGenerators/variableInitGenerator.cpp \
	$$PWD/src/simpleGenerators/randomInitGenerator.cpp \
	$$PWD/src/simpleGenerators/sendMessageThreadsGenerator.cpp \
	$$PWD/src/simpleGenerators/receiveMessageThreadsGenerator.cpp \
	$$PWD/src/simpleGenerators/killThreadGenerator.cpp \
	$$PWD/src/simpleGenerators/getButtonCodeGenerator.cpp \
	$$PWD/src/simpleGenerators/randomIdGenerator.cpp \
	$$PWD/src/lua/luaProcessor.cpp \
	$$PWD/src/lua/luaPrinter.cpp \
	$$PWD/src/lua/reservedFunctionsConverter.cpp \
	$$PWD/src/lua/precedenceConverter.cpp \
