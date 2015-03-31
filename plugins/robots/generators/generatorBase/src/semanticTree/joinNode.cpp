#include "generatorBase/semanticTree/joinNode.h"

#include <qrutils/stringUtils.h>
#include "generatorBase/parts/threads.h"

using namespace generatorBase::semantics;

JoinNode::JoinNode(const qReal::Id &idBinded, QObject *parent)
	: NonZoneNode(idBinded, parent)
{
}

void JoinNode::setThreadId(const QString &threadId)
{
	mThreadId = threadId;
}

QLinkedList<SemanticNode *> JoinNode::children() const
{
	return {};
}

QString JoinNode::toStringImpl(GeneratorCustomizer &customizer, int indent, const QString &indentString) const
{
	const QString code = customizer.factory()->joinGenerator(mId, customizer
			, customizer.factory()->threads().joinedThreads(mId), mThreadId)->generate();
	return utils::StringUtils::addIndent(code, indent, indentString);
}
