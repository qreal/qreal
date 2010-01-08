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
	QDomElement mSdfDomElement;
	QDomElement mPortsDomElement;

	virtual bool initAssociations();
	virtual bool initGraphics();
	bool initSdf();
	void generateSdf() const;

	bool initPorts();
	bool initPointPorts(QDomElement const &portsElement);
	bool initLinePorts(QDomElement const &portsElement);

	void generatePorts() const;
	void generateLinePorts(QDomElement const &portsElement, utils::OutFile &out) const;
	void generatePointPorts(QDomElement const &portsElement, utils::OutFile &out) const;
};
