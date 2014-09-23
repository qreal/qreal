#include "generatorBase/semanticTree/switchNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

SwitchNode::SwitchNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mDefaultBranch(nullptr)
	, mBranchesMerged(false)
{
}

void SwitchNode::addBranch(QString const &value, SemanticNode * const node)
{
	ZoneNode * const zone = new ZoneNode(this);
	zone->setParentNode(this);
	bind(value, zone);
	if (node) {
		zone->appendChild(node);
	}
}

void SwitchNode::mergeBranch(QString const &value, NonZoneNode * const node)
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

QString SwitchNode::toStringImpl(GeneratorCustomizer &customizer, int indent) const
{
	QString result;
	bool isHead = true;
	for (ZoneNode * const zone : mBranches.values().toSet()) {
		if (zone == mDefaultBranch) {
			// Branches merged with default on the diagram will be merged with it in code too
			continue;
		}

		result += generatePart(customizer, indent, zone, isHead
				? customizer.factory()->switchHeadGenerator(mId, customizer, mBranches.keys(zone))
				: customizer.factory()->switchMiddleGenerator(mId, customizer, mBranches.keys(zone)));

		isHead = false;
	}

	result += generatePart(customizer, indent, mDefaultBranch
			, customizer.factory()->switchDefaultGenerator(mId, customizer));

	return result;
}

void SwitchNode::bind(QString const &value, ZoneNode *zone)
{
	if (value.isEmpty()) {
		mDefaultBranch = zone;
	} else {
		mBranches[value] = zone;
	}
}

QString SwitchNode::generatePart(generatorBase::GeneratorCustomizer &customizer
		, int indent
		, ZoneNode * const zone
		, generatorBase::simple::AbstractSimpleGenerator *generator) const
{
	return utils::StringUtils::addIndent(generator->generate()
			.replace("@@BODY@@", zone->toString(customizer, indent + 1)), indent);
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
