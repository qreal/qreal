#include "generatorBase/semanticTree/switchNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;
using namespace qReal;

SwitchNode::SwitchNode(Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
	, mBranchesMerged(false)
	, mDefaultBranch(nullptr)
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

	result += "{\n";
	for (ZoneNode * const zone : mBranches.values().toSet()) {
		for (QString const &branch : mBranches.keys(zone)) {
			result += branch + ":\n";
		}

		result += zone->toString(customizer, indent) + "\n";
	}

	result += "default:\n" + mDefaultBranch->toString(customizer, indent) + "\n";
	result += "}\n";
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
