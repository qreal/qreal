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

#include "coreBlocksFactory.h"

#include <qrutils/interpreter/blocks/initialBlock.h>
#include <qrutils/interpreter/blocks/finalBlock.h>

#include "details/timerBlock.h"
#include <qrutils/interpreter/blocks/functionBlock.h>
#include <qrutils/interpreter/blocks/variableInitBlock.h>
#include <qrutils/interpreter/blocks/randomInitBlock.h>
#include <qrutils/interpreter/blocks/emptyBlock.h>
#include <qrutils/interpreter/blocks/loopBlock.h>
#include <qrutils/interpreter/blocks/forkBlock.h>
#include <qrutils/interpreter/blocks/joinBlock.h>
#include <qrutils/interpreter/blocks/killThreadBlock.h>
#include <qrutils/interpreter/blocks/sendThreadMessageBlock.h>
#include <qrutils/interpreter/blocks/receiveThreadMessageBlock.h>
#include <qrutils/interpreter/blocks/ifBlock.h>
#include <qrutils/interpreter/blocks/switchBlock.h>
#include <qrutils/interpreter/blocks/commentBlock.h>
#include <qrutils/interpreter/blocks/subprogramBlock.h>
#include <qrutils/interpreter/blocks/unsupportedBlock.h>

#include "details/printTextBlock.h"
#include "details/clearScreenBlock.h"

#include <twoDModel/blocks/markerDownBlock.h>
#include <twoDModel/blocks/markerUpBlock.h>

#include <kitBase/blocksBase/common/randomInitBlock.h>

using namespace interpreterCore::coreBlocks;

qReal::interpretation::Block *CoreBlocksFactory::produceBlock(const qReal::Id &element)
{
	if (elementMetatypeIs(element, "InitialNode")) {
		return new qReal::interpretation::blocks::InitialBlock();
	} else if (elementMetatypeIs(element, "FinalNode")) {
		return new qReal::interpretation::blocks::FinalBlock();
	} else if (elementMetatypeIs(element, "Timer")) {
		return new details::TimerBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "CommentBlock")) {
		return new qReal::interpretation::blocks::CommentBlock;
	} else if (elementMetatypeIs(element, "IfBlock")) {
		return new qReal::interpretation::blocks::IfBlock();
	} else if (elementMetatypeIs(element, "FiBlock")) {
		return new qReal::interpretation::blocks::EmptyBlock();
	} else if (elementMetatypeIs(element, "SwitchBlock")) {
		return new qReal::interpretation::blocks::SwitchBlock();
	} else if (elementMetatypeIs(element, "Loop")) {
		return new qReal::interpretation::blocks::LoopBlock();
	} else if (elementMetatypeIs(element, "Fork")) {
		return new qReal::interpretation::blocks::ForkBlock();
	} else if (elementMetatypeIs(element, "Join")) { // todo: waiting
		return new qReal::interpretation::blocks::JoinBlock();
	} else if (elementMetatypeIs(element, "KillThread")) {
		return new qReal::interpretation::blocks::KillThreadBlock();
	} else if (elementMetatypeIs(element, "SendMessageThreads")) {
		return new qReal::interpretation::blocks::SendThreadMessageBlock();
	} else if (elementMetatypeIs(element, "ReceiveMessageThreads")) {
		return new qReal::interpretation::blocks::ReceiveThreadMessageBlock();
	} else if (elementMetatypeIs(element, "Subprogram")) {
		return new qReal::interpretation::blocks::SubprogramBlock();
	} else if (elementMetatypeIs(element, "Function")) {
		return new qReal::interpretation::blocks::FunctionBlock();
	} else if (elementMetatypeIs(element, "VariableInit")) {
		return new qReal::interpretation::blocks::VariableInitBlock();
	} else if (elementMetatypeIs(element, "Randomizer")) {
		return new kitBase::blocksBase::common::RandomInitBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "PrintText")) {
		return new details::PrintTextBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "ClearScreen")) {
		return new details::ClearScreenBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "MarkerDown")) {
		return new twoDModel::blocks::MarkerDownBlock(mRobotModelManager->model());
	} else if (elementMetatypeIs(element, "MarkerUp")) {
		return new twoDModel::blocks::MarkerUpBlock(mRobotModelManager->model());
	}

	return nullptr;
}

qReal::IdList CoreBlocksFactory::providedBlocks() const
{
	return {
		id("ControlFlow")
		, id("InitialNode")
		, id("FinalNode")
		, id("Timer")
		, id("CommentBlock")
		, id("IfBlock")
		, id("FiBlock")
		, id("SwitchBlock")
		, id("Loop")
		, id("Fork")
		, id("Join")
		, id("SendMessageThreads")
		, id("ReceiveMessageThreads")
		, id("KillThread")
		, id("Subprogram")
		, id("Function")
		, id("VariableInit")
		, id("Randomizer")
		, id("ClearScreen")
		, id("PrintText")
		, id("MarkerDown")
		, id("MarkerUp")
	};
}

qReal::IdList CoreBlocksFactory::blocksToDisable() const
{
	qReal::IdList result;

	if (!mRobotModelManager->model().name().contains("TwoD")) {
		result
				<< id("MarkerDown")
				<< id("MarkerUp");
	}

	return result;
}
