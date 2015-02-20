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
{
	connect(&mStatus, &details::StatusReporter::success, [this]() { mSuccessTriggered = true; });
	connect(&mStatus, &details::StatusReporter::success, this, &ConstraintsChecker::success);
	connect(&mStatus, &details::StatusReporter::success, [this]() { mFailTriggered = true; });
	connect(&mStatus, &details::StatusReporter::fail, this, &ConstraintsChecker::fail);
	connect(&mStatus, &details::StatusReporter::checkerError, this, &ConstraintsChecker::checkerError);

	connect(&mModel.timeline(), &model::Timeline::started, this, &ConstraintsChecker::programStarted);
	connect(&mModel.timeline(), &model::Timeline::started, this, &ConstraintsChecker::programFinished);
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
	mObjects.clear();

	mCurrentXml = constraintsXml;
	details::ConstraintsParser parser(mEvents, mVariables, mObjects, mModel.timeline(), mStatus);
	const bool result = parser.parse(constraintsXml);

	for (const QString &error : parser.errors()) {
		reportParserError(error);
	}

	if (result) {
		prepareEvents();
	}

	return result;
}

void ConstraintsChecker::serializeConstraints(QDomElement &parent) const
{
	parent.appendChild(mCurrentXml);
}

void ConstraintsChecker::checkConstraints()
{
	for (details::Event * const event : mActiveEvents) {
		event->check();
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
		connect(event, &details::Event::settedUp, this, &ConstraintsChecker::setUpEvent);
		connect(event, &details::Event::dropped, this, &ConstraintsChecker::dropEvent);
		if (event->isAlive()) {
			mActiveEvents << event;
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
	connect(&mModel, &model::Model::robotAdded, [this](model::RobotModel * const robot) {
		const QString robotId = firstUnusedRobotId();
		mObjects[robotId] = robot;

		connect(&robot->configuration(), &model::SensorsConfiguration::deviceAdded
				, [=](const interpreterBase::robotModel::PortInfo &port, bool isLoading) {
			Q_UNUSED(isLoading)
			mObjects[robotId + "." + port.name()] = robot->info().configuration().device(port);
		});

		/// @todo: add led, display and other devices here.
		connect(&robot->configuration(), &model::SensorsConfiguration::deviceRemoved
				, [=](const interpreterBase::robotModel::PortInfo &port, bool isLoading) {
			Q_UNUSED(isLoading)
			mObjects.remove(robotId + "." + port.name());
		});
	});


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

QString ConstraintsChecker::firstUnusedRobotId() const
{
	int id = 1;
	while (mObjects.contains("robot" + QString::number(id))) {
		++id;
	}

	return "robot" + QString::number(id);
}

void ConstraintsChecker::programStarted()
{
	// In case of null checker we consider that all is ok.
	mSuccessTriggered = mCurrentXml.isNull();
	mFailTriggered = false;
}

void ConstraintsChecker::programFinished()
{
	if (!mSuccessTriggered && !mFailTriggered) {
		fail(tr("Program has finished, but the task is not accomplished."));
	}
}
