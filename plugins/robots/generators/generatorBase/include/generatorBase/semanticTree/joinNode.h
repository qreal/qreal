#pragma once

#include "nonZoneNode.h"

namespace generatorBase {
namespace semantics {

/// A semantic node for a block of join semantics.
class JoinNode : public NonZoneNode
{
public:
	explicit JoinNode(const qReal::Id &idBinded, QObject *parent = 0);

	/// Sets a main thread for this join (the one which will continue execution after join).
	void setThreadId(const QString &threadId);

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	QString mThreadId;
};

}
}
