#include "constraintsChecker.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "details/constraintsParser.h"
#include "details/event.h"
#include "src/engine/model/model.h"
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
{
	connect(&mStatus, &details::StatusReporter::success, [this]() { mSuccessTriggered = true; });
	connect(&mStatus, &details::StatusReporter::success, this, &ConstraintsChecker::success);
	connect(&mStatus, &details::StatusReporter::fail, [this]() { mFailTriggered = true; });
	connect(&mStatus, &details::StatusReporter::fail, this, &ConstraintsChecker::fail);
	connect(&mStatus, &details::StatusReporter::checkerError, this, &ConstraintsChecker::checkerError);

	connect(&mModel.timeline(), &model::Timeline::started, this, &ConstraintsChecker::programStarted);
	connect(&mModel.timeline(), &model::Timeline::stopped, this, &ConstraintsChecker::programFinished);
	connect(&mModel.timeline(), &model::Timeline::tick, this, &ConstraintsChecker::checkConstraints);

	bindToWorldModelObjects();
	bindToRobotObjects();
}

ConstraintsChecker::~ConstraintsChecker()
{
	qDeleteAll(mEvents);
}

bool ConstraintsChecker::parseConstraints(const QDomElement &constraintsXml)
{
	qDeleteAll(mEvents);
	mEvents.clear();
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
	QSetIterator<details::Event *> iterator(mActiveEvents);
	while (iterator.hasNext()) {
		iterator.next()->check();
	}
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
}

void ConstraintsChecker::setUpEvent()
{
	if (details::Event * const event = dynamic_cast<details::Event *>(sender())) {
		mActiveEvents << event;
	}
}

void ConstraintsChecker::dropEvent()
{
	if (details::Event * const event = dynamic_cast<details::Event *>(sender())) {
		mActiveEvents.remove(event);
	}
}

void ConstraintsChecker::bindToWorldModelObjects()
{
	connect(&mModel.worldModel(), &model::WorldModel::wallAdded
			, [this](items::WallItem *item) { mObjects[item->id()] = item; });
	connect(&mModel.worldModel(), &model::WorldModel::colorItemAdded
			, [this](items::ColorFieldItem *item) { mObjects[item->id()] = item; });
	connect(&mModel.worldModel(), &model::WorldModel::otherItemAdded, [this](QGraphicsItem *graphicsItem) {
		if (graphicsUtils::AbstractItem *item = dynamic_cast<graphicsUtils::AbstractItem *>(graphicsItem)) {
			mObjects[item->id()] = item;
		}

		if (items::RegionItem *item = dynamic_cast<items::RegionItem *>(graphicsItem)) {
			mObjects[item->id()] = item;
		}
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

void ConstraintsChecker::bindRobotObject(twoDModel::model::RobotModel * const robot)
{
	const QString robotId = firstUnusedRobotId();
	mObjects[robotId] = robot;

	connect(&robot->configuration(), &model::SensorsConfiguration::deviceAdded
			, [=](const kitBase::robotModel::PortInfo &port, bool isLoading) {
		Q_UNUSED(isLoading)
		bindDeviceObject(robotId, robot, port);
	});

	/// @todo: add led, display and other devices here.
	connect(&robot->configuration(), &model::SensorsConfiguration::deviceRemoved
			, [=](const kitBase::robotModel::PortInfo &port, bool isLoading) {
		Q_UNUSED(isLoading)
		mObjects.remove(portName(robotId, port));
	});
}

void ConstraintsChecker::bindDeviceObject(const QString &robotId
		, model::RobotModel * const robot, const kitBase::robotModel::PortInfo &port)
{
	mObjects[portName(robotId, port)] = robot->info().configuration().device(port);
}

QString ConstraintsChecker::firstUnusedRobotId() const
{
	int id = 1;
	while (mObjects.contains("robot" + QString::number(id))) {
		++id;
	}

	return "robot" + QString::number(id);
}

QString ConstraintsChecker::portName(const QString &robotId, const kitBase::robotModel::PortInfo &port) const
{
	return QString("%1.%2_%3").arg(robotId, port.name()
			, port.direction() == kitBase::robotModel::input ? "in" : "out");
}

void ConstraintsChecker::programStarted()
{
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
	mFailTriggered = false;
	if (mParsedSuccessfully) {
		prepareEvents();
	}
}

void ConstraintsChecker::programFinished()
{
	if (!mSuccessTriggered && !mFailTriggered) {
		fail(tr("Program has finished, but the task is not accomplished."));
	}
}
