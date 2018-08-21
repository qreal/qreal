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

#include <QtCore/QTimer>
#include <QtGui/QQuaternion>

#include <trikControl/gyroSensorInterface.h>
#include <trikKitInterpreterCommon/robotModel/twoD/trikTwoDRobotModel.h>
#include <kitBase/robotModel/robotParts/gyroscopeSensor.h>

namespace kitBase {
namespace robotModel {
namespace robotParts {
class GyroscopeSensor;
}
}
}

class TrikGyroscopeAdapter : public trikControl::GyroSensorInterface
{

	Q_OBJECT

public:
	TrikGyroscopeAdapter(kitBase::robotModel::robotParts::GyroscopeSensor *gyro
		, const QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> &model);

	virtual Status status() const override { return Status::ready; }

public slots:
	virtual QVector<int> read() const override;
	virtual void calibrate(int msec) override;
	virtual bool isCalibrated() const override;
	virtual QVector<int> readRawData() const override;
	virtual void setCalibrationValues(const QVector<int> &) override {}
	virtual QVector<int> getCalibrationValues() override { return QVector<int>(); }

private slots:
	void countTilt(const QVector<int> &oldFormat);

private:
	qreal degreeToMilidegree(qreal value);
	int convertToTrikRuntimeTime(quint64 time) const;

	bool mTimeInited = false;

	kitBase::robotModel::robotParts::GyroscopeSensor *mGyro;
	QSharedPointer<trik::robotModel::twoD::TrikTwoDRobotModel> mModel;
	QVector<int> mResult;
	QQuaternion mQuaternion;
	quint64 mStartTime;
	quint64 mLastUpdateTimeStamp;
};
