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

#include "qrtext/core/connection.h"

#include "qrtext/declSpec.h"

namespace qrtext {
namespace core {

/// A range between two connections in code. Characterizes location of a token in code.
class QRTEXT_EXPORT Range
{
public:
	/// Constructor that takes start and end coordinates of a range.
	Range(const Connection &start, const Connection &end);

	/// Constructor of an invalid range object.
	Range();

	/// Returns start coordinate of a range.
	const Connection &start() const;

	/// Returns end coordinate of a range.
	const Connection &end() const;

private:
	Connection mStart;
	Connection mEnd;
};

}
}
