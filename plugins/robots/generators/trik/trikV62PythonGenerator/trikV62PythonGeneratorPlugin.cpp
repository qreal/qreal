/* Copyright 2016 CyberTech Labs Ltd.
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

#include "trikV62PythonGeneratorPlugin.h"

#include <trikKit/blocks/trikV62BlocksFactory.h>
#include <trikGeneratorBase/robotModel/trikV62GeneratorRobotModel.h>

using namespace trik;
using namespace trik::python;
using namespace kitBase::robotModel;

TrikV62PythonGeneratorPlugin::TrikV62PythonGeneratorPlugin()
	: TrikPythonGeneratorPluginBase(new robotModel::TrikV62GeneratorRobotModel(
					"trikV62Kit"
					, "trikKitRobot"
					, "TrikV62PythonGeneratorRobotModel"
					, tr("Generation (Python)")
					, 7 /* After PascalABC model */)
			, new blocks::TrikV62BlocksFactory()
			, {":/trikPython/templates"})
	, mModel(static_cast<robotModel::TrikV62GeneratorRobotModel *>(robotModels().first()))
{
}

QString TrikV62PythonGeneratorPlugin::kitId() const
{
	return "trikV62Kit";
}

void TrikV62PythonGeneratorPlugin::init(const kitBase::KitPluginConfigurator &configurator)
{
	TrikPythonGeneratorPluginBase::init(configurator);
	addShellDevice(*mModel);
}
