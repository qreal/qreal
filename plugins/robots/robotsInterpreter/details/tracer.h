#pragma once

#include <QtCore/QVector>
#include <QtCore/QString>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

namespace tracer {
enum TracerEnum {
	d2Model
	, initialization
	, blocks
	, robotCommunication
	, autoupdatedSensorValues
	, sensors
};

enum TraceTarget {
	debug = 0
	, logFile
};
}

class Tracer
{
public:

	static void enableCategory(tracer::TracerEnum category);
	static void disableCategory(tracer::TracerEnum category);
	static void enableAll();
	static void disableAll();
	static void setTarget(tracer::TraceTarget target);

	static void debug(tracer::TracerEnum category, QString const &methodName, QString const &message);

private:
	static Tracer *mInstance;
	QVector<bool> mCategories;
	tracer::TraceTarget mTarget;
	QString const mLogPath;

	Tracer();
	static Tracer *instance();

	void enableCategoryImpl(tracer::TracerEnum const &category);
	void disableCategoryImpl(tracer::TracerEnum const &category);
	void enableAllImpl();
	void disableAllImpl();

	void debugImpl(tracer::TracerEnum const &category, QString const &methodName, QString const &message);
};

}
}
}
}
