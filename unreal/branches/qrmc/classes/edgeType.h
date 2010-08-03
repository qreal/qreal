#pragma once

#include "graphicType.h"
#include "label.h"

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
	virtual QString generateNodeClass(QString const &classTemplate) const;
	virtual QString generateEdgeClass(QString const &classTemplate) const;

	void print();

	void initLabels();
private:

	QString mBeginType;
	QString mEndType;
	QString mLineType;
	QList<Label*> mLabels; // refactor after #349 is closed
};
