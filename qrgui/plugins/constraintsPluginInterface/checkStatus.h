#pragma once

#include <QtCore/QString>
#include "constraintsPluginInterfaceDeclSpec.h"

namespace qReal {

class QRGUI_CONSTRAINTS_PLUGIN_INTERFACE_EXPORT CheckStatus
{
public:
	enum ErrorType {
		warning,
		critical,
		verification
	};

	CheckStatus(bool const &checkStatus, QString const &message, ErrorType const &errorType);
	bool checkStatus() const;
	ErrorType errorType() const;
	QString message() const;

	bool operator==(CheckStatus const &element);

	static QList<CheckStatus> resultCheckStatusList(QList<CheckStatus> const& listCheckStatus);
	static QList<CheckStatus> defaultCheckStatusAsList();

private:
	bool mCheckStatus;
	QString mMessage;
	ErrorType mErrorType;
};

}
