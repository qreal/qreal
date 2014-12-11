#pragma once

#include "nonZoneNode.h"

namespace generatorBase {
namespace semantics {

class SemanticTree;

/// A semantic node for the block with fork semantics. One of the threads stays in current one,
/// other represented as separate semantic trees whitch will be generated as separate task.
class ForkNode : public NonZoneNode
{
public:
	explicit ForkNode(qReal::Id const &idBinded, QObject *parent = 0);

	/// Adds one more thread to this fork block.
	void appendThread(qReal::Id const &thread);

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, QString const &indentString) const override;

private:
	QSet<qReal::Id> mThreads;
};

}
}
