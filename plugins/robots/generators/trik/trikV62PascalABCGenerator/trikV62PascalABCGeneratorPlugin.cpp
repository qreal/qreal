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

#include "trikV62PascalABCGeneratorPlugin.h"

#include <trikKit/blocks/trikV62BlocksFactory.h>
#include <trikGeneratorBase/robotModel/trikV62GeneratorRobotModel.h>

using namespace trik;
using namespace trik::pascalABC;
using namespace kitBase::robotModel;

TrikV62PascalABCGeneratorPlugin::TrikV62PascalABCGeneratorPlugin()
	: TrikPascalABCGeneratorPluginBase(
				new robotModel::TrikV62GeneratorRobotModel(
						"trikV62Kit"
						, "trikKitRobot"
						, "TrikV62PascalABCGeneratorRobotModel"
						, tr("Generation (PascalABC)")
						, 8 /* After Javascript generator */)
				, new blocks::TrikV62BlocksFactory()
				, {":/trikPascalABC/templates"})
{
}

QString TrikV62PascalABCGeneratorPlugin::kitId() const
{
	return "trikV62Kit";
}
