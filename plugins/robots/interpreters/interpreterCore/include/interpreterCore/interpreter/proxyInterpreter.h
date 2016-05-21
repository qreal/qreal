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

#pragma once

#include <kitBase/interpreterInterface.h>

namespace interpreterCore {
namespace interpreter {

/// Proxy between changing different interpreters (for example, control flow and dataflow).
class ProxyInterpreter : public kitBase::InterpreterInterface
{
	Q_OBJECT

public:
	ProxyInterpreter();
	~ProxyInterpreter() override;

	/// Sets \a interpreter as an active interpreter instance.
	void resetInterpreter(InterpreterInterface * const interpreter);

	int timeElapsed() const override;
	qReal::IdList supportedDiagrams() const override;
	virtual bool isRunning() const override;

public slots:
	void connectToRobot() override;
	void interpret() override;
	void stopRobot(qReal::interpretation::StopReason reason = qReal::interpretation::StopReason::userStop) override;

private:
	kitBase::InterpreterInterface *mProxiedInterpreter;  // Does not have ownership.
};

}
}
