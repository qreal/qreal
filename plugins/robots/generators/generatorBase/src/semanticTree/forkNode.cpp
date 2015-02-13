#include "generatorBase/semanticTree/forkNode.h"

#include <qrutils/stringUtils.h>

using namespace generatorBase::semantics;

ForkNode::ForkNode(const qReal::Id &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void ForkNode::appendThread(const qReal::Id &thread, const QString &threadId)
{
	mThreads[thread] = threadId;
}

QLinkedList<SemanticNode *> ForkNode::children() const
{
	return {};
}

QString ForkNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	const QString code = customizer.factory()->forkCallGenerator(mId, customizer, mThreads)->generate();
	return utils::StringUtils::addIndent(code, indent, indentString);
}
