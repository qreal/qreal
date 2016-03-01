#pragma once

//#include "twoDModel/engine/threeDModelEngineInterface.h"
#include "twoDModel/engine/twoDModelEngineInterface.h"

#include <QtCore/QScopedPointer>

#include "remoteApi/include/socketInConnection.h"
#include "remoteApi/include/v_repConst.h"
//#define pi 3.141593f

extern "C" {
	#include "remoteApi/extApi.h"
//	#include "remoteApi/extApiCustom.h"
}

namespace twoDModel {

namespace model {
class Model;
}
namespace view {
class TwoDModelWidget;
class FakeScene;
}

//class ThreeDModelEngineApi : public engine::ThreeDModelEngineInterface
class ThreeDModelEngineApi : public engine::TwoDModelEngineInterface
{
public:
	ThreeDModelEngineApi(model::Model &model, view::TwoDModelWidget &view);
	~ThreeDModelEngineApi();

	// Block for 3D model

	void setClientID(int newClientID) override;

	//

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

	// Block for 3d code

//	int portNb = 19997;

	int clientID = -1;

	// on time
//	bool connect = false;

	int frontLeftHandle = 0;
	int frontRightHandle = 0;
	int backLeftHandle = 0;
	int backRightHandle = 0;

	int sensorHandle = 0;

	//
};

}
