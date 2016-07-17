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

#include <kitBase/robotModel/portInfo.h>
#include <kitBase/robotModel/deviceInfo.h>

#include "initTerminateCodeGenerator.h"
#include "generatorBase/simpleGenerators/binding.h"

namespace generatorBase {
namespace parts {

/// Inspects sensors usage and generates code for their initialization and termination.
class Sensors : public InitTerminateCodeGenerator
{
public:
	Sensors(const QStringList &pathsToTemplates, const simple::Binding::ConverterInterface *inputPortConverter);
	virtual ~Sensors();

	virtual void reinit(const QMap<kitBase::robotModel::PortInfo
			, kitBase::robotModel::DeviceInfo> &devices);

	virtual QString initCode();
	virtual QString terminateCode();
	virtual QString isrHooksCode();

protected:
	void reinitPort(const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device);

	virtual QString code(const QString &directory
			, const kitBase::robotModel::PortInfo &port
			, const kitBase::robotModel::DeviceInfo &device);

private:
	const simple::Binding::ConverterInterface *mInputPortConverter;  // Takes ownership
	QStringList mInitCode;
	QStringList mTerminateCode;
	QStringList mIsrHooksCode;
};

}
}
