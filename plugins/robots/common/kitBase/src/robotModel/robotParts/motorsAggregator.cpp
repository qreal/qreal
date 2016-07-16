/* Copyright 2016 Grigorii Zimin
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

#include "kitBase/robotModel/robotParts/motorsAggregator.h"

#include <QtCore/QPair>

using namespace kitBase::robotModel;
using namespace robotParts;

MotorsAggregator::MotorsAggregator(const DeviceInfo &info, const kitBase::robotModel::PortInfo &port)
	: Device(info, port)
{
}

int kitBase::robotModel::robotParts::MotorsAggregator::power(const QString &port) const
{
	return mSpeeds.value(port, 0);
}

void MotorsAggregator::on(const QList<QPair<QString, int> > &powerForMotors)
{
	for (const QPair<QString, int> &pair : powerForMotors) {
		mSpeeds.insert(pair.first, pair.second);
	}
}

void MotorsAggregator::stop(const QStringList &ports)
{
	Q_UNUSED(ports)
}

void MotorsAggregator::off(const QStringList &ports)
{
	Q_UNUSED(ports)
}
