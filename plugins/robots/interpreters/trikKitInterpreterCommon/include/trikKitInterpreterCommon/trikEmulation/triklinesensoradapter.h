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

#pragma once

#include <trikControl/lineSensorInterface.h>
#include <trikKit/robotModel/parts/trikLineSensor.h> // replace with forward declaration

class TrikLineSensorAdapter : public trikControl::LineSensorInterface
{

Q_OBJECT

public:
	TrikLineSensorAdapter(trik::robotModel::parts::TrikLineSensor *sensor);
	virtual Status status() const override;

public slots:
	virtual void init(bool showOnDisplay) override;
	virtual void detect() override;
	virtual QVector<int> read() override;
	virtual void stop() override;
	virtual QVector<int> getDetectParameters() const override;

private:

	trik::robotModel::parts::TrikLineSensor *mLineSensor;
};
