#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include "tracer.h"

using namespace interpreterCore::utils;

Tracer* Tracer::mInstance = NULL;

QString const logFileName = QString("QRealRobots.log");

void Tracer::enableCategory(enums::TracerEnum category)
{
	instance()->enableCategoryImpl(category);
}

void Tracer::disableCategory(enums::TracerEnum category)
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

void Tracer::setTarget(enums::TraceTarget target)
{
	instance()->mTarget = target;
}

void Tracer::debug(enums::TracerEnum category, QString const &methodName, QString const &message)
{
	instance()->debugImpl(category, methodName, message);
}

Tracer::Tracer()
	: mTarget(enums::debug)
	, mLogPath(QString("%1/%2").arg(QCoreApplication::applicationDirPath(), logFileName))
{
	mCategories.resize(enums::sensors + 1);
}

Tracer *Tracer::instance()
{
	if (!mInstance) {
		mInstance = new Tracer;
	}
	return mInstance;
}

void Tracer::enableCategoryImpl(enums::TracerEnum const &category)
{
	mCategories[category] = true;
}

void Tracer::disableCategoryImpl(enums::TracerEnum const &category)
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

void Tracer::debugImpl(enums::TracerEnum const &category, QString const &methodName, QString const &message)
{
	if (mCategories[category]) {
		switch (mTarget) {
		case enums::debug:
			qDebug() << "Trace ---" << methodName << ":" << message;
		case enums::logFile:
			QFile file(mLogPath);
			if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
				return;
			}
			QTextStream outputStream(&file);
			outputStream << "Trace --- " << methodName << ": " << message;
			endl(outputStream);
		}
	}
}
