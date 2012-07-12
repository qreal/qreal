#pragma once

#include <QtCore/QDateTime>

#include "kernelDeclSpec.h"

namespace qReal {

/// Measures time interval between its creation and deletion, writes results
/// to qDebug. Used for profiling.
class QRKERNEL_EXPORT TimeMeasurer
{
public:
	/// Constructor.
	/// @param methodName A name of a method or part of program to be measured.
	explicit TimeMeasurer(QString const &methodName);

	/// Destructor. Calling it indicates end of measured interval.
	~TimeMeasurer();

	/// This method is used to avoid unused variables problem,
	/// because sometimes the functionality of the object of this class
	/// is limited to the functionality in destructor.
	void doNothing();

private:
	/// Time of start of measured interval.
	QDateTime mStartTime;

	/// Name of a method to measure.
	QString mMethodName;
};
}
