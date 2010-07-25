#pragma once

#include "graphicType.h"

#include <QList>

namespace utils {
	class OutFile;
}

class NodeType : public GraphicType
{
public:
	NodeType(Diagram *diagram, qrRepo::RepoApi *api, qReal::Id const id);
	virtual Type* clone() const;
	virtual ~NodeType();

	virtual bool init(QString const &context);

	virtual void print();

	virtual QString generateIsNodeOrEdge(QString const &lineTemplate) const;

private:
	bool mIsPin;
	bool mIsHavePin;
};
