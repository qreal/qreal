/* Copyright 2007-2015 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

void Tracer::enableCategoryImpl(const TracerEnum &category)
{
	mCategories[category] = true;
}

void Tracer::disableCategoryImpl(const TracerEnum &category)
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

void Tracer::debugImpl(const TracerEnum &category, const QString &methodName, const QString &message)
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
