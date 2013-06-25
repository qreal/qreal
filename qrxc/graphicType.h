#pragma once

#include <QDomElement>
#include <QStringList>
#include <QList>
#include <QVector>
#include <QPair>

#include "type.h"

class Label;
class Diagram;

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
	virtual bool generateContainedTypes(utils::OutFile &out, bool isNotFirst);
	virtual bool generateConnections(utils::OutFile &out, bool isNotFirst);
	virtual bool generateUsages(utils::OutFile &out, bool isNotFirst);
	virtual bool generatePossibleEdges(utils::OutFile &out, bool isNotFirst);
	virtual void generatePropertyTypes(utils::OutFile &out);
	virtual void generatePropertyDefaults(utils::OutFile &out);
	virtual void generateMouseGesturesMap(utils::OutFile &out);
	virtual void generateParentsMapping(utils::OutFile &out);
	virtual void generateExplosionsMap(utils::OutFile &out);

	QString description() const;
	void setDescription(QString const &description);

protected:
	typedef QPair<QPair<QString,QString>,QPair<bool,QString> > PossibleEdge; // Lol

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
	QStringList mConnections;
	QStringList mUsages;
	QList<PossibleEdge> mPossibleEdges;
	QStringList mBonusContextMenuFields;
	QMap<QString, QPair<bool, bool> > mExplosions;

	void copyFields(GraphicType *type) const;
	QString resourceName(QString const &resourceType) const;
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

	bool initLabels();
	bool initUsages();
	bool initParents();
	bool initBonusContextMenuFields();
	bool initProperties();
	bool initContainers();
	bool initContainerProperties();
	bool initConnections();
	bool initPossibleEdges();
	bool initExplosions();
	bool initTypeList(QString const &listName, QString const &listElementName
		, QStringList &resultingList) const;

	bool initFieldList(QString const &listName, QString const &listElementName
		, QStringList &resultingList, QString const &fieldName, bool const isNeedToNormalizeAtt) const;

	virtual bool initGraphics() = 0;
	virtual bool initAssociations() = 0;
	virtual bool initDividability() = 0;
	virtual bool initLabel(Label *label, QDomElement const &element, int const &count) = 0;

	bool addProperty(Property *property);
	void generateOneCase(utils::OutFile &out, bool isNotFirst) const;
	bool generateListForElement(utils::OutFile &out, bool isNotFirst, QStringList const &list) const;

	QVector<int> toIntVector(QString const &s, bool * isOk) const;

	QString mDescription;
};
