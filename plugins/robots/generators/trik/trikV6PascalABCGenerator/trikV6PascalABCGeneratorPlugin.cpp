/* Copyright 2016 Ivan Limar
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

#include "trikV6PascalABCGeneratorPlugin.h"

#include <trikKit/blocks/trikV6BlocksFactory.h>
#include <trikGeneratorBase/robotModel/trikV6GeneratorRobotModel.h>

using namespace trik;
using namespace trik::pascalABC;
using namespace kitBase::robotModel;

TrikV6PascalABCGeneratorPlugin::TrikV6PascalABCGeneratorPlugin()
	: TrikPascalABCGeneratorPluginBase(
				new robotModel::TrikV6GeneratorRobotModel(
						"trikKit"
						, "trikKitRobot"
						, "TrikV6PascalABCGeneratorRobotModel"
						, tr("Generation (PascalABC)")
						, 8 /* After Javascript generator */)
				, new blocks::TrikV6BlocksFactory()
				, {":/trikPascalABC/templates"})
{
}

QString TrikV6PascalABCGeneratorPlugin::kitId() const
{
	return "trikKit";
}
