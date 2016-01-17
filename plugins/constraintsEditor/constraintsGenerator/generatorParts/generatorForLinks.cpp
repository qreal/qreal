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

#include "generatorForLinks.h"

using namespace constraints::generator::generatorParts;
using namespace qReal;

bool GeneratorForLinks::linkWithGivenTypeExists(
		const qReal::Id &element
		, const QString &type
		, const qrRepo::LogicalRepoApi &api)
{
	for (const Id &link : api.links(element)) {
		if (link.element() == type) {
			return true;
		}
	}

	return false;
}
