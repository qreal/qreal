#pragma once

#include <QtGui/QAction>

namespace UML {

    // Наследник QAction, позволяющий себя "извне" параметризовать координатами
    // события. Полезен для того, чтобы вызывать со сцены обработчики в Element,
    // поскольку информация о положении курсора относительно сцены будет потеряна,
    // если её не сохранить тут.
    class ContextMenuAction: public QAction {
        Q_OBJECT
    public:
        ContextMenuAction(QString const &text, QObject *parent);
        void setEventPos(QPointF const &pos);
        QPointF eventPos() const;
    private Q_SLOTS:
        void trigger();
    Q_SIGNALS:
        void triggered(QPointF const &pos);
    private:
        QPointF mEventPos;
    };

}
