#pragma once

#include "zoneNode.h"

namespace generatorBase {
namespace semantics {

/// A root node for all semantic tree
class RootNode : public SemanticNode
{
public:
	explicit RootNode(SemanticNode * const initialBlock, QObject *parent = 0);

	qReal::Id initialBlock() const;

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	ZoneNode *mZone;  // Takes ownership
	const qReal::Id mInitialBlock;
};

}
}
