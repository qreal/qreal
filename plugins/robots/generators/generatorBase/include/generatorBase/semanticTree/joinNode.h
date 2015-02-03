#pragma once

#include "nonZoneNode.h"

namespace generatorBase {
namespace semantics {

class JoinNode : public NonZoneNode
{
public:
	explicit JoinNode(const qReal::Id &idBinded, QObject *parent = 0);

	void setThreadId(const QString &threadId);

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const override;

private:
	QString mThreadId;
};

}
}
