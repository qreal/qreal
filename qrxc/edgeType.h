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
	bool copyPorts(NodeType* parent) override;
	bool copyPictures(GraphicType *parent) override;

private:
	QList<Association*> mAssociations;
	QString mBeginType;
	QString mEndType;
	QString mLineType;
	QString mShapeType;
	QColor mLineColor;
	int mLineWidth;
	QString mIsDividable;
	QStringList mFromPorts;
	QStringList mToPorts;

	virtual bool initAssociations();
	virtual bool initGraphics();
	virtual bool initDividability();
	virtual bool initPortTypes();
	void initPortTypes(const QDomElement &portsElement, QStringList &ports);
	void generateGraphics() const;
	void generateEdgeStyle(const QString &styleString, utils::OutFile &out);
	void generatePorts(utils::OutFile &out, const QStringList &portTypes);
	virtual bool initLabel(Label *label, const QDomElement &element, const int &count);
};
