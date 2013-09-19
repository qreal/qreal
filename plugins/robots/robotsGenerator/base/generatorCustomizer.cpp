#include "generatorCustomizer.h"

using namespace qReal::robots::generators;

GeneratorCustomizer::GeneratorCustomizer()
	: mDefaultInitialBlockType("RobotsMetamodel", "RobotsDiagram", "InitialNode")
	, mDefaultFinalBlockType("RobotsMetamodel", "RobotsDiagram", "FinalNode")
	, mDefaultConditionalBlockType("RobotsMetamodel", "RobotsDiagram", "IfBlock")
	, mDefaultLoopBlockType("RobotsMetamodel", "RobotsDiagram", "Loop")
	, mDefaultForkBlockType("RobotsMetamodel", "RobotsDiagram", "Fork")
{
}

void GeneratorCustomizer::initialize()
{
	factory()->initialize();
}

bool GeneratorCustomizer::isInitialNode(qReal::Id const &block) const
{
	return block.type() == mDefaultInitialBlockType;
}

bool GeneratorCustomizer::isFinalNode(Id const &block) const
{
	return block.type() == mDefaultFinalBlockType;
}

bool GeneratorCustomizer::isConditional(Id const &block) const
{
	return block.type() == mDefaultConditionalBlockType;
}

bool GeneratorCustomizer::isLoop(Id const &block) const
{
	return block.type() == mDefaultLoopBlockType;
}

bool GeneratorCustomizer::isSwitch(Id const &block) const
{
	// TODO:
	Q_UNUSED(block)
	return false;
}

bool GeneratorCustomizer::isFork(Id const &block) const
{
	return block.type() == mDefaultForkBlockType;
}

enums::semantics::Semantics GeneratorCustomizer::semanticsOf(Id const &block) const
{
	if (isConditional(block)) {
		return enums::semantics::conditionalBlock;
	}

	if (isLoop(block)) {
		return enums::semantics::loopBlock;
	}

	if (isSwitch(block)) {
		return enums::semantics::switchBlock;
	}

	if (isFork(block)) {
		return enums::semantics::forkBlock;
	}

	if (isFinalNode(block)) {
		return enums::semantics::finalBlock;
	}

	return enums::semantics::regularBlock;
}
