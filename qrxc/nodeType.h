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
	virtual bool generateEnumValues(utils::OutFile &/*out*/, bool /*isNotFirst*/) { return false; }

private:
	QList<Port*> mPorts;
	QDomElement mSdfDomElement;
	QDomElement mPortsDomElement;
	bool mIsPin;
	bool mIsHavePin;
	bool mIsResizeable;

	virtual bool initAssociations();
	virtual bool initGraphics();
	virtual bool initDividability();

	bool hasLinePorts();
	bool hasPointPorts();
	bool initSdf();
	void generateSdf() const;

	bool initPorts();
	bool initPointPorts(QDomElement const &portsElement);
	bool initLinePorts(QDomElement const &portsElement);
	virtual bool initLabel(Label *label, QDomElement const &element, int const &count);
	bool initBooleanProperties();

	void generatePorts() const;
	void generateLinePorts(QDomElement const &portsElement, utils::OutFile &out) const;
	void generatePointPorts(QDomElement const &portsElement, utils::OutFile &out) const;
};
