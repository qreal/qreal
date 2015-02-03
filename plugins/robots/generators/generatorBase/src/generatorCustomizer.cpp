#include "generatorBase/generatorCustomizer.h"

using namespace generatorBase;
using namespace qReal;

GeneratorCustomizer::GeneratorCustomizer()
	: mDefaultInitialBlockType("RobotsMetamodel", "RobotsDiagram", "InitialNode")
	, mDefaultFinalBlockType("RobotsMetamodel", "RobotsDiagram", "FinalNode")
	, mDefaultConditionalBlockType("RobotsMetamodel", "RobotsDiagram", "IfBlock")
	, mDefaultLoopBlockType("RobotsMetamodel", "RobotsDiagram", "Loop")
	, mDefaultSwitchBlockType("RobotsMetamodel", "RobotsDiagram", "SwitchBlock")
	, mDefaultForkBlockType("RobotsMetamodel", "RobotsDiagram", "Fork")
	, mDefaultJoinBlockType("RobotsMetamodel", "RobotsDiagram", "Join")
	, mDefaultSubprogramCallBlockType("RobotsMetamodel", "RobotsDiagram", "Subprogram")
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

bool GeneratorCustomizer::isSubprogramCall(Id const &block) const
{
	return block.type() == mDefaultSubprogramCallBlockType;
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
	return block.type() == mDefaultSwitchBlockType;
}

bool GeneratorCustomizer::isFork(Id const &block) const
{
	return block.type() == mDefaultForkBlockType;
}

bool GeneratorCustomizer::isJoin(const Id &block) const
{
	return block.type() == mDefaultJoinBlockType;
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

	if (isJoin(block)) {
		return enums::semantics::joinBlock;
	}

	if (isFinalNode(block)) {
		return enums::semantics::finalBlock;
	}

	return enums::semantics::regularBlock;
}
