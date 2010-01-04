#pragma once

#include <QDomElement>
#include <QStringList>
#include <QList>

#include "type.h"

class Label;
class Diagram;

class GraphicType : public Type
{
public:
	GraphicType(Diagram *diagram);
	virtual ~GraphicType();
	virtual bool init(QDomElement const &element);
	virtual bool resolve();
	virtual void generateNameMapping(utils::OutFile &out);
	virtual bool generateObjectRequestString(utils::OutFile &out, bool notIsFirst);
	virtual bool generateProperties(utils::OutFile &out, bool notIsFirst);

protected:
	QDomElement mLogic;
	QDomElement mGraphics;
	QStringList mParents;
	QDomElement mElement;
	bool mVisible;
	int mWidth;
	int mHeight;
	QList<Label*> mLabels;

	void copyFields(GraphicType *type) const;
	QString resourceName(QString const &resourceType) const;

private:
	bool initParents();
	bool initProperties();
	virtual bool initAssociations() = 0;
	virtual bool initGraphics() = 0;
	bool initLabels();
	bool addProperty(Property *property);
	void generateOneCase(utils::OutFile &out, bool notIsFirst);
};
