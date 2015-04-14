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

#pragma once

#include <QtCore/QString>

#include "qrutils/utilsDeclSpec.h"

namespace qReal {

/// This class contains platform-dependent file system operations implementations
class QRUTILS_EXPORT FileSystemUtils
{
public:
	/// Tries hide specified file on windows and returns if operation was successfull.
	/// To make file hidden on Linux just make its name starting with '.'
	static bool makeHidden(const QString &filePath);
};

}
