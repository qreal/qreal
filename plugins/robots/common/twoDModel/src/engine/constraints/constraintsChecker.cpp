#include "constraintsChecker.h"

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "details/constraintsParser.h"
#include "details/event.h"

using namespace twoDModel::constraints;

ConstraintsChecker::ConstraintsChecker(const utils::TimelineInterface &timeline
		, qReal::ErrorReporterInterface &errorReporter)
	: mTimeline(timeline)
	, mErrorReporter(errorReporter)
{
	connect(&mStatus, &details::StatusReporter::success, this, &ConstraintsChecker::success);
	connect(&mStatus, &details::StatusReporter::fail, this, &ConstraintsChecker::fail);
	connect(&mStatus, &details::StatusReporter::checkerError, this, &ConstraintsChecker::checkerError);
}

ConstraintsChecker::~ConstraintsChecker()
{
	qDeleteAll(mEvents);
}

bool ConstraintsChecker::parseConstraints(const QString &constraintsXml)
{
	qDeleteAll(mEvents);
	mEvents.clear();
	mVariables.clear();
	mObjects.clear();

	details::ConstraintsParser parser(mEvents, mVariables, mObjects, mTimeline, mStatus);
	const bool result = parser.parse(constraintsXml);

	for (const QString &error : parser.errors()) {
		reportParserError(error);
	}

	if (result) {
		prepareEvents();
	}

	return result;
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
