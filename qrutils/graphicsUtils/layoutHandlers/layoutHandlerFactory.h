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
	explicit LayoutHandlerFactory(QGraphicsWidget *layoutHost /* Accepts ownership on this */);
	virtual ~LayoutHandlerFactory() {}

	LayoutHandler *handler(LayoutType type) const;

	LayoutHandler *currentHandler() const;
	LayoutType currentType() const;
	bool hasLayout() const;

	void handleDragEnter(QGraphicsLayoutItem const *draggedItem, QPointF const &position);
	void handleDragMove(QGraphicsLayoutItem const *draggedItem, QPointF const &position);
	void handleDragLeave();
	void handleDropEvent(QGraphicsLayoutItem *draggedItem, QPointF const &position);

	/// All chilren added by this layout manager after this method call
	/// will have specified policy
	void forceChindrenSizePolicy(QSizePolicy::Policy horizontalPolicy
			, QSizePolicy::Policy verticalPolicy);

	void setOuterMargin(int left, int top, int right, int bottom);
	void setLayoutMargin(int left, int top, int right, int bottom);

public slots:
	virtual void setType(LayoutType type);

private:
	void enableCurrentHandler(bool enable);

	LayoutType mCurrentType;
	LinearLayoutHandler *mVerticalHandler;
	LinearLayoutHandler *mHorizontalHandler;
	LayoutHandler *mGridHandler;

	bool mHasPreferences;
	QSizePolicy::Policy mVerticalPolicy;
	QSizePolicy::Policy mHorizontalPolicy;
};

}
}
