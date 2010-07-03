#pragma once

#include <QDomElement>
#include <QStringList>
#include <QList>
#include <QPair>

#include "type.h"

class Label;
class Diagram;

class GraphicType : public Type
{
public:
	GraphicType(Diagram *diagram);
	virtual ~GraphicType();
	virtual bool init(QDomElement const &element, QString const &context);
	virtual bool resolve();
	virtual void generateNameMapping(utils::OutFile &out);
	virtual bool generateObjectRequestString(utils::OutFile &out, bool isNotFirst);
	virtual bool generateProperties(utils::OutFile &out, bool isNotFirst);
	virtual bool generateContainedTypes(utils::OutFile &out, bool isNotFirst);
	virtual bool generateConnections(utils::OutFile &out, bool isNotFirst);
	virtual bool generateUsages(utils::OutFile &out, bool isNotFirst);
        virtual bool generatePossibleEdges(utils::OutFile &out, bool isNotFirst);

protected:
	QDomElement mLogic;
	QDomElement mGraphics;
	QStringList mParents;
	QDomElement mElement;
	bool mVisible;
	int mWidth;
	int mHeight;
	QList<Label*> mLabels;
	QStringList mContains;
	QStringList mConnections;
	QStringList mUsages;
        QList<QPair<QPair<QString,QString>,bool> > mPossibleEdges;

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

	bool initParents();
	bool initProperties();
	bool initContainers();
	bool initConnections();
        bool initPossibleEdges();
	bool initUsages();
	virtual bool initAssociations() = 0;
	virtual bool initGraphics() = 0;
	bool initLabels();
	virtual bool initLabel(Label *label, QDomElement const &element, int const &count) = 0;
	bool addProperty(Property *property);
	void generateOneCase(utils::OutFile &out, bool isNotFirst) const;

	bool generateListForElement(utils::OutFile &out, bool isNotFirst, QStringList const &list) const;

	bool initTypeList(QString const &listName, QString const &listElementName
		, QStringList &resultingList) const;
};
