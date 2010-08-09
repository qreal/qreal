/** @file editorviewscene.h
 * 	@brief Сцена для отрисовки объектов
 * */
#pragma once

#include <QGraphicsScene>

#include "../kernel/definitions.h"
#include "../umllib/uml_nodeelement.h"


namespace qReal {
    class EditorViewMViface;
    class EditorView;
    class MainWindow;
}

/** @class EditorViewScene
 *	@brief Сцена для отрисовки объектов
 * */
class EditorViewScene : public QGraphicsScene
{
    Q_OBJECT

public:
    /** @brief Конструктор  */
    explicit EditorViewScene(QObject *parent = 0 /**< � одительский объект*/);

    /** @brief Очистить сцену */
    void clearScene();


    virtual bool launchEdgeMenu(UML::EdgeElement *edge, QPointF scenePos);
    virtual qReal::Id *createElement(const QString &, QPointF scenePos);
    virtual void createElement(const QMimeData *mimeData, QPointF scenePos);

    /** @brief Получить элемент сцены по его идентификатору
     *	@brief @return Элемент сцены
     * */
    virtual UML::Element *getElem(qReal::Id const &uuid);  // Функция виртуальная только для того, чтобы обмануть линкер.
        // Она используется из плагинов редакторов, а включать в них еще и сцену (и всё, что тянет сцена) неохота.
        // Определение адреса виртуальной функции происходит во время выполнения, так что сцену можно не компилить.
    /** @brief Получить элемент сцены по его индексу в модели
     *	@brief @return
     * */
    virtual UML::Element *getElemByModelIndex(const QModelIndex& index /**< Индекс элемента в модели */);

    virtual QPersistentModelIndex rootItem();
    void setMainWindow(qReal::MainWindow *mainWindow);
    qReal::MainWindow *mainWindow() const;
    void setEnabled(bool enabled);

    bool canBeContainedBy(qReal::Id container, qReal::Id candidate);

protected:
    /** @brief Обработать начало события drag'n'drop */
    void dragEnterEvent( QGraphicsSceneDragDropEvent *event /**< Событие */ );
    /** @brief Обработать перемещение элемента при drag'n'drop */
    void dragMoveEvent( QGraphicsSceneDragDropEvent *event /**< Событие */);
    /** @brief Обработать завершение события drag'n'drop */
    void dragLeaveEvent( QGraphicsSceneDragDropEvent *event /**< Событие */);
    /** @brief Обработать событие drag'n'drop */
    void dropEvent ( QGraphicsSceneDragDropEvent *event /**< Событие */);

    /** @brief Обработать событие нажатия клавиши */
    void keyPressEvent( QKeyEvent *event /**< Событие */);

    /** @brief Обработать событие нажатия кнопок мыши */
    void mousePressEvent( QGraphicsSceneMouseEvent *event /**< Событие */);
    void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );

    /** @brief Обработать двойной щелчок мыши */
    void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event /**< Событие */);
private:

    UML::Element *getElemAt(const QPointF &position);

    /** @brief Новое положение элемента */
    QPointF newElementsPosition;

    /** @brief Объект, реализующий интерфейс представления в схеме Model/View */
    qReal::EditorViewMViface *mv_iface;
    /** @brief Объект, реализующей представление в схеме Model/View */
    qReal::EditorView *view;

    qReal::MainWindow *mWindow;

    QPointF mPrevPosition;
    QGraphicsItem *mPrevParent;

    QPointF mCreatePoint;

    friend class qReal::EditorViewMViface;

public Q_SLOTS:
    qReal::Id *createElement(const QString &);
// TODO: Убрать отсюда.
private Q_SLOTS:
    void connectActionTriggered();
    void goToActionTriggered();
    void disconnectActionTriggered();
};
