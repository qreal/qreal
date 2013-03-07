#pragma once

#include <QtCore/QObject>

#include "layoutTypes.h"
#include "linearLayoutHandler.h"
#include "../../utilsDeclSpec.h"

namespace qReal
{
namespace layouts
{

class QRUTILS_EXPORT LayoutHandlerFactory : public QObject
{
	Q_OBJECT

public:
	LayoutHandlerFactory(QGraphicsWidget *layoutHost /* Accepts ownership on this */);

	LayoutHandler *handler(LayoutType type) const;

	LayoutHandler *currentHandler() const;
	LayoutType currentType() const;
	bool hasLayout() const;

	void handleDragEnter(QGraphicsLayoutItem const *draggedItem, QPointF const &position);
	void handleDragMove(QGraphicsLayoutItem const *draggedItem, QPointF const &position);
	void handleDragLeave();
	void handleDropEvent(QGraphicsLayoutItem *draggedItem, QPointF const &position);

public slots:
	virtual void setType(LayoutType type);

private:
	void enableCurrentHandler(bool enable);

	LayoutType mCurrentType;
	LinearLayoutHandler *mVerticalHandler;
	LinearLayoutHandler *mHorizontalHandler;
	LayoutHandler *mGridHandler;
};

}
}
