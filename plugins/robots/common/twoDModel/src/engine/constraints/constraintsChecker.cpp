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

#include "constraintsChecker.h"

#include <qrutils/stringUtils.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>
#include <utils/objectsSet.h>

#include "details/constraintsParser.h"
#include "details/event.h"
#include "twoDModel/engine/model/model.h"
#include "src/engine/items/wallItem.h"
#include "src/engine/items/colorFieldItem.h"
#include "src/engine/items/regions/regionItem.h"

using namespace twoDModel::constraints;

ConstraintsChecker::ConstraintsChecker(qReal::ErrorReporterInterface &errorReporter, model::Model &model)
	: mErrorReporter(errorReporter)
	, mModel(model)
	, mParser(new details::ConstraintsParser(mEvents, mVariables, mObjects, mModel.timeline(), mStatus))
	, mParsedSuccessfully(false)
	, mSuccessTriggered(false)
	, mFailTriggered(false)
	, mEnabled(true)
{
	connect(&mStatus, &details::StatusReporter::success, [this](bool deferred) {
		if (deferred) {
			mDefferedSuccessTriggered = true;
		} else {
			mSuccessTriggered = true;
			emit success();
		}
	});
	connect(&mStatus, &details::StatusReporter::fail, [this]() { mFailTriggered = true; });
	connect(&mStatus, &details::StatusReporter::fail, this, &ConstraintsChecker::fail);
	connect(&mStatus, &details::StatusReporter::checkerError, this, &ConstraintsChecker::checkerError);

	connect(&mModel.timeline(), &model::Timeline::started, this, &ConstraintsChecker::programStarted);
	connect(&mModel.timeline(), &model::Timeline::stopped, this, &ConstraintsChecker::programFinished);
	connect(&mModel.timeline(), &model::Timeline::beforeStop, this, &ConstraintsChecker::checkConstraints);
	connect(&mModel.timeline(), &model::Timeline::tick, this, &ConstraintsChecker::checkConstraints);

	bindToWorldModelObjects();
	bindToRobotObjects();
	mObjects["trace"] = new utils::ObjectsSet<QGraphicsLineItem *>(mModel.worldModel().trace(), this);
}

ConstraintsChecker::~ConstraintsChecker()
{
	qDeleteAll(mEvents);
}

bool ConstraintsChecker::hasConstraints() const
{
	return !mCurrentXml.isNull() && mParsedSuccessfully;
}

bool ConstraintsChecker::parseConstraints(const QDomElement &constraintsXml)
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mActiveEvents.clear();
	mVariables.clear();

	mCurrentXml = constraintsXml;
	mParsedSuccessfully = mParser->parse(constraintsXml);

	for (const QString &error : mParser->errors()) {
		reportParserError(error);
	}

	return mParsedSuccessfully;
}

void ConstraintsChecker::serializeConstraints(QDomElement &parent) const
{
	parent.appendChild(mCurrentXml);
}

void ConstraintsChecker::checkConstraints()
{
	if (!mEnabled) {
		return;
	}

	QListIterator<details::Event *> iterator(mActiveEvents);
	while (iterator.hasNext()) {
		iterator.next()->check();
	}
}

void ConstraintsChecker::setEnabled(bool enabled)
{
	mEnabled = enabled;
}

void ConstraintsChecker::reportParserError(const QString &message)
{
	const QString fullMessage = tr("Error while parsing constraints: %1").arg(message);
	mErrorReporter.addError(fullMessage);
}

void ConstraintsChecker::prepareEvents()
{
	mActiveEvents.clear();
	for (details::Event * const event : mEvents) {
		connect(event, &details::Event::settedUp, this, &ConstraintsChecker::setUpEvent, Qt::UniqueConnection);
		connect(event, &details::Event::dropped, this, &ConstraintsChecker::dropEvent, Qt::UniqueConnection);
		if (event->isAliveInitially()) {
			mActiveEvents << event;
			event->setUp();
		} else {
			event->drop();
		}
	}

	std::sort(mActiveEvents.begin(), mActiveEvents.end(), [](auto e1, auto e2) { return e1->id() > e2->id(); });
}

void ConstraintsChecker::setUpEvent()
{
	if (details::Event * const event = dynamic_cast<details::Event *>(sender())) {
		if (!mActiveEvents.contains(event)) {
			mActiveEvents << event;
		}
	}

	std::sort(mActiveEvents.begin(), mActiveEvents.end(), [](auto e1, auto e2) { return e1->id() > e2->id(); });
}

void ConstraintsChecker::dropEvent()
{
	if (details::Event * const event = dynamic_cast<details::Event *>(sender())) {
		mActiveEvents.removeAll(event);
	}
}

void ConstraintsChecker::bindToWorldModelObjects()
{
	connect(&mModel.worldModel(), &model::WorldModel::wallAdded
			, [this](items::WallItem *item) { bindObject(item->id(), item); });
	connect(&mModel.worldModel(), &model::WorldModel::colorItemAdded
			, [this](items::ColorFieldItem *item) { bindObject(item->id(), item); });
	connect(&mModel.worldModel(), &model::WorldModel::regionItemAdded, [this](items::RegionItem *item) {
		bindObject(item->id(), item);
	});

	connect(&mModel.worldModel(), &model::WorldModel::itemRemoved, [this](QGraphicsItem *item) {
		for (const QString &key : mObjects.keys(dynamic_cast<QObject *>(item))) {
			mObjects.remove(key);
		}
	});
}

void ConstraintsChecker::bindToRobotObjects()
{
	for (model::RobotModel * const robotModel : mModel.robotModels()) {
		bindRobotObject(robotModel);
	}

	connect(&mModel, &model::Model::robotAdded, this, &ConstraintsChecker::bindRobotObject);
	connect(&mModel, &model::Model::robotRemoved, [this](model::RobotModel * const robot) {
		const QStringList keys = mObjects.keys(robot);
		for (const QString &keyToRemove : keys) {
			for (const QString &key : mObjects.keys()) {
				if (key.startsWith(keyToRemove)) {
					mObjects.remove(key);
				}
			}
		}
	});
}

void ConstraintsChecker::bindObject(const QString &id, QObject * const object)
{
	mObjects[id] = object;
	connect(object, &QObject::destroyed, this, [=]() {
		for (const QString &key : mObjects.keys(object)) {
			mObjects.remove(key);
		}
	});
}

void ConstraintsChecker::bindRobotObject(twoDModel::model::RobotModel * const robot)
{
	const QString robotId = firstUnusedRobotId();
	bindObject(robotId, robot);

	// Led, display, marker, all such devices will be also caught here.
	connect(&robot->info().configuration(), &kitBase::robotModel::ConfigurationInterface::deviceConfigured
			, [=](const kitBase::robotModel::robotParts::Device *device)
	{
		bindDeviceObject(robotId, robot, device->port());
	});

	connect(&robot->configuration(), &model::SensorsConfiguration::deviceRemoved
			, [=](const kitBase::robotModel::PortInfo &port, bool isLoading)
	{
		Q_UNUSED(isLoading)
		mObjects.remove(portName(robotId, robot, port));
	});
}

void ConstraintsChecker::bindDeviceObject(const QString &robotId
		, model::RobotModel * const robot, const kitBase::robotModel::PortInfo &port)
{
	mObjects[portName(robotId, robot, port)] = robot->info().configuration().device(port);
}

QString ConstraintsChecker::firstUnusedRobotId() const
{
	int id = 1;
	while (mObjects.contains("robot" + QString::number(id))) {
		++id;
	}

	return "robot" + QString::number(id);
}

QString ConstraintsChecker::portName(const QString &robotId
		, model::RobotModel * const robot, const kitBase::robotModel::PortInfo &port) const
{
	// We wish to know would be there a collision if someone writes "A1" or not.
	int portsWithSuchName = 0;
	for (kitBase::robotModel::PortInfo &otherPort : robot->info().availablePorts()) {
		if (port.name() == otherPort.name()) {
			++portsWithSuchName;
		}
	}

	// Making user write "robot1.DisplayPort_out.ellipses" or "robot1.MarkerPort_out" is non-humanistic.
	// So letting him write "robot1.display.ellipses" or "robot1.marker".
	QRegExp portRegExp("^(\\w+)Port$");
	const QString readablePortName = portRegExp.exactMatch(port.name())
			? utils::StringUtils::lowercaseFirstLetter(portRegExp.cap(1))
			: port.name();

	return portsWithSuchName > 1
			// If collision in name exists then user must specify what port exactly he wishes to process.
			? QString("%1.%2_%3").arg(robotId, readablePortName
					, port.direction() == kitBase::robotModel::input ? "in" : "out")
			: QString("%1.%2").arg(robotId, readablePortName);
}

void ConstraintsChecker::programStarted()
{
	if (!mEnabled) {
		return;
	}

	// Actually not all devices were configured during binding to robot, so iterating through them here...
	for (model::RobotModel * const robot : mModel.robotModels()) {
		const QStringList robotIds = mObjects.keys(robot);
		if (robotIds.isEmpty()) {
			continue;
		}

		const QString robotId = robotIds[0];
		for (kitBase::robotModel::robotParts::Device * const device : robot->info().configuration().devices()) {
			bindDeviceObject(robotId, robot, device->port());
		}
	}

	// In case of null checker we consider that all is ok.
	mSuccessTriggered = mCurrentXml.isNull();
	mDefferedSuccessTriggered = false;
	mFailTriggered = false;
	if (mParsedSuccessfully) {
		prepareEvents();
	}
}

void ConstraintsChecker::programFinished(qReal::interpretation::StopReason reason)
{
	if (!mSuccessTriggered && !mFailTriggered && mEnabled) {
		if (mDefferedSuccessTriggered && reason == qReal::interpretation::StopReason::finised) {
			emit success();
		} else {
			emit fail(tr("Program has finished, but the task is not accomplished."));
		}
	}
}
