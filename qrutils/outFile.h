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

#pragma once

#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// A class that provides a convenient way to write information to a file.
/// File is opened for writing in constructor, if something went wrong log entry will be created.
/// Exampe of usage:
///
/// bool fileOpened;
/// OutFile out("/tmp/myFile.txt", &fileOpened);
/// if (!fileOpened) {
///     errorReported.addError(out.errorString());
///     return;
/// }
/// out() << "1\n";
/// out() << "2\n";
///
class QRUTILS_EXPORT OutFile
{
public:
	explicit OutFile(const QString &fileName, bool *success = 0);
	~OutFile();
	QTextStream &operator()();

	/// Flushes all new modifications in text stream into the file system.
	void flush();

	/// Returns a human-readable message of the last occured error.
	QString errorString() const;

private:
	QByteArray mDummyArray;
	QFile mFile;
	QTextStream mOut;
};

}
