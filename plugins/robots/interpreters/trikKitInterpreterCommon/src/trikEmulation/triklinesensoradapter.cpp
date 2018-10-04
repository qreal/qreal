/* Copyright 2016-2017 CyberTech Labs Ltd.
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

#include <trikKitInterpreterCommon/trikEmulation/triklinesensoradapter.h>

TrikLineSensorAdapter::TrikLineSensorAdapter(trik::robotModel::parts::TrikLineSensor *sensor)
	: mLineSensor(sensor)
{
}

trikControl::DeviceInterface::Status TrikLineSensorAdapter::status() const
{
	return Status::ready;
}

void TrikLineSensorAdapter::init(bool showOnDisplay)
{
	Q_UNUSED(showOnDisplay);
	mLineSensor->init();
}

void TrikLineSensorAdapter::detect()
{
	mLineSensor->detectLine(); // hopefully the same
}

QVector<int> TrikLineSensorAdapter::read()
{
	QMetaObject::invokeMethod(mLineSensor, "read");
	return mLineSensor->lastData(); // hopefully the same format
}

void TrikLineSensorAdapter::stop()
{
}

QVector<int> TrikLineSensorAdapter::getDetectParameters() const
{
	return mLineSensor->lastData(); // i guess?
}
