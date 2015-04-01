#include "consoleErrorReporter.h"

using namespace qReal;

void ConsoleErrorReporter::addInformation(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qDebug() << message;
	emit informationAdded(message, position);
}

void ConsoleErrorReporter::addWarning(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qWarning() << message;
	emit warningAdded(message, position);
}

void ConsoleErrorReporter::addError(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qDebug() << QObject::tr("Error:") << message;
	mWereErrors = true;
	emit errorAdded(message, position);
}

void ConsoleErrorReporter::addCritical(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qCritical() << message;
	mWereErrors = true;
	emit criticalAdded(message, position);
}

bool ConsoleErrorReporter::wereErrors()
{
	return mWereErrors;
}

void ConsoleErrorReporter::clear()
{
}

void ConsoleErrorReporter::clearErrors()
{
	mWereErrors = false;
}
