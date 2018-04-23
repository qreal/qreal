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

#include "generatorBase/semanticTree/semanticNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

SemanticNode::SemanticNode(const Id &idBinded, QObject *parent)
	: QObject(parent)
	, mId(idBinded)
	, mParentNode(nullptr)
	, mLabeled(false)
	, mHasBreakInside(false)
{
}

SemanticNode::SemanticNode(const QString &text, QObject *parent)
	: QObject(parent)
	, mId(qReal::Id())
	, mParentNode(nullptr)
	, mLabeled(false)
	, mHasBreakInside(false)
	, mText(text)
{
}

Id SemanticNode::firstId() const
{
	return mId;
}

void SemanticNode::makeBreakForLastIf(bool isConditionTrue, SemanticNode *breakNode)
{
	Q_UNUSED(isConditionTrue)
	Q_UNUSED(breakNode)
}

Id SemanticNode::id() const
{
	return mId;
}

void SemanticNode::bindTo(const Id &id)
{
	mId = id;
}

void SemanticNode::setParentNode(SemanticNode * const parent)
{
	mParentNode = parent;
}

void SemanticNode::addLabel()
{
	mLabeled = true;
}

void SemanticNode::setHasBreakInside(bool hasBreakInside)
{
	mHasBreakInside = hasBreakInside;
}

bool SemanticNode::hasBreakInside() const
{
	return mHasBreakInside;
}

QString SemanticNode::toString(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	const QString code = toStringImpl(customizer, indent, indentString);
	/// @todo: Probably some more generalized entity? Prepended and appended code generators in general?
	auto prependedCodeGenerator = customizer.factory()->labelGenerator(mId, customizer);
	const QString prependedCode = mLabeled
			? utils::StringUtils::addIndent(prependedCodeGenerator->generate(), indent, indentString)
			: QString();
	return prependedCode + code;
}

SemanticNode *SemanticNode::findNodeFor(const Id &id)
{
	if (id == mId) {
		return this;
	}

	QLinkedList<SemanticNode *> const children = this->children();
	for (SemanticNode * const child : children) {
		SemanticNode * const searchResult = child->findNodeFor(id);
		if (searchResult) {
			return searchResult;
		}
	}

	return nullptr;
}

Id SemanticNode::lastIfId() const
{
	return mId;
}
