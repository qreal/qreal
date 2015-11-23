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

#include <QtCore/QSet>
#include <QtXml/QDomElement>

#include <qrutils/interpreter/stopReason.h>

#include "details/defines.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace kitBase {
namespace robotModel {
class PortInfo;
}
}

namespace twoDModel {

namespace model {
class Model;
class RobotModel;
}

namespace constraints {

namespace details {
class ConstraintsParser;
}

/// Checks robot`s behaviour in 2D model world.
/// The constraints are specified in terms of XML-based language. The program on that language
/// first must be parsed using parseConstraints() method and then checkConstraints() method may be called
/// any number of times (each timeline tick for example). If some constraints violated fail() signal will be emitted.
/// If checker thinks that robot`s behaviour is correct and the goal is reached success() signal will be emitted.
/// Also always checkerError() can be emitted if checker program contains runtime errors.
class ConstraintsChecker : public QObject
{
	Q_OBJECT

public:
	ConstraintsChecker(qReal::ErrorReporterInterface &errorReporter, model::Model &model);
	~ConstraintsChecker();

	/// Returns true if constraints checker is active now (constraints list passed into checker is non-empty).
	bool hasConstraints() const;

	/// Parses the given program on 2D model constraints language and returns the success of this operation.
	/// All parser errors will be reported using errorReporter interface passed to constructor.
	bool parseConstraints(const QDomElement &constraintsXml);

	/// Adds constraints xml as a child to a given element.
	void serializeConstraints(QDomElement &parent) const;

	/// Checks once all constraints active at the call moment. If all constraints are satisfied nothing happens.
	/// If some constraint is violated fail() signal will be emitted. It may also happen that robot has fulfilled
	/// its task one check, then success() signal will be emitted.
	void checkConstraints();

	/// Enables or disables checker. Checker will be still disabled if true passed here but constraints list is empty.
	void setEnabled(bool enabled);

signals:
	/// Emitted when program execution meets <success/> trigger. That means that robot successfully accomplished its
	/// task without violation any constraint.
	void success();

	/// Emitted when robot violates some restriction.
	/// @param message The reason why checker thinks robot failed that may be reported to user.
	void fail(const QString &message);

	/// Emitted when checker program written incorrectly with the reason as parameter.
	void checkerError(const QString &message);

private:
	void reportParserError(const QString &message);

	void prepareEvents();

	void setUpEvent();
	void dropEvent();

	void bindToWorldModelObjects();
	void bindToRobotObjects();
	void bindObject(const QString &id, QObject * const object);
	void bindRobotObject(model::RobotModel * const robot);
	void bindDeviceObject(const QString &robotId
			, model::RobotModel * const robot
			, const kitBase::robotModel::PortInfo &port);
	QString firstUnusedRobotId() const;
	QString portName(const QString &robotId
			, model::RobotModel * const robot
			, const kitBase::robotModel::PortInfo &port) const;

	void programStarted();
	void programFinished(qReal::interpretation::StopReason reason);

	qReal::ErrorReporterInterface &mErrorReporter;
	model::Model &mModel;
	details::StatusReporter mStatus;
	QScopedPointer<details::ConstraintsParser> mParser;
	bool mParsedSuccessfully;
	bool mSuccessTriggered;
	bool mDefferedSuccessTriggered;
	bool mFailTriggered;

	details::Events mEvents;
	details::Variables mVariables;
	details::Objects mObjects;

	QList<details::Event *> mActiveEvents;

	QDomElement mCurrentXml;
	bool mEnabled;
};

}
}
