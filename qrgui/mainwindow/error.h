#pragma once

#include <QtCore/QString>
#include "../kernel/ids.h"



class Error {
public:
	enum Severity {
		information,
		warning,
		error,
		critical
	};

	Error(QString const &message, Severity const &severity, qReal::Id  const &position);
	Severity severity() const;
	QString message() const;
	qReal::Id  position() const;
private:
	QString mMessage;
	Severity mSeverity;
	qReal::Id  mPosition;
};

