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
#include <QtCore/QDir>
#include <QtCore/QTextStream>

#include "qrutils/utilsDeclSpec.h"

namespace utils {

/// Helper class for reading data from a file.
class QRUTILS_EXPORT InFile
{
public:
	/// Read all data from given file.
	/// @param fileName Name of the file to be read.
	/// @param errorString If non-null and file open operation failed it will be assigned with
	///  human-readable error message.
	/// @returns All contents of a file.
	static QString readAll(const QString &fileName, QString *errorString = 0);
};

}
