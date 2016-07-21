/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

bool GeneratorCustomizer::isInitialNode(const qReal::Id &block) const
{
	return block.type() == mDefaultInitialBlockType;
}

bool GeneratorCustomizer::isFinalNode(const Id &block) const
{
	return block.type() == mDefaultFinalBlockType;
}

bool GeneratorCustomizer::isSubprogramCall(const Id &block) const
{
	return block.type() == mDefaultSubprogramCallBlockType;
}

bool GeneratorCustomizer::isConditional(const Id &block) const
{
	return block.type() == mDefaultConditionalBlockType;
}

bool GeneratorCustomizer::isLoop(const Id &block) const
{
	return block.type() == mDefaultLoopBlockType;
}

bool GeneratorCustomizer::isSwitch(const Id &block) const
{
	return block.type() == mDefaultSwitchBlockType;
}

bool GeneratorCustomizer::isFork(const Id &block) const
{
	return block.type() == mDefaultForkBlockType;
}

bool GeneratorCustomizer::isJoin(const Id &block) const
{
	return block.type() == mDefaultJoinBlockType;
}

enums::semantics::Semantics GeneratorCustomizer::semanticsOf(const Id &block) const
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
