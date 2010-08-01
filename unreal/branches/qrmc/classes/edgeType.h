#pragma once

#include "graphicType.h"

#include <QList>

namespace utils {
	class OutFile;
}

class EdgeType : public GraphicType
{
public:
	EdgeType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const &id);
	virtual Type* clone() const;
	virtual ~EdgeType();

	virtual bool isGraphicalType() const;

	virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const;
	virtual QString generateClass(QString const &classTemplate) const;

	void print();

private:
	QString mBeginType;
	QString mEndType;
	QString mLineType;
};
