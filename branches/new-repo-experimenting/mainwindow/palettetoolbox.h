#ifndef PALETTETOOLBOX_H
#define PALETTETOOLBOX_H

#include <QToolBox>

class QDragEnterEvent;
class QDropEvent;

class PaletteToolbox : public QToolBox
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
				DraggableElement(QString className, QWidget *parent=0);
				QIcon icon()
					{ return m_icon; };
				QString text()
					{ return m_text; };
			private:
				QIcon m_icon;
				QString m_text;
		};

};

#endif
