#include "consoleErrorReporter.h"

using namespace qReal;

void ConsoleErrorReporter::addInformation(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qDebug() << message;
}

void ConsoleErrorReporter::addWarning(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qWarning() << message;
}

void ConsoleErrorReporter::addError(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qDebug() << QObject::tr("Error:") << message;
	mWereErrors = true;
}

void ConsoleErrorReporter::addCritical(const QString &message, const Id &position)
{
	Q_UNUSED(position)
	qCritical() << message;
	mWereErrors = true;
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
