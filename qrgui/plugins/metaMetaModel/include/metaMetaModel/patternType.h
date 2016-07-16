/* Copyright 2016 Dmitry Mordvinov
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

#include "metaMetaModel/elementType.h"

namespace qReal {

/// Describes type of group of node and edge elements that can be instantiated on scene by dragging out
/// one entry from palette.
/// @todo: This entity is not exactly ElementType. ElementType contains many methods that have no sense for patterns
/// (labels(), propertyNames() and so on). There must be actially common base for ElementType and PatternType.
class QRGUI_META_META_MODEL_EXPORT PatternType : public ElementType
{
public:
	/// @param metamodel Metamodel that owns this node element.
	explicit PatternType(Metamodel &metamodel);

	Type type() const override;

	/// Returns the XML description of pattern.
	/// @todo: XML is not needed for this. This is just a piece of model, can be formalized as multigraph
	/// and generated as-is.
	QString xml() const;

	/// Sets the XML description of pattern.
	/// @todo: XML is not needed for this. This is just a piece of model, can be generated as-is.
	void setXml(const QString &xml);

private:
	QString mXml;
};

}
