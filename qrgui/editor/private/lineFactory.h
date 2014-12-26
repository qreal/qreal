#pragma once

#include "editor/edgeElement.h"

using namespace enums;

namespace qReal {

/// A factory for creation of specific LineHandler's subclasses.
/// It also groups all the methods that must be changed to support a new LineHandler's subclass.
class LineFactory : public QObject
{
	Q_OBJECT
public:
	LineFactory(EdgeElement *edge);

	/// Create LineHandler according to link type
	LineHandler *createHandler(linkShape::LinkShape const type) const;

	/// Create menu for choosing shape type
	QMenu *shapeTypeMenu() const;

	/// @return String representation of shapeType
	static QString shapeToString(linkShape::LinkShape const shapeType);

	/// @return LinkShape represented by string; return linkShape::unset if string is incorrect
	static linkShape::LinkShape stringToShape(QString const &string);

private slots:
	void setSquareLine() const;
	void setBrokenLine() const;
	void setCurveLine() const;

private:
	EdgeElement *mEdge; // Doesn't take ownership
};

}
