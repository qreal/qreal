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

#include "qrkernel/kernelDeclSpec.h"

namespace qReal {

/// Base class for all QReal exceptions. Thrown when "something is wrong" in QReal code.
class QRKERNEL_EXPORT Exception
{
public:
	/// Constructor.
	/// @param message Error message.
	explicit Exception(const QString &message);

	/// Get error message.
	QString message() const;

private:
	/// Error message.
	const QString mMessage;
};

}
