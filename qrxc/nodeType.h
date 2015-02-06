#pragma once

#include <QtCore/QList>

#include "graphicType.h"
#include "port.h"

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
	virtual bool generatePorts(utils::OutFile &out, bool isNotFirst);
	bool copyPorts(NodeType *port) override;
	bool copyPictures(GraphicType *parent) override;

private:
	QList<Port*> mPorts;
	QDomElement mSdfDomElement;
	QDomElement mPortsDomElement;
	bool mIsResizeable;

	virtual bool initAssociations();
	virtual bool initGraphics();
	virtual bool initDividability();
	virtual bool initPortTypes();

	bool initSdf();
	void generateSdf() const;

	bool initPorts();
	bool initPointPorts(QDomElement const &portsElement);
	bool initLinePorts(QDomElement const &portsElement);
	virtual bool initLabel(Label *label, QDomElement const &element, const int &count);
	bool initBooleanProperties();
};
