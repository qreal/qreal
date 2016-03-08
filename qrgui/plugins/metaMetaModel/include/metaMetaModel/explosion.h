/* Copyright 2013-2016 Dmitry Mordvinov
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
#include <qrgraph/edge.h>

#include "metaMetaModel/metaMetaModelDeclSpec.h"

namespace qReal {

class Metamodel;
class ElementType;

/// This class describes such entity as explosion: explosion relation between source() and target() means
/// that source() can or must be binded with some instance of target() type. Double click on such
/// element unfolds a diagram with target element as root
class QRGUI_META_META_MODEL_EXPORT Explosion : public qrgraph::Edge
{
public:
	Explosion(Metamodel &metamodel, bool isReusable, bool requiresImmediateLinkage);

	/// The type of the source element (which explodes to target).
	ElementType &source() const;

	/// The type of the target element (element to which source will explode to).
	ElementType &target() const;

	/// True means that each instance of explosion binding will appear in special palette for future reusability.
	bool isReusable() const;

	/// True means that elements of type @see source() have no sense without explosion binding so such binding must
	/// be created immediately after element creation.
	bool requiresImmediateLinkage() const;

private:
	bool mReusable;
	bool mRequiresImmediateLinkage;
};

}
