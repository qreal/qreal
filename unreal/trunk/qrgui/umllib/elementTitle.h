#pragma once

#include <QtGui/QGraphicsTextItem>

namespace UML {

	class ElementTitle : public QGraphicsTextItem
	{
		Q_OBJECT
	public:
		ElementTitle(int x, int y, QString const &text);
		ElementTitle(int x, int y, QString const &binding, bool readOnly);
		~ElementTitle() {}
		void setNeededTextInteractionFlags();
	protected:
		virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
		virtual void focusInEvent(QFocusEvent *event);
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void keyPressEvent(QKeyEvent *event);

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
	private:
		bool mFocusIn;
		bool mReadOnly;
		QString mOldText;
		QString mBinding;
	};

}
