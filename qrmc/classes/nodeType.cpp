/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "nodeType.h"
#include "../diagram.h"
#include "../metaCompiler.h"
#include "../editor.h"
#include "../utils/nameNormalizer.h"
#include "../../qrrepo/repoApi.h"

#include <QDebug>

using namespace qrmc;
using namespace qReal;

NodeType::NodeType(Diagram *diagram, qrRepo::LogicalRepoApi *api, qReal::Id id) : GraphicType(diagram, api, id)
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

bool NodeType::init(const QString &context)
{
	GraphicType::init(context);
	return true;
}

void NodeType::print()
{
	qDebug() << "node " << mName;
}



QString NodeType::generateIsNodeOrEdge(const QString &lineTemplate) const
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
			.replace(nodeBorderTag, border)
			.replace(isNodeTag, "true")
			.replace(elementNameTag, name())
			.replace(isResizeable, loadBoolProperty(mId, "isResizeable"))
			.replace("\\n", "\n");
	nodeClass += endline;
	return nodeClass;
}

QString NodeType::loadBoolProperty(const qReal::Id &id, const QString &property) const
{
	QString result = mApi->stringProperty(id, property);
	if (result.isEmpty()) {
		result = "false";
	}
	return result;
}

QString NodeType::loadIntProperty(const qReal::Id &id, const QString &property) const
{
	QString result = mApi->stringProperty(id, property);
	if (result.isEmpty()) {
		result = "0";
	}
	return result;
}

QString NodeType::loadIntVectorProperty(const qReal::Id &id, const QString &property) const
{
	QString result = mApi->stringProperty(id, property);
	if (result.isEmpty()) {
		result = "0,0,0,0";
	}
	return result;
}

void NodeType::generateContainerStuff(QString &classTemplate) const
{
	IdList children = mApi->children(mId);
	bool foundChild = false;
	foreach(Id child, children) {
		if (!mApi->isLogicalElement(child))
			continue;

		if (child.element() == metaEntityPropertiesAsContainer) {
			foundChild = true;
			QString movableChildren = mApi->stringProperty(child, "banChildrenMove") == "true" ? "false" : "true";
			classTemplate.replace(sortingContainerTag, loadBoolProperty(child, "sortContainer"))
					.replace(minimizeToChildrenTag, loadBoolProperty(child, "minimizeToChildren"))
					.replace(maximizeChildrenTag, loadBoolProperty(child, "maximizeChildren"))
					.replace(hasMovableChildrenTag, movableChildren)
					.replace(forestallingSizeTag, loadIntVectorProperty(child, "forestallingSize"))
					.replace(childrenForestallingSizeTag, loadIntProperty(child, "childrenForestallingSize"));
			break;
		}
	}
	if (!foundChild)
		classTemplate.replace(sortingContainerTag, "false")
				.replace(minimizeToChildrenTag, "false")
				.replace(maximizeChildrenTag, "false")
				.replace(hasMovableChildrenTag, "true")
				.replace(forestallingSizeTag, "QVector<int>()")
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
