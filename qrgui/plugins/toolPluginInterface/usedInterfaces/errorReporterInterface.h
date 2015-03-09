#pragma once

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>

namespace qReal {

class ErrorReporterInterface
{
public:
	virtual ~ErrorReporterInterface() {}
	virtual void addInformation(const QString &message, const Id &position = Id::rootId()) = 0;
	virtual void addWarning(const QString &message, const Id &position = Id::rootId()) = 0;
	virtual void addError(const QString &message, const Id &position = Id::rootId()) = 0;
	virtual void addCritical(const QString &message, const Id &position = Id::rootId()) = 0;

	virtual void clear() = 0;
	virtual void clearErrors() = 0;

	/// Are there errors reported by now or not. Counted after latest clearErrors() call
	/// @returns True, when there are messages with severity "Critical" and "Error" registered
	virtual bool wereErrors() = 0;
};

}
