#pragma once

#include "worldModel.h"
#include "robotModel.h"
#include "timeline.h"
#include "settings.h"

namespace twoDModel {
namespace model {

/// A main class managing model part of 2D emulator. Creates and maintains different parts
/// such as world map, robot model, timelines and physical engines.
class Model : public QObject
{
	Q_OBJECT

public:
	/// @param configurer - allows to configure various model parameters specific to a kit. Takes ownership.
	explicit Model(interpreterBase::robotModel::RobotModelInterface &robotModel, QObject *parent = 0);
	~Model();

	/// Returns a reference to a world map.
	WorldModel &worldModel();

	/// Returns a reference to a 2D model timeline.
	Timeline &timeline();

	/// Returns a reference to a 2D robot model component.
	RobotModel &robotModel();

	/// Returns a reference to a 2D model`s settings storage.
	Settings &settings();

	QDomDocument serialize() const;
	void deserialize(QDomDocument const &xml);

signals:
	/// Emitted each time when some user actions lead to world model modifications
	/// @param xml World model description in xml format
	void modelChanged(QDomDocument const &xml);

private:
	Settings mSettings;
	WorldModel mWorldModel;
	Timeline mTimeline;
	RobotModel mRobotModel;

	physics::PhysicsEngineBase *mPhysicsEngine;
};

}
}
