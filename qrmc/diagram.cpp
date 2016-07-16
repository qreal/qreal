/* Copyright 2007-2016 QReal Research Group, Yurii Litvinov
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

#include "diagram.h"

#include "classes/type.h"
#include "classes/enumType.h"
#include "classes/nodeType.h"
#include "classes/edgeType.h"
#include "editor.h"

using namespace qReal;
using namespace qrmc;

Diagram::Diagram(const qReal::Id &id, const qrRepo::LogicalRepoApi &api, Editor &editor
		, const QString &targetDirectory)
	: mId(id)
	, mApi(api)
	, mEditor(editor)
	, mTargetDirectory(targetDirectory)
{
	mDiagramName = mApi.name(id);
	mDiagramDisplayedName = mApi.stringProperty(id, "displayedName");
	mDiagramNodeName  = mApi.stringProperty(id, "nodeName");
}

Diagram::~Diagram()
{
	qDeleteAll(mTypes);
}

bool Diagram::init()
{
	for (const Id &id : mApi.children(mId)) {
		if (!mApi.isLogicalElement(id)) {
			continue;
		}

		if (id.element() == metaEntityNode) {
			Type * const nodeType = new NodeType(*this, mApi, id, mTargetDirectory);
			if (!nodeType->init(mDiagramName)) {
				delete nodeType;
				qDebug() << "can't load node";
				return false;
			}

			mTypes[nodeType->name()] = nodeType;
		} else if (id.element() == metaEntityEdge) {
			Type * const edgeType = new EdgeType(*this, mApi, id, mTargetDirectory);
			if (!edgeType->init(mDiagramName)) {
				delete edgeType;
				qDebug() << "can't load edge";
				return false;
			}

			mTypes[edgeType->name()] = edgeType;
		} else if (id.element() == metaEntityImport) {
			ImportSpecification import = {
				mApi.stringProperty(id, "importedFrom") + "::" + mApi.stringProperty(id, "as"),
				mApi.stringProperty(id, "as"),
				mApi.stringProperty(id, "displayedName"),
			};

			mImports.append(import);
		} else if (id.element() == metaEntityEnum) {
			Type * const enumType = new EnumType(*this, mApi, id);
			if (!enumType->init(mDiagramName)) {
				delete enumType;
				qDebug() << "can't load enum";
				return false;
			}

			mTypes[enumType->name()] = enumType;
		} else {
			qDebug() << "ERROR: unknown type" << id.element();
			return false;
		}
	}

	return true;
}

bool Diagram::resolve()
{
	for (const ImportSpecification &import : mImports) {
		const Type * const importedType = mEditor.findType(import.name);
		if (importedType == nullptr) {
			qDebug() << "ERROR: imported type" << import.name << "not found, skipping";
			continue;
		}

		Type * const copiedType = importedType->clone();
		copiedType->setName(import.as);
		copiedType->setDisplayedName(import.displayedName);
		copiedType->setDiagram(*this);
		copiedType->setContext(mDiagramName);
		mTypes.insert(copiedType->qualifiedName(), copiedType);
	}

	for (Type * const type : mTypes.values()) {
		if (!type->resolve()) {
			qDebug() << "ERROR: can't resolve type" << type->name();
			return false;
		}
	}

	return true;
}

Editor* Diagram::editor() const
{
	return &mEditor;
}

Type* Diagram::findType(const QString &name) const
{
	if (mTypes.contains(name)) {
		return mTypes[name];
	}

	return mEditor.findType(name);
}

const QMap<QString, Type *> &Diagram::types() const
{
	return mTypes;
}

QString Diagram::name() const
{
	return mDiagramName;
}

QString Diagram::nodeName() const
{
	return mDiagramNodeName;
}

QString Diagram::displayedName() const
{
	return mDiagramDisplayedName;
}

QString Diagram::generateMapMethod(const QString& lineTemplate
		, std::function<QString(Type * const, const QString &)> generator) const
{
	QString result;
	for (Type * const type : mTypes) {
		const QString line = generator(type, lineTemplate);
		if (!line.isEmpty()) {
			result += line + endline;
		}
	}

	return result;
}

QString Diagram::generateNamesMap(const QString& lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generateNames);
}

QString Diagram::generateMouseGesturesMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generateMouseGestures);
}

QString Diagram::generatePropertiesMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generatePropertyName);
}

QString Diagram::generatePropertyDefaultsMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generatePropertyDefaults);
}

QString Diagram::generatePropertyDisplayedNamesMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generatePropertyDisplayedNames);
}

QString Diagram::generateElementDescriptionMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generateElementDescription);
}

QString Diagram::generateParentsMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, &Type::generateParents);
}

QString Diagram::generateUsages(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generateUsages);
}

QString Diagram::generateConnections(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generateConnections);
}

QString Diagram::generateReferenceProperties(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generateReferenceProperties);
}

QString Diagram::generatePortTypes(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generatePortTypes);
}

QString Diagram::generatePropertyName(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generatePropertyName);
}

QString Diagram::generateContainers(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generateContainers);
}

QString Diagram::generateFactory(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generateFactory);
}

QString Diagram::generateIsNodeOrEdge(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generateIsNodeOrEdge);
}

QString Diagram::generatePossibleEdges(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, &Type::generatePossibleEdges);
}

QString Diagram::generateListMethod(const QString &lineTemplate
		, std::function<QString(const Type * const, const QString &)> generator) const
{
	QString result;
	bool isFirstLine = true;
	for (const Type * const type : mTypes) {
		QString line = generator(type, lineTemplate);
		if (line.isEmpty()) {
			continue;
		}

		if (!isFirstLine) {
			line.replace("if ", "else if ");
		}

		isFirstLine = false;
		result += line + endline;
	}

	if (result.isEmpty()) {
		return "	Q_UNUSED(element)" + endline;
	}

	return result;
}

QString Diagram::generateEnums(const QString &lineTemplate) const
{
	QString result;
	bool isFirstLine = true;

	for (const EnumType * const type : mEditor.getAllEnumTypes()) {
		QString line = type->generateEnums(lineTemplate);
		if (line.isEmpty()) {
			continue;
		}

		if (!isFirstLine) {
			line.replace("if ", "else if ");
		}

		isFirstLine = false;
		result += line + endline;
	}

	if (result.isEmpty()) {
		return "	Q_UNUSED(name);";
	}

	return result;
}

QString Diagram::generateNodeClasses(const QString &nodeTemplate) const
{
	return generateMapMethod(nodeTemplate, &Type::generateNodeClass);
}

QString Diagram::generateEdgeClasses(const QString &edgeTemplate) const
{
	return generateMapMethod(edgeTemplate, &Type::generateEdgeClass);
}

QString Diagram::generateResourceFile(const QString &resourceTemplate) const
{
	return generateMapMethod(resourceTemplate, &Type::generateResourceLine);
}
