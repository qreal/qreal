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
	explicit ForkNode(const qReal::Id &idBinded, QObject *parent = 0);

	/// Adds one more thread to this fork block.
	void appendThread(const qReal::Id &thread);

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	QSet<qReal::Id> mThreads;
};

}
}
