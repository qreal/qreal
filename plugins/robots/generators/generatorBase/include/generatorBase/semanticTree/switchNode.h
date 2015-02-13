#pragma once

#include "nonZoneNode.h"

namespace generatorBase {
namespace semantics {

/// A semantic node for the block with 'switch' semantics
class SwitchNode : public NonZoneNode
{
public:
	explicit SwitchNode(const qReal::Id &idBinded, QObject *parent = 0);

	/// Adds new branch for the switch value (i.e. branch zone will be created for the given value).
	void addBranch(const QString &value, SemanticNode * const node);

	/// Binds existing branch with the switch value (i.e. value will be binded with the parent zone of the node).
	void mergeBranch(const QString &value, NonZoneNode * const node);

	/// Returns true if branches for this switch block were already merged.
	/// This can help to resolve confusion in case when one branch enter into the middle of another.
	bool branchesMerged() const;

	/// Marks that branches for this switch block were already merged.
	/// This can help to resolve confusion in case when one branch enter into the middle of another.
	void setBranchesMergedFlag();

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	void bind(const QString &value, ZoneNode *zone);
	QString generatePart(GeneratorCustomizer &customizer
		, int indent
		, const QString &indentString
		, ZoneNode * const zone
		, simple::AbstractSimpleGenerator *generator) const;

	QMap<QString, ZoneNode *> mBranches;
	ZoneNode *mDefaultBranch;
	bool mBranchesMerged;
};

}
}
