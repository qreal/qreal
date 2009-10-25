#pragma once

#include "graphicType.h"

#include <QList>

class Port;
class OutFile;

class NodeType : public GraphicType
{
public:
	NodeType(Diagram *diagram);
	virtual ~NodeType();
	virtual void generateCode(OutFile &out);

private:
	QList<Port*> mPorts;

	virtual void addKernelParent();
	virtual bool initAssociations();
	virtual bool initGraphics();
	bool initSdf();
	bool initPorts();
	bool initLinePorts(QDomElement const &portsElement, OutFile &out);
	bool initPointPorts(QDomElement const &portsElement, OutFile &out);
};
