#pragma once

#include <QtCore/QSet>

#include "details/defines.h"

namespace qReal {
class ErrorReporterInterface;
}

namespace utils {
class TimelineInterface;
}

namespace twoDModel {
namespace constraints {

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
	ConstraintsChecker(const utils::TimelineInterface &timeline
			, qReal::ErrorReporterInterface &errorReporter);
	~ConstraintsChecker();

	/// Parses the given program on 2D model constraints language and returns the success of this operation.
	/// All parser errors will be reported using errorReporter interface passed to constructor.
	bool parseConstraints(const QString &constraintsXml);

	/// Checks once all constraints active at the call moment. If all constraints are satisfied nothing happens.
	/// If some constraint is violated fail() signal will be emitted. It may also happen that robot has fulfilled
	/// its task one check, then success() signal will be emitted.
	void checkConstraints();

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

	const utils::TimelineInterface &mTimeline;
	qReal::ErrorReporterInterface &mErrorReporter;
	details::StatusReporter mStatus;

	details::Events mEvents;
	details::Variables mVariables;
	details::Objects mObjects;

	QSet<details::Event *> mActiveEvents;
};

}
}
