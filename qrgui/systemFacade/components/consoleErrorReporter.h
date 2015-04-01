#pragma once

#include <qrgui/plugins/toolPluginInterface/usedInterfaces/errorReporterInterface.h>

#include "qrgui/systemFacade/systemFacadeDeclSpec.h"

namespace qReal {

/// A mock up class writing errors into the console.
class QRGUI_SYSTEM_FACADE_EXPORT ConsoleErrorReporter : public QObject, public ErrorReporterInterface
{
	Q_OBJECT

public:
	virtual void addInformation(const QString &message, const Id &position = Id::rootId());
	virtual void addWarning(const QString &message, const Id &position = Id::rootId());
	virtual void addError(const QString &message, const Id &position = Id::rootId());
	virtual void addCritical(const QString &message, const Id &position = Id::rootId());

	virtual bool wereErrors();
	virtual void clear();
	virtual void clearErrors();

signals:
	/// Emitted when new message with level 'Info' added to error reporter.
	void informationAdded(const QString &message, const Id &position);

	/// Emitted when new message with level 'Warning' added to error reporter.
	void warningAdded(const QString &message, const Id &position);

	/// Emitted when new message with level 'Error' added to error reporter.
	void errorAdded(const QString &message, const Id &position);

	/// Emitted when new message with level 'Critical' added to error reporter.
	void criticalAdded(const QString &message, const Id &position);

private:
	bool mWereErrors = false;
};

}
