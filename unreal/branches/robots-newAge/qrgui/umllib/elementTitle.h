#pragma once

#include <QtGui/QGraphicsTextItem>
#include "../pluginInterface/elementTitleHelpers.h"

namespace UML {

	class ElementTitle;

	class ElementTitleFactory : public ElementTitleFactoryInterface
	{
	public:
		ElementTitleInterface *createTitle(int x, int y, QString const &text);
		ElementTitleInterface *createTitle(int x, int y, QString const &binding, bool readOnly);
	};

	class ElementTitle : public ElementTitleInterface
	{
		Q_OBJECT
	public:
		ElementTitle(int x, int y, QString const &text);
		ElementTitle(int x, int y, QString const &binding, bool readOnly);
		void setBackground(Qt::GlobalColor const &background);
		~ElementTitle() {}
		void startTextInteraction();

	protected:
		virtual void focusOutEvent(QFocusEvent *event);
		virtual void keyPressEvent(QKeyEvent *event);

		virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = NULL);
	private:
		bool mFocusIn;
		bool mReadOnly;
		QString mOldText;
		QString mBinding;
		Qt::GlobalColor mBackground;
	};

}
