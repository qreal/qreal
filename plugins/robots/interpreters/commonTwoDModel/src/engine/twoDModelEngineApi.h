#pragma once

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class D2ModelWidget;
}

class TwoDModelEngineApi : public engine::TwoDModelEngineInterface
{
public:
	TwoDModelEngineApi(model::Model &model, view::D2ModelWidget &view);

	void setNewMotor(int speed, uint degrees
			, const interpreterBase::robotModel::PortInfo &port, bool breakMode) override;

	int readEncoder(const interpreterBase::robotModel::PortInfo &port) const override;
	void resetEncoder(const interpreterBase::robotModel::PortInfo &port) override;

	/// @todo: move this logic into sensors adding here some more low-level logic instead.
	int readTouchSensor(const interpreterBase::robotModel::PortInfo &port) const override;
	int readSonarSensor(const interpreterBase::robotModel::PortInfo &port) const override;
	int readColorSensor(const interpreterBase::robotModel::PortInfo &port) const override;
	int readLightSensor(const interpreterBase::robotModel::PortInfo &port) const override;

	void playSound(int timeInMs) override;

	void markerDown(const QColor &color) override;
	void markerUp() override;

	utils::TimelineInterface &modelTimeline() override;
	engine::TwoDModelDisplayInterface *display() override;

private:
	QPair<QPointF, qreal> countPositionAndDirection(const interpreterBase::robotModel::PortInfo &port) const;

	QImage printColorSensor(const interpreterBase::robotModel::PortInfo &port) const;
	int readColorFullSensor(QHash<uint, int> const &countsColor) const;
	int readColorNoneSensor(QHash<uint, int> const &countsColor, int n) const;
	int readSingleColorSensor(uint color, QHash<uint, int> const &countsColor, int n) const;

	uint spoilColor(const uint color) const;
	uint spoilLight(const uint color) const;
	int spoilSonarReading(const int distance) const;

	model::Model &mModel;
	view::D2ModelWidget &mView;
};

}
