#pragma once

#include <QtGui/QGraphicsTextItem>
#include "../pluginInterface/elementTitleHelpers.h"

namespace UML {

	class ElementTitle;

	class ElementTitleFactory : public ElementTitleFactoryInterface
	{
	public:
		ElementTitleInterface *createTitle(qreal x, qreal y, QString const &text);
		ElementTitleInterface *createTitle(qreal x, qreal y, QString const &binding, bool readOnly);
	};

	class ElementTitle : public ElementTitleInterface
	{
		Q_OBJECT
	public:
		ElementTitle(qreal x, qreal y, QString const &text);
		ElementTitle(qreal x, qreal y, QString const &binding, bool readOnly);
		void init(QRectF const& contents);
		void setBackground(Qt::GlobalColor const &background);
		void setScaling(bool scalingX, bool scalingY);
		~ElementTitle() {}
		void startTextInteraction();
		void transform(QRectF const& contents);

	protected:
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void keyPressEvent(QKeyEvent *event);

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
	private:
		bool mFocusIn;
		bool mReadOnly;
		bool mScalingX;
		bool mScalingY;
		QRectF mContents;
		QPointF mPoint;
		QString mOldText;
		QString mBinding;
		Qt::GlobalColor mBackground;
	};

}
