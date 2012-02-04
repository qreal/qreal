#include "error.h"

using namespace qReal::gui;

Error::Error(QString const &message, Severity const &severity, qReal::Id const &position)
	: mMessage(message), mTimestamp(QTime::currentTime()), mSeverity(severity), mPosition(position)
{
}

Error::Severity Error::severity() const
{
	return mSeverity;
}

QString Error::severityString() const
{
	if (mSeverity == information) {
		return "information";
	} else if (mSeverity == warning) {
		return "warning";
	} else if (mSeverity == error) {
		return "error";
	} else if (mSeverity == critical) {
		return "critical";
	} else {
		return "";
	}
}

Error::Severity Error::severityByString(QString const &severity)
{
	if (severity.compare("warning") == 0) {
		return warning;
	} else if (severity.compare("error") == 0) {
		return error;
	} else if (severity.compare("critical") == 0) {
		return critical;
	} else /*if (severity.compare("information") == 0)*/ {
		return information;
	}
}

QString Error::message() const
{
	return mMessage;
}

QString Error::timestamp() const
{
	return "[" + mTimestamp.toString() + "]:";
}

qReal::Id Error::position() const
{
	return mPosition;
}
