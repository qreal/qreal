/* Copyright 2015 QReal Research Group, Yurii Litvinov
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

#include "exceptions/qrrepoException.h"
#include "private/qrRepoGlobal.h"

namespace qrRepo {

/// Thrown when folder with uncompressed save is not even found. Seems like internal error or something with rights
/// of 'temp' folder where uncompressed save shall be.
class QRREPO_EXPORT SourceFolderNotFoundException : public QrRepoException
{
public:
	explicit SourceFolderNotFoundException(const QString &folderName)
		: QrRepoException("Uncompressed save folder not found: " + folderName)
	{}
};

}
