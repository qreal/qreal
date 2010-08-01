#include "nodeType.h"
#include "diagram.h"
#include "metaCompiler.h"
#include "../utils/outFile.h"
#include "editor.h"
#include "utils/nameNormalizer.h"

#include <QDebug>

using namespace utils;

NodeType::NodeType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id id) : GraphicType(diagram, api, id), mIsPin(false),
mIsHavePin(false)
{
}

NodeType::~NodeType()
{
}

Type* NodeType::clone() const
{
	NodeType *result = new NodeType(mDiagram, mApi, mId);
	GraphicType::copyFields(result);
	return result;
}

bool NodeType::init(QString const &context)
{
	GraphicType::init(context);
	return true;
}

void NodeType::print()
{
	qDebug() << "node " << mName;
}



QString NodeType::generateIsNodeOrEdge(QString const &lineTemplate) const
{
	QString line = lineTemplate;
	line.replace(elementNameTag, name()).replace(isNodeOrEdgeTag, "1");
	return line;
}

QString NodeType::generateClass(const QString &classTemplate) const
{
	if (!mIsVisible)
		return "";
	QString nodeClass = classTemplate;
//	MetaCompiler *compiler = diagram()->editor()->metaCompiler();

	mShape.generate(nodeClass);



	nodeClass.replace(elementNameTag, name())
			.replace("\\n", "\n");
	nodeClass += endline;
	return nodeClass;
}
