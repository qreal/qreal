#pragma once

#include <QtCore/QVector>
#include <QtCore/QString>

#include "utilsDeclSpec.h"

namespace utils {

class ROBOTS_UTILS_EXPORT Tracer
{
public:
	enum TracerEnum {
		d2Model
		, initialization
		, blocks
		, robotCommunication
		, autoupdatedSensorValues
		, sensors
	};

	enum TraceTarget {
		debugOutput = 0
		, logFile
	};

	static void enableCategory(TracerEnum category);
	static void disableCategory(TracerEnum category);
	static void enableAll();
	static void disableAll();
	static void setTarget(TraceTarget target);

	static void debug(TracerEnum category, const QString &methodName, const QString &message);

private:
	static Tracer *mInstance;
	QVector<bool> mCategories;
	TraceTarget mTarget;
	const QString mLogPath;

	Tracer();
	static Tracer *instance();

	void enableCategoryImpl(const TracerEnum &category);
	void disableCategoryImpl(const TracerEnum &category);
	void enableAllImpl();
	void disableAllImpl();

	void debugImpl(const TracerEnum &category, const QString &methodName, const QString &message);
};

}
