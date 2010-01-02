#pragma once

#include "graphicType.h"

#include <QList>

class Association;
namespace utils {
	class OutFile;
}

class EdgeType : public GraphicType
{
public:
	EdgeType(Diagram *diagram);
	virtual ~EdgeType();
	virtual void generateCode(utils::OutFile &out);

private:
	QList<Association*> mAssociations;
	QString mBeginType;
	QString mEndType;
	QString mLineType;

	virtual void addKernelParent();
	virtual bool initAssociations();
	virtual bool initGraphics();
	void generateEdgeStyle(QString const &styleString, utils::OutFile &out);
};
