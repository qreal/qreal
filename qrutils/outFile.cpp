/* Copyright 2007-2015 QReal Research Group, Dmitry Mordvinov
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

#include "outFile.h"

#include <QtCore/QBuffer>

#include <qrkernel/logging.h>

using namespace utils;

OutFile::OutFile(const QString &fileName, bool *success)
	: mFile(fileName)
	, mOut(&mDummyArray)
{
	if (fileName.isEmpty()) {
		if (success) {
			*success = false;
		}

		return;
	}

	if (!mFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QLOG_ERROR() << QString("Opening %1 for write failed: %2").arg(fileName, mFile.errorString());
		if (success) {
			*success = false;
		}

		return;
	}

	mOut.setDevice(&mFile);
	mOut.setCodec("UTF-8");
	if (success) {
		*success = true;
	}
}

OutFile::~OutFile()
{
	mFile.close();
}

QTextStream &OutFile::operator()()
{
	return mOut;
}

void OutFile::flush()
{
	mOut.flush();
}

QString OutFile::errorString() const
{
	return mFile.errorString();
}
