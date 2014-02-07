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

	static void debug(TracerEnum category, QString const &methodName, QString const &message);

private:
	static Tracer *mInstance;
	QVector<bool> mCategories;
	TraceTarget mTarget;
	QString const mLogPath;

	Tracer();
	static Tracer *instance();

	void enableCategoryImpl(TracerEnum const &category);
	void disableCategoryImpl(TracerEnum const &category);
	void enableAllImpl();
	void disableAllImpl();

	void debugImpl(TracerEnum const &category, QString const &methodName, QString const &message);
};

}
