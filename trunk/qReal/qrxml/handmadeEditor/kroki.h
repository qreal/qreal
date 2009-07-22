#pragma once

#include "../../qrgui/umllib/uml_nodeelement.h"

#include <QDebug>

namespace UML {

	class Kroki : public NodeElement {
	public:
		Kroki() {
			mRenderer.load(QString(":/TestEditor/Orthodox/Kroki.sdf"));
			mText = "";
			m_contents.setWidth(100);
			m_contents.setHeight(100);
		}

		virtual ~Kroki() {}

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
		{
			mRenderer.render(painter, m_contents);
			NodeElement::paint(painter, style, widget, NULL);
			d.setTextWidth(m_contents.width() - 15);
		}

		void updateData()
		{
			NodeElement::updateData();
			mText = "";
			update();
		}

	private:
		QString mText;
		SdfRenderer mRenderer;
	};
}
