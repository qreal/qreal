#pragma once

#include <qrkernel/ids.h>
#include <qrkernel/definitions.h>
#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

class QRGUI_SYSTEM_FACADE_EXPORT NullErrorReporter : public ErrorReporterInterface
{
public:
	virtual void addInformation(const QString &message, const Id &position = Id::rootId());
	virtual void addWarning(const QString &message, const Id &position = Id::rootId());
	virtual void addError(const QString &message, const Id &position = Id::rootId());
	virtual void addCritical(const QString &message, const Id &position = Id::rootId());

	virtual bool wereErrors();
	virtual void clear();
	virtual void clearErrors();

private:
	bool mWereErrors = false;
};

}
