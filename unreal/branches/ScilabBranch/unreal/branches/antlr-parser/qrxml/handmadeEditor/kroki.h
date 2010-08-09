#pragma once

#include "../../qrgui/umllib/uml_nodeelement.h"

#include <QDebug>

namespace UML {

	class Kroki : public NodeElement {
	public:
		Kroki();
		virtual ~Kroki() {}
		void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget);
		void updateData();
	private:
		QString mText;
		SdfRenderer mRenderer;
	};
}
