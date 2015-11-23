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

#include "trikGeneratorBase/trikGeneratorPluginBase.h"

#include <qrutils/inFile.h>

using namespace trik;

TrikGeneratorPluginBase::TrikGeneratorPluginBase(kitBase::robotModel::RobotModelInterface * const robotModel
		, kitBase::blocksBase::BlocksFactoryInterface * const blocksFactory)
	: mRobotModel(robotModel)
	, mBlocksFactory(blocksFactory)
{
}

TrikGeneratorPluginBase::~TrikGeneratorPluginBase()
{
}

QList<kitBase::robotModel::RobotModelInterface *> TrikGeneratorPluginBase::robotModels()
{
	return { mRobotModel.data() };
}

kitBase::blocksBase::BlocksFactoryInterface *TrikGeneratorPluginBase::blocksFactoryFor(
		const kitBase::robotModel::RobotModelInterface *model)
{
	Q_UNUSED(model)
	return mBlocksFactory;
}

QList<kitBase::AdditionalPreferences *> TrikGeneratorPluginBase::settingsWidgets()
{
	return {};
}

void TrikGeneratorPluginBase::regenerateExtraFiles(const QFileInfo &newFileInfo)
{
	Q_UNUSED(newFileInfo);
}
