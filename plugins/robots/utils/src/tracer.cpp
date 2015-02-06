#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>

#include <utils/tracer.h>

using namespace utils;

Tracer *Tracer::mInstance = nullptr;

const QString logFileName = QString("QRealRobots.log");

void Tracer::enableCategory(TracerEnum category)
{
	instance()->enableCategoryImpl(category);
}

void Tracer::disableCategory(TracerEnum category)
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

void Tracer::setTarget(TraceTarget target)
{
	instance()->mTarget = target;
}

void Tracer::debug(TracerEnum category, const QString &methodName, const QString &message)
{
	instance()->debugImpl(category, methodName, message);
}

Tracer::Tracer()
	: mTarget(debugOutput)
	, mLogPath(QString("%1/%2").arg(QCoreApplication::applicationDirPath(), logFileName))
{
	mCategories.resize(sensors + 1);
}

Tracer *Tracer::instance()
{
	if (!mInstance) {
		mInstance = new Tracer;
	}
	return mInstance;
}

void Tracer::enableCategoryImpl(TracerEnum const &category)
{
	mCategories[category] = true;
}

void Tracer::disableCategoryImpl(TracerEnum const &category)
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

void Tracer::debugImpl(TracerEnum const &category, const QString &methodName, const QString &message)
{
	if (mCategories[category]) {
		switch (mTarget) {
		case debugOutput:
			qDebug() << "Trace ---" << methodName << ":" << message;
		case logFile:
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
