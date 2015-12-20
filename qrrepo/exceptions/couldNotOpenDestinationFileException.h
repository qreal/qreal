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

/// Thrown when save file can not be opened for writing for some reason.
class QRREPO_EXPORT CouldNotOpenDestinationFileException : public QrRepoException
{
public:
	explicit CouldNotOpenDestinationFileException(const QString &fileName)
		: QrRepoException("Could not open file for saving: " + fileName)
	{}
};

}
