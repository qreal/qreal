#ifndef PALETTETOOLBOX_H
#define PALETTETOOLBOX_H

#include <QTabWidget>

class QDragEnterEvent;
class QDropEvent;

class PaletteToolbox : public QTabWidget
{
	public:
		PaletteToolbox(QWidget *parent=0);

	protected:
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		void mousePressEvent(QMouseEvent *event);
	private:
		class DraggableElement : public QWidget {
			public:
				DraggableElement(int classid, QWidget *parent=0);
				QIcon icon()
					{ return m_icon; };
				QString text()
					{ return m_text; };
				int id()
					{ return m_id; };
			private:
				int m_id;
				QIcon m_icon;
				QString m_text;
		};

};

#endif
