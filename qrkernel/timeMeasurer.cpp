#include "timeMeasurer.h"

#include <QtCore/QDebug>

using namespace qReal;

TimeMeasurer::TimeMeasurer(QString const &methodName)
		: mStartTime(QDateTime::currentDateTime())
		, mMethodName(methodName)
{
}

TimeMeasurer::~TimeMeasurer()
{
	QDateTime currentTime = QDateTime::currentDateTime();
	qDebug() << QString("TimeMeasurer %1: The operation lasted for %2 mseconds")
			.arg(mMethodName, QString::number(static_cast<qlonglong>(mStartTime.msecsTo(currentTime))));
}

void TimeMeasurer::doNothing()
{
}
