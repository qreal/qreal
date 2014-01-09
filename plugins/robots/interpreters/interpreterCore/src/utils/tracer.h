#pragma once

#include <QtCore/QVector>
#include <QtCore/QString>

namespace interpreterCore {
namespace utils {

namespace enums {
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
	static void enableCategory(enums::TracerEnum category);
	static void disableCategory(enums::TracerEnum category);
	static void enableAll();
	static void disableAll();
	static void setTarget(enums::TraceTarget target);

	static void debug(
			enums::TracerEnum category
			, QString const &methodName
			, QString const &message
			);

private:
	static Tracer *mInstance;
	QVector<bool> mCategories;
	enums::TraceTarget mTarget;
	QString const mLogPath;

	Tracer();
	static Tracer *instance();

	void enableCategoryImpl(enums::TracerEnum const &category);
	void disableCategoryImpl(enums::TracerEnum const &category);
	void enableAllImpl();
	void disableAllImpl();

	void debugImpl(
			enums::TracerEnum const &category
			, QString const &methodName
			, QString const &message
			);
};

}
}
