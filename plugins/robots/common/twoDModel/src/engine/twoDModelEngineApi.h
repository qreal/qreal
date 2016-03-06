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

#include "twoDModel/engine/twoDModelEngineInterface.h"

#include <QtCore/QScopedPointer>

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class TwoDModelWidget;
class FakeScene;
}

class TwoDModelEngineApi : public engine::TwoDModelEngineInterface
{
public:
	TwoDModelEngineApi(model::Model &model, view::TwoDModelWidget &view);
	~TwoDModelEngineApi();

	void setNewMotor(int speed, uint degrees
			, const kitBase::robotModel::PortInfo &port, bool breakMode) override;

	int readEncoder(const kitBase::robotModel::PortInfo &port) const override;
	void resetEncoder(const kitBase::robotModel::PortInfo &port) override;

	/// @todo: move this logic into sensors adding here some more low-level logic instead.
	int readTouchSensor(const kitBase::robotModel::PortInfo &port) const override;
	int readSonarSensor(const kitBase::robotModel::PortInfo &port) const override;
	QVector<int> readAccelerometerSensor() const override;
	QVector<int> readGyroscopeSensor() const override;
	int readColorSensor(const kitBase::robotModel::PortInfo &port) const override;
	int readLightSensor(const kitBase::robotModel::PortInfo &port) const override;

	QImage areaUnderSensor(const kitBase::robotModel::PortInfo &port, qreal widthFactor) const override;

	void playSound(int timeInMs) override;

	bool isMarkerDown() const override;
	void markerDown(const QColor &color) override;
	void markerUp() override;

	utils::TimelineInterface &modelTimeline() override;
	engine::TwoDModelDisplayInterface *display() override;
	engine::TwoDModelGuiFacade &guiFacade() const override;

private:
	QPair<QPointF, qreal> countPositionAndDirection(const kitBase::robotModel::PortInfo &port) const;

	int readColorFullSensor(QHash<uint, int> const &countsColor) const;
	int readColorNoneSensor(QHash<uint, int> const &countsColor, int n) const;
	int readSingleColorSensor(uint color, QHash<uint, int> const &countsColor, int n) const;

	uint spoilColor(const uint color) const;
	uint spoilLight(const uint color) const;
	int spoilSonarReading(const int distance) const;

	void enableBackgroundSceneDebugging();

	model::Model &mModel;
	view::TwoDModelWidget &mView;
	QScopedPointer<view::FakeScene> mFakeScene;
	QScopedPointer<engine::TwoDModelGuiFacade> mGuiFacade;
};

}
