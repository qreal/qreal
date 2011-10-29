#include "tracer.h"

#include <QtCore/QDebug>

using namespace qReal::interpreters::robots::details;

Tracer* Tracer::mInstance = NULL;

void Tracer::enableCategory(tracer::TracerEnum const &category)
{
	instance()->enableCategoryImpl(category);
}

void Tracer::disableCategory(tracer::TracerEnum const &category)
{
	instance()->disableCategoryImpl(category);
}

void Tracer::enableAll()
{
	instance()->enableAllImpl();
}

void Tracer::disableAll()
{
	instance()->disableAllImpl();
}

void Tracer::debug(tracer::TracerEnum const &category, QString const &methodName, QString const &message)
{
	instance()->debugImpl(category, methodName, message);
}

Tracer::Tracer()
{
	mCategories.resize(tracer::sensors + 1);
}

Tracer *Tracer::instance()
{
	if (!mInstance)
		mInstance = new Tracer;
	return mInstance;
}

void Tracer::enableCategoryImpl(tracer::TracerEnum const &category)
{
	mCategories[category] = true;
}

void Tracer::disableCategoryImpl(tracer::TracerEnum const &category)
{
	mCategories[category] = false;
}

void Tracer::enableAllImpl()
{
	mCategories.fill(true);
}

void Tracer::disableAllImpl()
{
	mCategories.fill(false);
}

void Tracer::debugImpl(tracer::TracerEnum const &category, QString const &methodName, QString const &message)
{
	if (mCategories[category])
		qDebug() << "Trace ---" << methodName << ":" << message;
}
