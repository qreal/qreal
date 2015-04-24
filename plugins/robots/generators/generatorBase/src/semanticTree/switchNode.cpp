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

#include "generatorBase/semanticTree/switchNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

SwitchNode::SwitchNode(const Id &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mDefaultBranch(nullptr)
	, mBranchesMerged(false)
{
}

void SwitchNode::addBranch(const QString &value, SemanticNode * const node)
{
	ZoneNode * const zone = new ZoneNode(this);
	zone->setParentNode(this);
	bind(value, zone);
	if (node) {
		zone->appendChild(node);
	}
}

void SwitchNode::mergeBranch(const QString &value, NonZoneNode * const node)
{
	Q_ASSERT(node);
	bind(value, node->parentZone());
}

bool SwitchNode::branchesMerged() const
{
	return mBranchesMerged;
}

void SwitchNode::setBranchesMergedFlag()
{
	mBranchesMerged = true;
}

QString SwitchNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	QString result;
	bool isHead = true;
	for (ZoneNode * const zone : mBranches.values().toSet()) {
		if (zone == mDefaultBranch) {
			// Branches merged with default on the diagram will be merged with it in code too
			continue;
		}

		result += generatePart(customizer, indent, indentString, zone, isHead
				? customizer.factory()->switchHeadGenerator(mId, customizer, mBranches.keys(zone))
				: customizer.factory()->switchMiddleGenerator(mId, customizer, mBranches.keys(zone)));

		isHead = false;
	}

	if (result.isEmpty()) {
		// Then all branches lead to one block, we may ignore switch construction.
		return mDefaultBranch->toString(customizer, indent, indentString);
	}

	result += generatePart(customizer, indent, indentString, mDefaultBranch
			, customizer.factory()->switchDefaultGenerator(mId, customizer));

	return result;
}

void SwitchNode::bind(const QString &value, ZoneNode *zone)
{
	if (value.isEmpty()) {
		mDefaultBranch = zone;
	} else {
		mBranches[value] = zone;
	}
}

QString SwitchNode::generatePart(generatorBase::GeneratorCustomizer &customizer
		, int indent
		, const QString &indentString
		, ZoneNode * const zone
		, generatorBase::simple::AbstractSimpleGenerator *generator) const
{
	return utils::StringUtils::addIndent(generator->generate()
			.replace("@@BODY@@", zone->toString(customizer, indent + 1, indentString)), indent, indentString);
}

QLinkedList<SemanticNode *> SwitchNode::children() const
{
	QLinkedList<SemanticNode *> result;
	for (ZoneNode * const zone : mBranches.values().toSet()) {
		result << zone;
	}

	if (mDefaultBranch) {
		result << mDefaultBranch;
	}

	return result;
}
