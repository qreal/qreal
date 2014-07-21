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
	GraphicType(Diagram *diagram);
	virtual ~GraphicType();
	virtual bool init(QDomElement const &element, QString const &context);
	virtual bool resolve();
	virtual void generateNameMapping(utils::OutFile &out);
	virtual void generateDescriptionMapping(utils::OutFile &out);
	virtual void generatePropertyDisplayedNamesMapping(utils::OutFile &out);
	virtual void generatePropertyDescriptionMapping(utils::OutFile &out);
	virtual bool generateObjectRequestString(utils::OutFile &out, bool isNotFirst);
	virtual bool generateProperties(utils::OutFile &out, bool isNotFirst, bool isReference);
	virtual bool generatePorts(utils::OutFile &out, bool isNotFirst);
	virtual bool generateContainedTypes(utils::OutFile &out, bool isNotFirst);
	virtual bool generatePossibleEdges(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out);
	virtual void generateMouseGesturesMap(utils::OutFile &out);
	virtual void generateParentsMapping(utils::OutFile &out);
	virtual void generateExplosionsMap(utils::OutFile &out);
	virtual bool copyPorts(NodeType *parent) = 0;
	bool copyLabels(GraphicType *parent);
	virtual bool copyPictures(GraphicType *parent) = 0;

	class Override
	{
	public:
		Override(QString parentString);
		bool valueOverridePorts();
		bool valueOverrideLabels();
		bool valueOverridePictures();

	private:
		QString mOverrideString;
		bool mOverridePorts = false;
		bool mOverrideLabels = false;
		bool mOverridePictures = false;
	};

	QString description() const;
	void setDescription(QString const &description);

protected:
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

	QDomElement mLogic;
	QDomElement mGraphics;
	QStringList mParents;
	QDomElement mElement;
	bool mVisible;
	int mWidth;
	int mHeight;
	QList<Label*> mLabels;
	QStringList mContains;
	ContainerProperties mContainerProperties;
	QList<PossibleEdge> mPossibleEdges;
	QStringList mBonusContextMenuFields;
	QMap<QString, QPair<bool, bool> > mExplosions;
	bool mCreateChildrenFromMenu;
	QString mAbstract;
	void copyFields(GraphicType *type) const;
	QString resourceName(QString const &resourceType) const;
	virtual bool isResolving() const;

	void generateOneCase(utils::OutFile &out, bool isNotFirst) const;

private:
	class ResolvingHelper {
	public:
		ResolvingHelper(bool &resolvingFlag);
		~ResolvingHelper();
	private:
		bool &mResolvingFlag;
	};

	bool mResolving;

	bool initLabels();
	bool initUsages();
	bool initParents();
	bool initBonusContextMenuFields();
	bool initProperties();
	bool initContainers();
	bool initContainerProperties();
	bool initCreateChildrenFromMenu();
	bool initPossibleEdges();
	bool initExplosions();
	bool initTypeList(QString const &listName, QString const &listElementName
		, QStringList &resultingList) const;

	bool initFieldList(QString const &listName, QString const &listElementName
		, QStringList &resultingList, QString const &fieldName, bool const isNeedToNormalizeAtt) const;

	virtual bool initGraphics() = 0;
	virtual bool initAssociations() = 0;
	virtual bool initDividability() = 0;
	virtual bool initPortTypes() = 0;
	virtual bool initLabel(Label *label, QDomElement const &element, int const &count) = 0;

	bool addProperty(Property *property);
	bool addPort(Port *port);
	bool generateListForElement(utils::OutFile &out, bool isNotFirst, QStringList const &list) const;

	QVector<int> toIntVector(QString const &s, bool * isOk) const;

	QString mDescription;
};
