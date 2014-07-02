#pragma once

#include <QtCore/QObject>
#include <QtCore/QLinkedList>

#include <qrkernel/ids.h>
#include "generatorBase/generatorCustomizer.h"

namespace generatorBase {
namespace semantics {

/// A base for all semantic nodes
class SemanticNode : public QObject
{
public:
	/// Returns an id of the binded to this semantic node block
	qReal::Id id() const;

	/// Binds this node to a given block
	void bindTo(qReal::Id const &id);

	/// Attaches this node to given parent
	void setParentNode(SemanticNode *parent);

	/// Places goto label near this node
	void addLabel();

	/// Generates code for this semantic node
	QString toString(GeneratorCustomizer &customizer, int indent) const;

	/// Performs deep (recursive) search in children subhierarchy and returns
	/// a node with specified id binded if such was found or NULL otherwise.
	SemanticNode *findNodeFor(qReal::Id const &id);

protected:
	explicit SemanticNode(qReal::Id const &idBinded = qReal::Id(), QObject *parent = 0);

	virtual QLinkedList<SemanticNode *> children() const = 0;

	virtual QString toStringImpl(GeneratorCustomizer &customizer, int indent) const = 0;

	qReal::Id mId;
	SemanticNode *mParentNode;
	bool mLabeled;
};

}
}
