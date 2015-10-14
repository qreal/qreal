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

#include "worldModel.h"
#include "robotModel.h"
#include "timeline.h"
#include "settings.h"
#include <twoDModel/robotModel/twoDRobotModel.h>

#include "twoDModel/twoDModelDeclSpec.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace kitBase {
class InterpreterControlInterface;
}

namespace twoDModel {

namespace constraints {
class ConstraintsChecker;
}

namespace model {

/// A main class managing model part of 2D emulator. Creates and maintains different parts
/// such as world map, robot model, timelines and physical engines.
class TWO_D_MODEL_EXPORT Model : public QObject
{
	Q_OBJECT

public:
	explicit Model(QObject *parent = 0);
	~Model();

	void init(qReal::ErrorReporterInterface &errorReporter
			, kitBase::InterpreterControlInterface &interpreterControl);

	/// Returns a reference to a world map.
	WorldModel &worldModel();

	/// Returns a reference to a 2D model timeline.
	Timeline &timeline();

	/// Returns a list of existing robot models
	QList<RobotModel *> robotModels() const;

	/// Returns a reference to a 2D model`s settings storage.
	Settings &settings();

	/// Returns a pointer to an object that reports system errors.
	qReal::ErrorReporterInterface *errorReporter();

	QDomDocument serialize() const;
	void deserialize(const QDomDocument &xml);

	/// Add new robot model
	/// @param robotModel Model to be added
	/// @param pos Initial positon of robot model
	void addRobotModel(robotModel::TwoDRobotModel &robotModel, const QPointF &pos = QPointF());

	/// Remove robot model
	/// @param robotMode Model to be removed
	void removeRobotModel(const twoDModel::robotModel::TwoDRobotModel &robotModel);

	/// Delete old model and add new model with the same coordinates that old model
	void replaceRobotModel(const twoDModel::robotModel::TwoDRobotModel &oldModel
			, robotModel::TwoDRobotModel &newModel);

	/// Returns true if constraints checker is active (constraints list in the model is non-empty).
	bool hasConstraints() const;

	/// Activates or deactivates constraints checker.
	void setConstraintsEnabled(bool enabled);

signals:
	/// Emitted each time when some user actions lead to world model modifications
	/// @param xml World model description in xml format
	void modelChanged(const QDomDocument &xml);

	/// Emitted after new robot model added
	/// @param robotModel Pointer to robot model which was removed
	void robotAdded(RobotModel *robotModel);

	/// Emitted after robot model removed
	/// @param robotModel Pointer to robot model which was added
	void robotRemoved(RobotModel *robotModel);

private:
	int findModel(const twoDModel::robotModel::TwoDRobotModel &robotModel);

	Settings mSettings;
	WorldModel mWorldModel;
	Timeline mTimeline;
	QScopedPointer<constraints::ConstraintsChecker> mChecker;
	QList<RobotModel *> mRobotModels;
	qReal::ErrorReporterInterface *mErrorReporter;  // Doesn`t take ownership.
};

}
}
