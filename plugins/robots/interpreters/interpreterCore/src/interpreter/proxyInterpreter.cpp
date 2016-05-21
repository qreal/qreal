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

#include "interpreterCore/interpreter/proxyInterpreter.h"

using namespace interpreterCore::interpreter;

ProxyInterpreter::ProxyInterpreter()
	: mProxiedInterpreter(nullptr)
{
}

ProxyInterpreter::~ProxyInterpreter()
{
}

void ProxyInterpreter::resetInterpreter(InterpreterInterface * const interpreter)
{
	if (mProxiedInterpreter) {
		stopRobot(qReal::interpretation::StopReason::userStop);
		disconnect(mProxiedInterpreter);
	}

	mProxiedInterpreter = interpreter;
	connect(mProxiedInterpreter, &InterpreterInterface::started, this, &ProxyInterpreter::started);
	connect(mProxiedInterpreter, &InterpreterInterface::stopped, this, &ProxyInterpreter::stopped);
	connect(mProxiedInterpreter, &InterpreterInterface::connected, this, &ProxyInterpreter::connected);
}

void ProxyInterpreter::connectToRobot()
{
	if (mProxiedInterpreter) {
		mProxiedInterpreter->connectToRobot();
	}
}

void ProxyInterpreter::interpret()
{
	if (mProxiedInterpreter) {
		mProxiedInterpreter->interpret();
	}
}

void ProxyInterpreter::stopRobot(qReal::interpretation::StopReason reason)
{
	if (mProxiedInterpreter) {
		mProxiedInterpreter->stopRobot(reason);
	}
}

int ProxyInterpreter::timeElapsed() const
{
	return mProxiedInterpreter ? mProxiedInterpreter->timeElapsed() : 0;
}

qReal::IdList ProxyInterpreter::supportedDiagrams() const
{
	return mProxiedInterpreter ? mProxiedInterpreter->supportedDiagrams() : qReal::IdList();
}

bool ProxyInterpreter::isRunning() const
{
	return mProxiedInterpreter ? mProxiedInterpreter->isRunning() : false;
}
