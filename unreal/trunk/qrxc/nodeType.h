#pragma once

#include "graphicType.h"

#include <QList>

class Port;

namespace utils {
	class OutFile;
}

class NodeType : public GraphicType
{
public:
	NodeType(Diagram *diagram);
	virtual Type* clone() const;
	virtual ~NodeType();
	virtual void generateCode(utils::OutFile &out);

private:
	QList<Port*> mPorts;

	virtual void addKernelParent();
	virtual bool initAssociations();
	virtual bool initGraphics();
	bool initSdf();
	bool initPorts();
	bool initLinePorts(QDomElement const &portsElement, utils::OutFile &out);
	bool initPointPorts(QDomElement const &portsElement, utils::OutFile &out);
};
