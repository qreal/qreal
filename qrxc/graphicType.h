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

#pragma once

#include <QtXml/QDomElement>
#include <QtCore/QStringList>
#include <QtCore/QList>
#include <QtCore/QVector>
#include <QtCore/QPair>

#include "type.h"
#include "port.h"

class Label;
class Diagram;
class NodeType;

const int maxLineLength = 80;

class GraphicType : public Type
{
public:
	explicit GraphicType(Diagram *diagram);
	~GraphicType() override;
	bool init(const QDomElement &element, const QString &context) override;
	bool resolve() override;

	virtual bool copyPictures(GraphicType *parent) = 0;
	virtual bool copyPorts(NodeType *parent) = 0;
	void copyLabels(GraphicType *parent);

	QString description() const;
	void setDescription(const QString &description);

	/// Returns a list of types generalized by this one.
	QStringList immediateParents() const;

	/// Returns a list of types that instances of this one can contain.
	/// Types are returned without considering of generalizations.
	QStringList containedTypes() const;

	/// Returns mapping of element`s possible explosion targets to explosion properties.
	const QMap<QString, QPair<bool, bool>> &explosions() const;

protected:
	void generateCommonData(utils::OutFile &out) const;
	void generateName(utils::OutFile &out) const;
	void generateFriendlyName(utils::OutFile &out) const;
	void generateDiagram(utils::OutFile &out) const;
	void generateDescription(utils::OutFile &out) const;
	void generateLabels(utils::OutFile &out) const;

	void generatePropertyData(utils::OutFile &out) const;

	QString boolToString(bool value) const;

	/// @todo Remove this sh~.
	typedef QPair<QPair<QString,QString>,QPair<bool,QString> > PossibleEdge;  // Lol

	struct ContainerProperties {
		ContainerProperties();

		bool isSortingContainer;
		QVector<int> sizeOfForestalling;
		int sizeOfChildrenForestalling;
		bool hasMovableChildren;
		bool minimizesToChildren;
		bool maximizesChildren;
	};

	struct GeneralizationProperties {
		GeneralizationProperties(const QString &name, const QString &overrides);
		QString name;
		bool overridePorts = false;
		bool overrideLabels = false;
		bool overridePictures = false;
	};

	QDomElement mLogic;
	QDomElement mGraphics;
	QList<GeneralizationProperties> mParents;
	QDomElement mElement;
	bool mVisible;
	int mWidth;
	int mHeight;
	QList<Label*> mLabels;
	QStringList mContains;
	ContainerProperties mContainerProperties;
	QList<PossibleEdge> mPossibleEdges;
	QMap<QString, QPair<bool, bool> > mExplosions;
	bool mCreateChildrenFromMenu;
	QString mAbstract;
	void copyFields(GraphicType *type) const;
	QString resourceName(const QString &resourceType) const;
	virtual bool isResolving() const;

private:
	class ResolvingHelper {
	public:
		ResolvingHelper(bool &resolvingFlag);
		~ResolvingHelper();
	private:
		bool &mResolvingFlag;
	};

	bool mResolving;

	virtual bool initLabels();
	virtual bool initParents();
	virtual bool initProperties();
	virtual bool initContainers();
	virtual bool initContainerProperties();
	virtual bool initCreateChildrenFromMenu();
	virtual bool initPossibleEdges();
	virtual bool initExplosions();
	virtual bool initTypeList(const QString &listName, const QString &listElementName
		, QStringList &resultingList) const;

	bool initFieldList(const QString &listName, const QString &listElementName
		, QStringList &resultingList, const QString &fieldName, const bool isNeedToNormalizeAtt) const;

	virtual bool initGraphics() = 0;
	virtual bool initAssociations() = 0;
	virtual bool initDividability() = 0;
	virtual bool initPortTypes() = 0;
	virtual bool initLabel(Label *label, const QDomElement &element, const int &count) = 0;

	bool addProperty(Property *property);
	bool addPort(Port *port);
	bool generateListForElement(utils::OutFile &out, bool isNotFirst, const QStringList &list) const;

	QVector<int> toIntVector(const QString &s, bool * isOk) const;

	QString mDescription;
};
