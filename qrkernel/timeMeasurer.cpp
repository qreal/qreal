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

#include "timeMeasurer.h"

#include <QtCore/QDebug>

using namespace qReal;

TimeMeasurer::TimeMeasurer(const QString &methodName)
		: mMethodName{methodName}
{
	mTimer.start();
}

TimeMeasurer::~TimeMeasurer()
{
	qDebug() << QString("TimeMeasurer %1: The operation lasted for %2 mseconds")
			.arg(mMethodName, QString::number(mTimer.elapsed()));
}

void TimeMeasurer::doNothing() const
{
}
