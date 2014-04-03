#pragma once

#include "conditionalNode.h"

namespace qReal {
namespace robots {
namespace generators {
namespace semantics {

/// A semantic node for loops inside program
class LoopNode : public ConditionalNode
{
public:
	explicit LoopNode(Id const &idBinded, QObject *parent = 0);


	void appendChildren(QLinkedList<SemanticNode *> const &nodes);

	/// Switches between while-do and do-while forms.
	void setForm(bool doWhileForm);

	ZoneNode *bodyZone();

protected:
	QLinkedList<SemanticNode *> children() const override;
	QString toStringImpl(GeneratorCustomizer &customizer, int indent) const override;

private:
	ZoneNode *mBodyZone;  // Takes ownership
	bool mDoWhileForm;
};

}
}
}
}
