#pragma once

#include <QtCore/QString>
#include <QtCore/QList>

#include "../../qrgui/view/gestures/GeometricForms.h"
#include "../../qrkernel/ids.h"

namespace qReal {
namespace gestures {

class GesturesPainterInterface
{
public:
	/// Returns the id of current selected for drawing element
	virtual qReal::Id currentElement() const = 0;
	/// Draws specified gesture
	virtual void draw(PathVector const &paths) = 0;
	/// Prepares given elements set for gestures drawing
	/// @param elements A list of elements to prepare. First item is
	/// element`s frienly name, second is its id
	virtual void setElements(QList<QPair<QString, Id> > const &elements) = 0;
};

}
}
