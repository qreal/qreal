#pragma once

#include <QtCore/QHash>
#include <QtGui/QToolBox>

#include "../kernel/ids.h"

class QDragEnterEvent;
class QDropEvent;

namespace qReal {

	namespace gui {

		class PaletteToolbox : public QToolBox
		{
		public:
			explicit PaletteToolbox(QWidget *parent = NULL);

			void addDiagramType(Id const &id, QString const &name);
			void addItemType(Id const &id, QString const &name, QIcon const &icon);

		private:
			class DraggableElement : public QWidget {
			public:
				DraggableElement(Id const &id, QString const &name,
								 QIcon const &icon, QWidget *parent = NULL);

				QIcon icon() const
				{
					return mIcon;
				}

				QString text() const
				{
					return mText;
				}

				Id id() const
				{
					return mId;
				}

			private:
				Id mId;
				QIcon mIcon;
				QString mText;
			};

			virtual void dragEnterEvent(QDragEnterEvent *event);
			virtual void dropEvent(QDropEvent *event);
			virtual void mousePressEvent(QMouseEvent *event);

			QHash<Id, int> mCategories;
		};

	}

}
