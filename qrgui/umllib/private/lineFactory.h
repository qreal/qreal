#pragma once

#include "umllib/edgeElement.h"

using namespace enums;

namespace qReal {

class LineFactory : public QObject
{
	Q_OBJECT
public:
	LineFactory(EdgeElement *edge);

	LineHandler *createHandler(linkShape::LinkShape const type) const;
	QMenu *shapeTypeMenu() const;

	QString shapeToString(linkShape::LinkShape const shapeType) const;
	linkShape::LinkShape stringToShape(QString const &string) const;

private slots:
	void setSquareLine() const;
	void setBrokenLine() const;
	void setCurveLine() const;

private:
	EdgeElement *mEdge; // Doesn't take ownership
};

}
