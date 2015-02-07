#include "nullErrorReporter.h"

using namespace qReal;

void NullErrorReporter::addInformation(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
}

void NullErrorReporter::addWarning(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
}

void NullErrorReporter::addError(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
	mWereErrors = true;
}

void NullErrorReporter::addCritical(const QString &message, const Id &position)
{
	Q_UNUSED(message)
	Q_UNUSED(position)
	mWereErrors = true;
}

bool NullErrorReporter::wereErrors()
{
	return mWereErrors;
}

void NullErrorReporter::clear()
{
}

void NullErrorReporter::clearErrors()
{
	mWereErrors = false;
}
