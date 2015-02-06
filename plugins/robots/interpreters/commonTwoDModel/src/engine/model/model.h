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

	/// Returns a list of existing robot models
	QList<RobotModel *> robotModels();

	/// Returns a reference to a 2D model`s settings storage.
	Settings &settings();

	QDomDocument serialize() const;
	void deserialize(QDomDocument const &xml);

	/// Add new robot model
	/// @param robotModel Model to be added
	/// @param pos Initial positon of robot model
	void addRobotModel(robotModel::TwoDRobotModel &robotModel, const QPointF &pos = QPointF());

	/// Remove robot model
	/// @param robotMode Model to be removed
	void removeRobotModel(twoDModel::robotModel::TwoDRobotModel const &robotModel);

	/// Delete old model and add new model with the same coordinates that old model
	void replaceRobotModel(twoDModel::robotModel::TwoDRobotModel const &oldModel
			, robotModel::TwoDRobotModel &newModel);

signals:
	/// Emitted each time when some user actions lead to world model modifications
	/// @param xml World model description in xml format
	void modelChanged(QDomDocument const &xml);

	/// Emitted after new robot model added
	/// @param robotModel Pointer to robot model which was removed
	void robotAdded(RobotModel *robotModel);

	/// Emitted after robot model removed
	/// @param robotModel Pointer to robot model which was added
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
