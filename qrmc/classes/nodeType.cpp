#include "nodeType.h"
#include "diagram.h"
#include "metaCompiler.h"
#include "editor.h"
#include "utils/nameNormalizer.h"
#include "../qrrepo/repoApi.h"

#include <QDebug>

using namespace qrmc;
using namespace qReal;

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

QString NodeType::generateEdgeClass(const QString &classTemplate) const
{
	Q_UNUSED(classTemplate)
	return "";
}

QString NodeType::generateNodeClass(const QString &classTemplate)
{
	if (!mIsVisible)
		return "";

	QString nodeClass = classTemplate;
	MetaCompiler *compiler = diagram()->editor()->metaCompiler();

	mShape.setNode(this);
	mShape.generate(nodeClass);
	generateContainerStuff(nodeClass);
	generateContextMenuItems(nodeClass, compiler);

	QString isContainer  = mContains.isEmpty() ? "false" : "true";
	QString isAction = loadBoolProperty(mId, "isAction");
	QString border = isAction == "true"
					? compiler->getTemplateUtils(nodeValidBorderTag)
					: compiler->getTemplateUtils(nodeInvalidBorderTag);

	nodeClass.replace(isContainerTag, isContainer)
			.replace(isPortTag, loadBoolProperty(mId, "isPin"))
			.replace(hasPinTag, isAction)
			.replace(nodeBorderTag, border)
			.replace(isNodeTag, "true")
			.replace(elementNameTag, name())
			.replace("\\n", "\n");
	nodeClass += endline;
	return nodeClass;
}

QString NodeType::loadBoolProperty(qReal::Id const &id, const QString &property) const
{
	QString result = mApi->stringProperty(id, property);
	if (result.isEmpty())
		result = "false";
	return result;
}

QString NodeType::loadIntProperty(qReal::Id const &id, const QString &property) const
{
	QString result = mApi->stringProperty(id, property);
	if (result.isEmpty())
		result = "0";
	return result;
}

void NodeType::generateContainerStuff(QString &classTemplate) const
{
	IdList children = mApi->children(mId);
	bool foundChild = false;
	foreach(Id child, children){
		if (!mApi->isLogicalElement(child))
			continue;

		if (child.element() == metaEntityPropertiesAsContainer) {
			foundChild = true;
			QString movableChildren = mApi->stringProperty(child, "banChildrenMove") == "true" ? "false" : "true";
			classTemplate.replace(sortingContainerTag, loadBoolProperty(child, "sortContainer"))
					.replace(minimizeToChildrenTag, loadBoolProperty(child, "minimizeToChildren"))
					.replace(maximizeChildrenTag, loadBoolProperty(child, "maximizeChildren"))
					.replace(hasMovableChildrenTag, movableChildren)
					.replace(forestallingSizeTag, loadIntProperty(child, "forestallingSize"))
					.replace(childrenForestallingSizeTag, loadIntProperty(child, "childrenForestallingSize"));
			break;
		}
	}
	if (!foundChild)
		classTemplate.replace(sortingContainerTag, "false")
				.replace(minimizeToChildrenTag, "false")
				.replace(maximizeChildrenTag, "false")
				.replace(hasMovableChildrenTag, "true")
				.replace(forestallingSizeTag, "0")
				.replace(childrenForestallingSizeTag, "0");
}

void NodeType::generateContextMenuItems(QString &classTemplate, MetaCompiler *compiler) const
{
	if (mContextMenuItems.isEmpty()){
		classTemplate.replace(nodeConstructorTag, "")
				.replace(itemsList, compiler->getTemplateUtils(itemsInvalidList));
		return;
	}

	QString constructor = compiler->getTemplateUtils(nodeConstructorTag);
	QString items;
	foreach(QString item, mContextMenuItems){
		items += " << \"" + item + "\"";
	}
	constructor.replace(contextMenuItems, items);
	classTemplate.replace(nodeConstructorTag, constructor)
			.replace(itemsList, compiler->getTemplateUtils(itemsValidList));
}

QString NodeType::generateResourceLine(const QString &resourceTemplate) const
{
	return mShape.generateResourceLine(resourceTemplate);
}
