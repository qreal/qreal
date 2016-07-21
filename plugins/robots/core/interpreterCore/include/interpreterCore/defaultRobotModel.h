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

#pragma once

#include "kitBase/robotModel/commonRobotModel.h"

namespace interpreterCore {

/// Default robot model that is used when no kit plugin with models is loaded.
/// @todo: Replace it with nullKitInterpreter.
class DefaultRobotModel : public kitBase::robotModel::CommonRobotModel
{
	Q_OBJECT

public:
	DefaultRobotModel();
	QString name() const override;
	QString friendlyName() const override;
	bool needsConnection() const override;
	int priority() const override;
};

}
