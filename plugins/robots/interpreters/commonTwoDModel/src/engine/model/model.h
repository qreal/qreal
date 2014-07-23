#pragma once

#include "worldModel.h"
#include "robotModel.h"
#include "timeline.h"
#include "settings.h"
#include <commonTwoDModel/robotModel/twoDRobotModel.h>

namespace twoDModel {
namespace model {

/// A main class managing model part of 2D emulator. Creates and maintains different parts
/// such as world map, robot model, timelines and physical engines.
class Model : public QObject
{
	Q_OBJECT

public:
	explicit Model(QObject *parent = 0);

	/// Returns a reference to a world map.
	WorldModel &worldModel();

	/// Returns a reference to a 2D model timeline.
	Timeline &timeline();

	/// Returns a reference to a 2D robot models components.
	QList<RobotModel *> robotModels();

	/// Returns a reference to a 2D model`s settings storage.
	Settings &settings();

	QDomDocument serialize() const;
	void deserialize(QDomDocument const &xml);

	void addRobotModel(robotModel::TwoDRobotModel *robotModel, QPointF const &pos = QPointF()
			, QString const &robotId = "");
	void removeRobotModel(twoDModel::robotModel::TwoDRobotModel const &robotModel);
	void replaceRobotModel(twoDModel::robotModel::TwoDRobotModel const &oldModel
			, twoDModel::robotModel::TwoDRobotModel const &newModel);

signals:
	/// Emitted each time when some user actions lead to world model modifications
	/// @param xml World model description in xml format
	void modelChanged(QDomDocument const &xml);

	void robotAdded(RobotModel *robotModel);
	void robotRemoved(RobotModel *robotModel);

private:
	int findModel(twoDModel::robotModel::TwoDRobotModel const &robotModel);

	Settings mSettings;
	WorldModel mWorldModel;
	Timeline mTimeline;
	QList<RobotModel *> mRobotModels;
};

}
}
