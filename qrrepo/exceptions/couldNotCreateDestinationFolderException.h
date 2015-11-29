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

/// Thrown when decompressor can not create directory for decompressed files. It probably means that temp folder
/// (specified in QReal config) is write-protected.
class QRREPO_EXPORT CouldNotCreateDestinationFolderException : public QrRepoException
{
public:
	explicit CouldNotCreateDestinationFolderException(const QString &folderName)
		: QrRepoException("Could not create destination folder: " + folderName)
	{}
};

}
