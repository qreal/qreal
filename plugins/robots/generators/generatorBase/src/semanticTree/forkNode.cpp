#include "generatorBase/semanticTree/forkNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;

ForkNode::ForkNode(qReal::Id const &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void ForkNode::appendThread(qReal::Id const &thread)
{
	mThreads << thread;
}

QLinkedList<SemanticNode *> ForkNode::children() const
{
	return {};
}

QString ForkNode::toStringImpl(GeneratorCustomizer &customizer, int indent, QString const &indentString) const
{
	QString const code = customizer.factory()->forkCallGenerator(mId, customizer, mThreads.toList())->generate();
	return utils::StringUtils::addIndent(code, indent, indentString);
}
