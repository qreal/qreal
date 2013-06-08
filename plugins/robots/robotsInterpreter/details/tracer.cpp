#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include "tracer.h"

using namespace qReal::interpreters::robots::details;

Tracer* Tracer::mInstance = NULL;

QString const logFileName = QString("QRealRobots.log");

void Tracer::enableCategory(tracer::TracerEnum category)
{
	instance()->enableCategoryImpl(category);
}

void Tracer::disableCategory(tracer::TracerEnum category)
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

void Tracer::setTarget(tracer::TraceTarget target)
{
	instance()->mTarget = target;
}

void Tracer::debug(tracer::TracerEnum category, QString const &methodName, QString const &message)
{
	instance()->debugImpl(category, methodName, message);
}

Tracer::Tracer()
	: mTarget(tracer::debug)
	, mLogPath(QString("%1/%2").arg(QCoreApplication::applicationDirPath(), logFileName))
{
	mCategories.resize(tracer::sensors + 1);
}

Tracer *Tracer::instance()
{
	if (!mInstance) {
		mInstance = new Tracer;
	}
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
	if (mCategories[category]) {
		switch (mTarget) {
		case tracer::debug:
			qDebug() << "Trace ---" << methodName << ":" << message;
		case tracer::logFile:
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
