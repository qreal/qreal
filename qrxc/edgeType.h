#pragma once

#include <QtCore/QList>
#include <QtGui/QColor>

#include "graphicType.h"

class Association;
namespace utils {
	class OutFile;
}

class EdgeType : public GraphicType
{
public:
	EdgeType(Diagram *diagram);
	virtual Type* clone() const;
	virtual ~EdgeType();
	virtual void generateCode(utils::OutFile &out);
	virtual bool generateEnumValues(utils::OutFile &/*out*/, bool /*isNotFirst*/) { return false; }

private:
	virtual bool initAssociations();
	virtual bool initGraphics();
	virtual bool initDividability();
	void generateGraphics() const;
	void generateEdgeStyle(QString const &styleString, utils::OutFile &out);
	virtual bool initLabel(Label *label, QDomElement const &element, int const &count);
	virtual bool isWidgetBased(QDomElement const &graphics) const;

	QList<Association*> mAssociations;
	QString mBeginType;
	QString mEndType;
	QString mLineType;
	QColor mLineColor;
	int mLineWidth;
	QString mIsDividable;
};
