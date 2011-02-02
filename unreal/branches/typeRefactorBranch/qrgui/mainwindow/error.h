#pragma once

#include <QtCore/QString>
#include "../kernel/NewType.h"



class Error {
public:
	enum Severity {
		information,
		warning,
		error,
		critical
	};

        Error(QString const &message, Severity const &severity, qReal::NewType  const &position);
	Severity severity() const;
	QString message() const;
        qReal::NewType  position() const;
private:
	QString mMessage;
	Severity mSeverity;
        qReal::NewType  mPosition;
};

