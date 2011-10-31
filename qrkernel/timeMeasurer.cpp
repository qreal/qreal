#include "timeMeasurer.h"

#include <QtCore/QDebug>

using namespace qReal;

TimeMeasurer::TimeMeasurer(const QString &methodName)
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

void TimeMeasurer::doNothing() // This method is used to avoid unused variables problem,
							// because sometimes the functionality of the object of this class
							// is limited to the functionality in destructor
{
}
