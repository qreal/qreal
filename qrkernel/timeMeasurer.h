#pragma once

#include <QtCore/QDateTime>

#include "kernelDeclSpec.h"

namespace qReal {

class QRKERNEL_EXPORT TimeMeasurer {

public:
	explicit TimeMeasurer(QString const &methodName);
	~TimeMeasurer();
	void doNothing();

private:
	QDateTime mStartTime;
	QString mMethodName;
};
}
