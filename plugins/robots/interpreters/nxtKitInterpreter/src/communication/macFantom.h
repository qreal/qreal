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

#include "fantom.h"

#include <QtCore/QLibrary>

namespace nxt {
namespace communication {

class MacFantom : public Fantom
{
	Q_OBJECT
public:
	/// Constructor. Tries to load a dylib.
	MacFantom();

	/// Returns true if fantom driver is available, false otherwise.
	virtual bool isAvailable() const;
	virtual void checkConsistency();

private:
	/// Returns available if fantom driver is available, notFound otherwise. Return x64 if it's 64-bit Mac.
	enum class Status { available, notFound, x64 };
	Status availability() const;

	unsigned long onDriverUnavailable();

	Status mAvailability;

};

}
}
