#pragma once

#include "twoDModel/engine/threeDModelEngineInterface.h"

#include <QtCore/QScopedPointer>

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class TwoDModelWidget;
class FakeScene;
}

class ThreeDModelEngineApi : public engine::ThreeDModelEngineInterface
{
public:
	ThreeDModelEngineApi(model::Model &model, view::TwoDModelWidget &view);
	~ThreeDModelEngineApi();

	void setNewMotor(int speed, uint degrees
			, const kitBase::robotModel::PortInfo &port, bool breakMode) override;

	int readEncoder(const kitBase::robotModel::PortInfo &port) const override;
	void resetEncoder(const kitBase::robotModel::PortInfo &port) override;

	/// @todo: move this logic into sensors adding here some more low-level logic instead.
	int readTouchSensor(const kitBase::robotModel::PortInfo &port) const override;
	int readSonarSensor(const kitBase::robotModel::PortInfo &port) const override;
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
