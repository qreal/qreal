#pragma once

#include <QtCore/QHash>
#include <QtGui/QWidget>
#include <QtGui/QIcon>

#include "../kernel/ids.h"

class QVBoxLayout;
class QComboBox;
class QScrollArea;
class QDragEnterEvent;
class QDropEvent;

namespace qReal {

    namespace gui {

        class PaletteToolbox : public QWidget
        {
            Q_OBJECT

        public:
            explicit PaletteToolbox(QWidget *parent = NULL);
            ~PaletteToolbox();

            void addDiagramType(Id const &id, QString const &name);
            void addItemType(Id const &id, QString const &name, QIcon const &icon);
            void initDone();

        public Q_SLOTS:
            /** @brief Сделать данный редактор активным */
            void setActiveEditor(int const editorIndex);

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
            /** @brief Массив содержимого редакторов */
            QVector<QWidget*> mTabs;
            /** @brief Массив имен редакторов */
            QVector<QString> mTabNames;
            /** @brief Основной layout */
            QVBoxLayout *mLayout;
            /** @brief Выпадающий список видимых редаторов */
            QComboBox *mComboBox;
            /** @brief Содержимое активного редактора */
            QScrollArea *mScrollArea;
        };

    }

}
