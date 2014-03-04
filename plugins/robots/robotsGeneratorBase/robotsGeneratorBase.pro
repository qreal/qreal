QT += widgets

CONFIG += c++11

TEMPLATE = lib
CONFIG += plugin
DESTDIR = ../../../bin/
MOC_DIR = .moc
RCC_DIR = .moc
OBJECTS_DIR = .obj

DEFINES += ROBOTS_GENERATOR_LIBRARY

INCLUDEPATH += \
	$$PWD/../../../ \
	$$PWD/../../../qrgui \

LIBS += -L../../../bin -lqrkernel -lqrutils -lqrrepo

TRANSLATIONS = robotsGeneratorBase_ru.ts

RESOURCES = robotsGeneratorBase.qrc

HEADERS += \
	$$PWD/robotsGeneratorDeclSpec.h \
	$$PWD/robotsGeneratorPluginBase.h \
	$$PWD/masterGeneratorBase.h \
	$$PWD/generatorCustomizer.h \
	$$PWD/controlFlowGeneratorBase.h \
	$$PWD/readableControlFlowGenerator.h \
	$$PWD/gotoControlFlowGenerator.h \
	$$PWD/robotsDiagramVisitor.h \
	$$PWD/primaryControlFlowValidator.h \
	$$PWD/generatorFactoryBase.h \
	$$PWD/templateParametrizedEntity.h \
	$$PWD/parts/variables.h \
	$$PWD/parts/subprograms.h \
	$$PWD/parts/engines.h \
	$$PWD/parts/initTerminateCodeGenerator.h \
	$$PWD/parts/sensors.h \
	$$PWD/parts/functions.h \
	$$PWD/parts/images.h \
	$$PWD/semanticTree/semanticTree.h \
	$$PWD/semanticTree/semanticNode.h \
	$$PWD/semanticTree/nonZoneNode.h \
	$$PWD/semanticTree/zoneNode.h \
	$$PWD/semanticTree/simpleNode.h \
	$$PWD/semanticTree/finalNode.h \
	$$PWD/semanticTree/conditionalNode.h \
	$$PWD/semanticTree/ifNode.h \
	$$PWD/semanticTree/loopNode.h \
	$$PWD/semanticTree/rootNode.h \
	$$PWD/rules/semanticTransformationRule.h \
	$$PWD/rules/simpleRules/simpleBlockRuleBase.h \
	$$PWD/rules/simpleRules/simpleUnvisitedRule.h \
	$$PWD/rules/simpleRules/simpleVisitedOneZoneRule.h \
	$$PWD/rules/simpleRules/simpleVisitedRuleBase.h \
	$$PWD/rules/simpleRules/simpleMergedIfBranchesRule.h \
	$$PWD/rules/simpleRules/simpleIfInsideCycleRule.h \
	$$PWD/rules/ifRules/ifBlockRuleBase.h \
	$$PWD/rules/ifRules/ifWithBothUnvisitedRule.h \
	$$PWD/rules/ifRules/ifWithOneVisitedRule.h \
	$$PWD/rules/loopRules/loopBlockRuleBase.h \
	$$PWD/rules/loopRules/loopWithBothUnvisitedRule.h \
	$$PWD/rules/loopRules/loopWithIterationVisitedRule.h \
	$$PWD/rules/loopRules/loopWithNextVisitedRule.h \
    simpleGenerators/labelGenerator.h \
    simpleGenerators/gotoSimpleGenerator.h

SOURCES += \
	$$PWD/robotsGeneratorPluginBase.cpp \
	$$PWD/masterGeneratorBase.cpp \
	$$PWD/generatorCustomizer.cpp \
	$$PWD/controlFlowGeneratorBase.cpp \
	$$PWD/readableControlFlowGenerator.cpp \
	$$PWD/gotoControlFlowGenerator.cpp \
	$$PWD/robotsDiagramVisitor.cpp \
	$$PWD/primaryControlFlowValidator.cpp \
	$$PWD/generatorFactoryBase.cpp \
	$$PWD/templateParametrizedEntity.cpp \
	$$PWD/parts/variables.cpp \
	$$PWD/parts/subprograms.cpp \
	$$PWD/parts/engines.cpp \
	$$PWD/parts/initTerminateCodeGenerator.cpp \
	$$PWD/parts/sensors.cpp \
	$$PWD/parts/functions.cpp \
	$$PWD/parts/images.cpp \
	$$PWD/semanticTree/semanticTree.cpp \
	$$PWD/semanticTree/semanticNode.cpp \
	$$PWD/semanticTree/nonZoneNode.cpp \
	$$PWD/semanticTree/zoneNode.cpp \
	$$PWD/semanticTree/simpleNode.cpp \
	$$PWD/semanticTree/finalNode.cpp \
	$$PWD/semanticTree/conditionalNode.cpp \
	$$PWD/semanticTree/ifNode.cpp \
	$$PWD/semanticTree/loopNode.cpp \
	$$PWD/semanticTree/rootNode.cpp \
	$$PWD/rules/semanticTransformationRule.cpp \
	$$PWD/rules/simpleRules/simpleBlockRuleBase.cpp \
	$$PWD/rules/simpleRules/simpleUnvisitedRule.cpp \
	$$PWD/rules/simpleRules/simpleVisitedOneZoneRule.cpp \
	$$PWD/rules/simpleRules/simpleVisitedRuleBase.cpp \
	$$PWD/rules/simpleRules/simpleMergedIfBranchesRule.cpp \
	$$PWD/rules/simpleRules/simpleIfInsideCycleRule.cpp \
	$$PWD/rules/ifRules/ifBlockRuleBase.cpp \
	$$PWD/rules/ifRules/ifWithBothUnvisitedRule.cpp \
	$$PWD/rules/ifRules/ifWithOneVisitedRule.cpp \
	$$PWD/rules/loopRules/loopBlockRuleBase.cpp \
	$$PWD/rules/loopRules/loopWithBothUnvisitedRule.cpp \
	$$PWD/rules/loopRules/loopWithIterationVisitedRule.cpp \
	$$PWD/rules/loopRules/loopWithNextVisitedRule.cpp \
    simpleGenerators/labelGenerator.cpp \
    simpleGenerators/gotoSimpleGenerator.cpp

# Simple element generators & converters

HEADERS += \
	$$PWD/converters/templateParametrizedConverter.h \
	$$PWD/converters/templateParametrizedMultiConverter.h \
	$$PWD/converters/enumConverterBase.h \
	$$PWD/converters/codeConverterBase.h \
	$$PWD/converters/intPropertyConverter.h \
	$$PWD/converters/floatPropertyConverter.h \
	$$PWD/converters/boolPropertyConverter.h \
	$$PWD/converters/stringPropertyConverter.h \
	$$PWD/converters/nameNormalizerConverter.h \
	$$PWD/converters/functionBlockConverter.h \
	$$PWD/converters/inequalitySignConverter.h \
	$$PWD/converters/colorConverter.h \
	$$PWD/converters/breakModeConverter.h \
	$$PWD/converters/outputPortConverter.h \
	$$PWD/converters/enginePortsConverter.h \
	$$PWD/converters/typeConverter.h \
	$$PWD/converters/inputPortConverter.h \
	$$PWD/converters/functionInvocationConverter.h \
	$$PWD/simpleGenerators/abstractSimpleGenerator.h \
	$$PWD/simpleGenerators/nullGenerator.h \
	$$PWD/simpleGenerators/commentElementGenerator.h \
	$$PWD/simpleGenerators/functionElementGenerator.h \
	$$PWD/simpleGenerators/ifElementGenerator.h \
	$$PWD/simpleGenerators/infiniteLoopGenerator.h \
	$$PWD/simpleGenerators/forLoopGenerator.h \
	$$PWD/simpleGenerators/whileLoopGenerator.h \
	$$PWD/simpleGenerators/bindingGenerator.h \
	$$PWD/simpleGenerators/binding.h \
	$$PWD/simpleGenerators/enginesStopGenerator.h \
	$$PWD/simpleGenerators/timerGenerator.h \
	$$PWD/simpleGenerators/beepGenerator.h \
	$$PWD/simpleGenerators/playToneGenerator.h \
	$$PWD/simpleGenerators/finalNodeGenerator.h \
	$$PWD/simpleGenerators/nullificationEncoderGenerator.h \
	$$PWD/simpleGenerators/waitForColorBlockGenerator.h \
	$$PWD/simpleGenerators/waitForColorIntensityBlockGenerator.h \
	$$PWD/simpleGenerators/waitForLightBlockGenerator.h \
	$$PWD/simpleGenerators/waitForSonarBlockGenerator.h \
	$$PWD/simpleGenerators/waitForEncoderBlockGenerator.h \
	$$PWD/simpleGenerators/waitForTouchSensorBlockGenerator.h \
	$$PWD/simpleGenerators/initialNodeGenerator.h \
	$$PWD/simpleGenerators/enginesGenerator.h \
	$$PWD/simpleGenerators/waitForSoundBlockGenerator.h \
	$$PWD/simpleGenerators/waitForGyroscopeBlockGenerator.h \
	$$PWD/simpleGenerators/waitForAccelerometerBlockGenerator.h \
	$$PWD/simpleGenerators/drawPixelBlockGenerator.h \
	$$PWD/simpleGenerators/waitForButtonsBlockGenerator.h \
	$$PWD/simpleGenerators/drawLineBlockGenerator.h \
	$$PWD/simpleGenerators/drawCircleBlockGenerator.h \
	$$PWD/simpleGenerators/printTextBlockGenerator.h \
	$$PWD/simpleGenerators/drawRectBlockGenerator.h \
	$$PWD/simpleGenerators/clearScreenBlockGenerator.h \
	$$PWD/simpleGenerators/drawBlockGenerator.h \
	$$PWD/simpleGenerators/subprogramsSimpleGenerator.h \
	$$PWD/simpleGenerators/breakGenerator.h \
	$$PWD/simpleGenerators/continueGenerator.h \

SOURCES += \
	$$PWD/converters/templateParametrizedConverter.cpp \
	$$PWD/converters/templateParametrizedMultiConverter.cpp \
	$$PWD/converters/enumConverterBase.cpp \
	$$PWD/converters/codeConverterBase.cpp \
	$$PWD/converters/intPropertyConverter.cpp \
	$$PWD/converters/floatPropertyConverter.cpp \
	$$PWD/converters/boolPropertyConverter.cpp \
	$$PWD/converters/stringPropertyConverter.cpp \
	$$PWD/converters/nameNormalizerConverter.cpp \
	$$PWD/converters/functionBlockConverter.cpp \
	$$PWD/converters/inequalitySignConverter.cpp \
	$$PWD/converters/colorConverter.cpp \
	$$PWD/converters/breakModeConverter.cpp \
	$$PWD/converters/outputPortConverter.cpp \
	$$PWD/converters/enginePortsConverter.cpp \
	$$PWD/converters/typeConverter.cpp \
	$$PWD/converters/inputPortConverter.cpp \
	$$PWD/converters/functionInvocationConverter.cpp \
	$$PWD/simpleGenerators/abstractSimpleGenerator.cpp \
	$$PWD/simpleGenerators/nullGenerator.cpp \
	$$PWD/simpleGenerators/commentElementGenerator.cpp \
	$$PWD/simpleGenerators/functionElementGenerator.cpp \
	$$PWD/simpleGenerators/ifElementGenerator.cpp \
	$$PWD/simpleGenerators/infiniteLoopGenerator.cpp \
	$$PWD/simpleGenerators/forLoopGenerator.cpp \
	$$PWD/simpleGenerators/whileLoopGenerator.cpp \
	$$PWD/simpleGenerators/bindingGenerator.cpp \
	$$PWD/simpleGenerators/binding.cpp \
	$$PWD/simpleGenerators/enginesStopGenerator.cpp \
	$$PWD/simpleGenerators/timerGenerator.cpp \
	$$PWD/simpleGenerators/beepGenerator.cpp \
	$$PWD/simpleGenerators/playToneGenerator.cpp \
	$$PWD/simpleGenerators/finalNodeGenerator.cpp \
	$$PWD/simpleGenerators/nullificationEncoderGenerator.cpp \
	$$PWD/simpleGenerators/waitForColorBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForColorIntensityBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForLightBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForSonarBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForEncoderBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForTouchSensorBlockGenerator.cpp \
	$$PWD/simpleGenerators/initialNodeGenerator.cpp \
	$$PWD/simpleGenerators/enginesGenerator.cpp \
	$$PWD/simpleGenerators/waitForSoundBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForGyroscopeBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForAccelerometerBlockGenerator.cpp \
	$$PWD/simpleGenerators/drawPixelBlockGenerator.cpp \
	$$PWD/simpleGenerators/waitForButtonsBlockGenerator.cpp \
	$$PWD/simpleGenerators/drawLineBlockGenerator.cpp \
	$$PWD/simpleGenerators/drawCircleBlockGenerator.cpp \
	$$PWD/simpleGenerators/printTextBlockGenerator.cpp \
	$$PWD/simpleGenerators/drawRectBlockGenerator.cpp \
	$$PWD/simpleGenerators/clearScreenBlockGenerator.cpp \
	$$PWD/simpleGenerators/drawBlockGenerator.cpp \
	$$PWD/simpleGenerators/subprogramsSimpleGenerator.cpp \
	$$PWD/simpleGenerators/breakGenerator.cpp \
	$$PWD/simpleGenerators/continueGenerator.cpp \
