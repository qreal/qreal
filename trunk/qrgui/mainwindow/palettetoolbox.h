#ifndef PALETTETOOLBOX_H
#define PALETTETOOLBOX_H

#include <QtCore/QMap>
#include <QtGui/QToolBox>

class QDragEnterEvent;
class QDropEvent;

class PaletteToolbox : public QToolBox
{
	public:
		PaletteToolbox(QWidget *parent=0);

		void addDiagramType( QString id, QString name );
		void addItemType( QString id, QString name, QIcon icon );

	protected:
		void dragEnterEvent(QDragEnterEvent *event);
		void dropEvent(QDropEvent *event);
		void mousePressEvent(QMouseEvent *event);
	private:
		QMap<QString, int> categories;

		class DraggableElement : public QWidget {
			public:
				DraggableElement(QString id, QString name,
						QIcon icon, QWidget *parent=0);
				QIcon icon()
				{ return m_icon; }
				QString text()
				{ return m_text; }
				QString id()
				{ return m_id; }
			private:
				QString m_id;
				QIcon m_icon;
				QString m_text;
		};

};

#endif
