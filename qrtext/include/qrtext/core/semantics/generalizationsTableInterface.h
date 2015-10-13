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

#include "qrtext/core/types/typeExpression.h"

namespace qrtext {
namespace core {

/// Interface that tells type inferrer which type is a subtype of which. Shall be implemented for concrete language.
class GeneralizationsTableInterface
{
public:
	virtual ~GeneralizationsTableInterface() {}

	/// Shall return true if "specific" type is a subtype of "general" type, including case when they are equivalent.
	virtual bool isGeneralization(const QSharedPointer<types::TypeExpression> &specific
			, const QSharedPointer<types::TypeExpression> &general) const = 0;
};

}
}
