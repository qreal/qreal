#pragma once

#include "../../qrgui/umllib/uml_nodeelement.h"

namespace UML {

	class Kroki : public NodeElement {
	public:
		Kroki() {
			renderer.load(QString(":/TestEditor/Orthodox/Kroki.sdf"));
			text = "";
			height = 100;
			width = 100;
			m_contents.setWidth(width);
			m_contents.setHeight(height);
			d.setFlags(QGraphicsItem::ItemIsSelectable | d.flags());
			d.setTextInteractionFlags(Qt::TextEditorInteraction);
			d.setParentItem(this);
		}

		virtual ~Kroki() {}

		void paint(QPainter *painter, const QStyleOptionGraphicsItem *style, QWidget *widget)
		{
			renderer.render(painter, m_contents);
			d.setTextWidth(m_contents.width() - 15);
		}

		void updateData()
		{
			NodeElement::updateData();
			text = "";
			update();
		}

	private:
		QString text;
		int width;
		int height;
		SdfRenderer renderer;
	};
}
