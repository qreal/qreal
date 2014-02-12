#pragma once

#include <QtCore/QList>

#include "graphicType.h"

class Port;

namespace utils {
	class OutFile;
}

class NodeType : public GraphicType
{
public:
	explicit NodeType(Diagram *diagram);
	virtual Type* clone() const;
	virtual ~NodeType();
	virtual void generateCode(utils::OutFile &out);
	virtual bool generateEnumValues(utils::OutFile &/*out*/, bool /*isNotFirst*/) { return false; }
	virtual bool generatePorts(utils::OutFile &out, bool isNotFirst) const;

private:
	virtual bool initAssociations();
	virtual bool initGraphics();
	virtual bool initDividability();
	virtual bool initPortTypes();

	bool initSdf();
	void initSizeFromRoot(QDomElement const &root);
	void generateSdf() const;

	bool initPorts();
	bool initPointPorts(QDomElement const &portsElement);
	bool initLinePorts(QDomElement const &portsElement);
	virtual bool initLabel(Label *label, QDomElement const &element, int const &count);
	bool initBooleanProperties();

	void generatePorts() const;
	void generateLinePorts(QDomElement const &portsElement, utils::OutFile &out) const;
	void generatePointPorts(QDomElement const &portsElement, utils::OutFile &out) const;

	bool initIcon();
	void generateIcon();

	virtual bool isWidgetBased(QDomElement const &graphics) const;

	QList<Port*> mPorts;
	QDomElement mSdfDomElement;
	QDomElement mPortsDomElement;
	QDomElement mIconDomElement;
	bool mIsPin;
	bool mIsHavePin;
	bool mIsResizeable;
};
