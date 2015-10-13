/* Copyright 2007-2015 QReal Research Group
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

#include <QDebug>

using namespace qReal;
using namespace qrmc;

Diagram::Diagram(const qReal::Id &id,  qrRepo::LogicalRepoApi *api, Editor *editor)
	: mId(id), mApi(api), mEditor(editor)
{
	mDiagramName = mApi->name(id);
	mDiagramDisplayedName = mApi->stringProperty(id, "displayedName");
	mDiagramNodeName  = mApi->stringProperty(id, "nodeName");
}

Diagram::~Diagram()
{
	foreach(Type *type, mTypes.values())
		if (type)
			delete type;
}

bool Diagram::init()
{
	foreach(Id id, mApi->children(mId)) {
		if (!mApi->isLogicalElement(id))
			continue;

		if (id.element() == metaEntityNode) {
			Type *nodeType = new NodeType(this, mApi, id);
			if (!nodeType->init(mDiagramName)) {
				delete nodeType;
				qDebug() << "can't load node";
				return false;
			}
			mTypes[nodeType->name()] = nodeType;
		} else if (id.element() == metaEntityEdge) {
			Type *edgeType = new EdgeType(this, mApi, id);
			if (!edgeType->init(mDiagramName)) {
				delete edgeType;
				qDebug() << "can't load edge";
				return false;
			}
			mTypes[edgeType->name()] = edgeType;
		} else if (id.element() == metaEntityImport) {
			ImportSpecification import = {
				mApi->stringProperty(id, "importedFrom") + "::" + mApi->stringProperty(id, "as"),
				mApi->stringProperty(id, "as"),
				mApi->stringProperty(id, "displayedName"),
			};
			mImports.append(import);
		} else if (id.element() == metaEntityEnum) {
			Type *enumType = new EnumType(this, mApi, id);
			if (!enumType->init(mDiagramName))
			{
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
	foreach (ImportSpecification import, mImports) {
		Type *importedType = mEditor->findType(import.name);
		if (importedType == nullptr) {
			qDebug() << "ERROR: imported type" << import.name << "not found, skipping";
			continue;
		}
		Type *copiedType = importedType->clone();
		copiedType->setName(import.as);
		copiedType->setDisplayedName(import.displayedName);
		copiedType->setDiagram(this);
		copiedType->setContext(mDiagramName);
		mTypes.insert(copiedType->qualifiedName(), copiedType);
	}

	foreach(Type *type, mTypes.values())
		if (!type->resolve()) {
			qDebug() << "ERROR: can't resolve type" << type->name();
			return false;
		}

	return true;
}

Editor* Diagram::editor() const
{
	return mEditor;
}

Type* Diagram::findType(QString name)
{
	if (mTypes.contains(name))
		return mTypes[name];

	return mEditor->findType(name);
}

QMap<QString, Type*> Diagram::types() const
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

void Diagram::print()
{
	qDebug() << "elements:" << mTypes.size();
//	foreach(Type *type, mTypes)
//		type->print();
}

class Diagram::MapMethodGenerator {
public:
	virtual ~MapMethodGenerator() {}
	virtual QString generate(Type *type, const QString &lineTemplate) const = 0;
};

class Diagram::NamesGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateNames(lineTemplate);
	}
};

class Diagram::MouseGesturesGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateMouseGestures(lineTemplate);
	}
};

class Diagram::PropertyNamesGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateProperties(lineTemplate);
	}
};

class Diagram::PropertyDefaultsGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generatePropertyDefaults(lineTemplate);
	}
};

class Diagram::PropertyDisplayedNamesGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generatePropertyDisplayedNames(lineTemplate);
	}
};

class Diagram::ElementDescriptonGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateElementDescription(lineTemplate);
	}
};

class Diagram::ParentsMapGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateParents(lineTemplate);
	}
};

class Diagram::NodesGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateNodeClass(lineTemplate);
	}
};

class Diagram::EdgesGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateEdgeClass(lineTemplate);
	}
};

class Diagram::ResourceGenerator: public Diagram::MapMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateResourceLine(lineTemplate);
	}
};

QString Diagram::generateMapMethod(const QString& lineTemplate, const MapMethodGenerator &generator) const
{
	QString result;
	foreach(Type* type, mTypes) {
		QString line = generator.generate(type, lineTemplate);
		if (!line.isEmpty())
			result += line + endline;
	}
	return result;
}

QString Diagram::generateNamesMap(const QString& lineTemplate) const
{
	return generateMapMethod(lineTemplate, NamesGenerator());
}

QString Diagram::generateMouseGesturesMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, MouseGesturesGenerator());
}

QString Diagram::generatePropertiesMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, PropertyNamesGenerator());
}

QString Diagram::generatePropertyDefaultsMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, PropertyDefaultsGenerator());
}

QString Diagram::generatePropertyDisplayedNamesMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, PropertyDisplayedNamesGenerator());
}

QString Diagram::generateElementDescriptionMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, ElementDescriptonGenerator());
}

QString Diagram::generateParentsMap(const QString &lineTemplate) const
{
	return generateMapMethod(lineTemplate, ParentsMapGenerator());
}

class Diagram::ListMethodGenerator {
public:
	virtual ~ListMethodGenerator() {}
	virtual QString generate(Type *type, const QString &lineTemplate) const = 0;
};

class Diagram::UsagesGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateUsages(lineTemplate);
	}
};

class Diagram::ContainersGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateContainers(lineTemplate);
	}
};

class Diagram::ReferencePropertiesGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateReferenceProperties(lineTemplate);
	}
};

class Diagram::PortTypesGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generatePortTypes(lineTemplate);
	}
};

class Diagram::PropertyNameGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generatePropertyName(lineTemplate);
	}
};

class Diagram::ConnectionsGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateConnections(lineTemplate);
	}
};

class Diagram::FactoryGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateFactory(lineTemplate);
	}
};

class Diagram::IsNodeOrEdgeGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateIsNodeOrEdge(lineTemplate);
	}
};

class Diagram::EnumsGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generateEnums(lineTemplate);
	}
};

class Diagram::PossibleEdgesGenerator: public Diagram::ListMethodGenerator {
public:
	virtual QString generate(Type *type, const QString &lineTemplate) const {
		return type->generatePossibleEdges(lineTemplate);
	}
};

QString Diagram::generateUsages(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, UsagesGenerator());
}

QString Diagram::generateConnections(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, ConnectionsGenerator());
}

QString Diagram::generateReferenceProperties(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, ReferencePropertiesGenerator());
}

QString Diagram::generatePortTypes(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, PortTypesGenerator());
}

QString Diagram::generatePropertyName(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, PropertyNameGenerator());
}

QString Diagram::generateContainers(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, ContainersGenerator());
}

QString Diagram::generateFactory(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, FactoryGenerator());
}

QString Diagram::generateIsNodeOrEdge(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, IsNodeOrEdgeGenerator());
}

QString Diagram::generatePossibleEdges(const QString &lineTemplate) const
{
	return generateListMethod(lineTemplate, PossibleEdgesGenerator());
}

QString Diagram::generateListMethod(const QString &lineTemplate, const ListMethodGenerator &generator) const
{
	QString result;
	bool isFirstLine = true;
	foreach(Type* type, mTypes) {
		QString line = generator.generate(type, lineTemplate);
		if (line.isEmpty())
			continue;
		if (!isFirstLine)
			line.replace("if ", "else if ");
		isFirstLine = false;
		result += line + endline;
	}
	if (result.isEmpty())
		return "	Q_UNUSED(element)" + endline;
	return result;
}

QString Diagram::generateEnums(const QString &lineTemplate) const
{
	EnumsGenerator generator;
	QString result;
	bool isFirstLine = true;

	foreach (EnumType *type, mEditor->getAllEnumTypes()) {
		QString line = generator.generate(type, lineTemplate);
		if (line.isEmpty())
			continue;
		if (!isFirstLine)
			line.replace("if ", "else if ");
		isFirstLine = false;
		result += line + endline;
	}

	if (result.isEmpty())
		return "	Q_UNUSED(name);";

	return result;
}

QString Diagram::generateNodeClasses(const QString &nodeTemplate) const
{
	return generateMapMethod(nodeTemplate, NodesGenerator());
}

QString Diagram::generateEdgeClasses(const QString &edgeTemplate) const
{
	return generateMapMethod(edgeTemplate, EdgesGenerator());
}

QString Diagram::generateResourceFile(const QString &resourceTemplate) const
{
	return generateMapMethod(resourceTemplate, ResourceGenerator());
}
