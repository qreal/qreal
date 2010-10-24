#pragma once
#include <QtCore/QString>
#include <QtGui/QGraphicsTextItem>

namespace UML {

	class ElementTitle;

	class ElementTitleFactoryInterface
	{
	public:
		virtual ElementTitle *createTitle(int x, int y, QString const &text) = 0;
		virtual ElementTitle *createTitle(int x, int y, QString const &binding, bool readOnly) = 0;
	};

	class ElementTitleInterface : public QGraphicsTextItem
	{
	public:
		virtual void setBackground(Qt::GlobalColor const &background) = 0;
	};
}
