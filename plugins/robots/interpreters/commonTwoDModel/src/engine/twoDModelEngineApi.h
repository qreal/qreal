#pragma once

#include "commonTwoDModel/engine/twoDModelEngineInterface.h"

#include <qrutils/mathUtils/gaussNoise.h>

namespace twoDModel {

class Model;
class D2ModelWidget;

class TwoDModelEngineApi : public engine::TwoDModelEngineInterface
{
public:
	TwoDModelEngineApi(Model &model, D2ModelWidget &view);

	void setNewMotor(int speed, uint degrees
			, interpreterBase::robotModel::PortInfo const &port, bool breakMode) override;

	int readEncoder(interpreterBase::robotModel::PortInfo const &port) const override;
	void resetEncoder(interpreterBase::robotModel::PortInfo const &port) override;

	/// @todo: move this logic into sensors adding here some more low-level logic instead.
	int readTouchSensor(interpreterBase::robotModel::PortInfo const &port) const override;
	int readSonarSensor(interpreterBase::robotModel::PortInfo const &port) const override;
	int readColorSensor(interpreterBase::robotModel::PortInfo const &port) const override;
	int readLightSensor(interpreterBase::robotModel::PortInfo const &port) const override;

	void playSound(int timeInMs) override;

	utils::TimelineInterface &modelTimeline() override;
	engine::TwoDModelDisplayInterface *display() override;

private:
	QPair<QPointF, qreal> countPositionAndDirection(interpreterBase::robotModel::PortInfo const &port) const;

	QImage printColorSensor(interpreterBase::robotModel::PortInfo const &port) const;
	int readColorFullSensor(QHash<uint, int> const &countsColor) const;
	int readColorNoneSensor(QHash<uint, int> const &countsColor, int n) const;
	int readSingleColorSensor(uint color, QHash<uint, int> const &countsColor, int n) const;

	uint spoilColor(uint const color) const;
	uint spoilLight(uint const color) const;
	int varySpeed(int const speed) const;
	int spoilSonarReading(int const distance) const;

	Model &mModel;
	D2ModelWidget &mView;

	mathUtils::GaussNoise mNoiseGenerator;
};

}
