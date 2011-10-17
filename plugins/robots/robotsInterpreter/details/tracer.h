#pragma once

#include <QtCore/QVector>

namespace qReal {
namespace interpreters {
namespace robots {
namespace details {

namespace tracer {
enum TracerEnum {
	d2Model
	, initialization
	, blocks
	, sensors
};
}

class Tracer
{
public:

	static void enableCategory(tracer::TracerEnum const &category);
	static void disableCategory(tracer::TracerEnum const &category);
	static void enableAll();
	static void disableAll();

	static void debug(tracer::TracerEnum const &category, QString const &methodName, QString const &message);

private:
	static Tracer *mInstance;
	QVector<bool> mCategories;

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
