#pragma once

#include "nonZoneNode.h"

namespace generatorBase {
namespace semantics {

/// A semantic node for the block with if semantics
class SwitchNode : public NonZoneNode
{
public:
	explicit SwitchNode(qReal::Id const &idBinded, QObject *parent = 0);

	/// Adds new branch for the switch value (i.e. branch zone will be created for the given value).
	void addBranch(QString const &value, SemanticNode * const node);

	/// Binds existing branch with the switch value (i.e. value will be binded with the parent zone of the node).
	void mergeBranch(QString const &value, NonZoneNode * const node);

	/// Returns true if branches for this switch block were already merged.
	/// This can help to resolve confusion in case when one branch enter into the middle of another.
	bool branchesMerged() const;

	/// Marks that branches for this switch block were already merged.
	/// This can help to resolve confusion in case when one branch enter into the middle of another.
	void setBranchesMergedFlag();

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent) const override;

private:
	void bind(QString const &value, ZoneNode *zone);
	QString generatePart(GeneratorCustomizer &customizer
		, int indent
		, ZoneNode * const zone
		, simple::AbstractSimpleGenerator *generator) const;

	QMap<QString, ZoneNode *> mBranches;
	ZoneNode *mDefaultBranch;
	bool mBranchesMerged;
};

}
}
