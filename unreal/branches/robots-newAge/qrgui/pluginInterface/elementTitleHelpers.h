#pragma once
#include <QtCore/QString>
#include <QtGui/QGraphicsTextItem>

namespace UML {

	class ElementTitleInterface : public QGraphicsTextItem
	{
	public:
		virtual void setBackground(Qt::GlobalColor const &background) = 0;
	};

	class ElementTitleFactoryInterface
	{
	public:
		virtual ElementTitleInterface *createTitle(int x, int y, QString const &text) = 0;
		virtual ElementTitleInterface *createTitle(int x, int y, QString const &binding, bool readOnly) = 0;
	};

}
