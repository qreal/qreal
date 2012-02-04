#pragma once

#include <QtCore/QString>
#include <QtCore/QTime>
#include "../../qrkernel/ids.h"

namespace qReal {
namespace gui {

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
	QString severityString() const;
	static Severity severityByString(QString const &severity);
	QString message() const;
	QString timestamp() const;
	qReal::Id  position() const;

private:
	QString mMessage;
	QTime mTimestamp;
	Severity mSeverity;
	qReal::Id  mPosition;
};

}
}
