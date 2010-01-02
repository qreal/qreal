#pragma once

#include "type.h"

#include <QDomElement>
#include <QStringList>
#include <QList>

class Diagram;
class Label;

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
	QStringList mParents;
	QDomElement mElement;
	Diagram *mDiagram;
	bool mVisible;
	int mWidth;
	int mHeight;
	QList<Label*> mLabels;

private:
	virtual void addKernelParent() = 0;
	bool initParents();
	bool initProperties();
	virtual bool initAssociations() = 0;
	virtual bool initGraphics() = 0;
	bool initLabels();
	bool addProperty(Property *property);
	void generateOneCase(utils::OutFile &out, bool notIsFirst);
};
