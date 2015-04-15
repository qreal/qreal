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

#include <qrkernel/ids.h>

#include "qrgui/plugins/pluginManager/pluginsManagerDeclSpec.h"

namespace qReal {

/// This class describes such entity as explosion: explosion relation between source() and target() means
/// that source() can or must be binded with some instance of target() type. Double click on such
/// element unfolds a diagram with target element as root
class QRGUI_PLUGINS_MANAGER_EXPORT Explosion
{
public:
	Explosion(const Id &source, const Id &target, bool isReusable, bool requiresImmediateLinkage);

	/// The type of the source element
	Id source() const;

	/// The type of the target element
	Id target() const;

	/// True means that each instance of explosion binding will appear in
	/// special palette for future reusability
	bool isReusable() const;

	/// True means that elements of type @see source() have no sense without
	/// explosion binding so such binding must be created immediately after
	/// element creation
	bool requiresImmediateLinkage() const;

private:
	Id mSource;
	Id mTarget;
	bool mReusable;
	bool mRequiresImmediateLinkage;
};

}
