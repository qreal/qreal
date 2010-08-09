/** @file propertyeditorproxymodel.h
 *	@brief Модель редактора свойств
 * */
#pragma once

#include <QAbstractTableModel>
#include <QtCore/QStringList>

#include "../editorManager/editorManager.h"

/** @class PropertyEditorModel
 *	@brief Модель редактора свойств
 * */
class PropertyEditorModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit PropertyEditorModel(qReal::EditorManager const &editorManager,
                                 QObject *parent = 0);

    /** @brief Получить число дочерних элементов
     *	@brief @return Число дочерних элементов
     * */
    int rowCount(const QModelIndex &index /**< Индекс элемента*/) const;
    /** @brief Получить число колонок элемента
     *	@brief @return Получить число колонок элемента
     * */
    int columnCount(const QModelIndex &index /**< Индекс элемента*/) const;

    /** @brief Получить флаги элемента
     *	@brief @return Флаги элемента
     * */
    Qt::ItemFlags flags (const QModelIndex &index /**< Индекс элемента*/) const;

    /** @brief Получить заголовок
     *	@brief @return Заголовок
     * */
    QVariant headerData(int section, /**< Номер секции */
                        Qt::Orientation orientation, /**< Ориентация */
                        int role = Qt::DisplayRole /**< � оль */
                        ) const;
    /** @brief Получить данные модели
     *	@brief @return Данные модели
     * */
    QVariant data(const QModelIndex &index, /**< Индекс элемента*/
                    int role = Qt::DisplayRole /**< � оль */
                    ) const;

    /** @brief Сохранить данные в модели
     *	@brief @return Успешность выполнения операции
     * */
    bool setData(const QModelIndex &index, /**< Индекс элемента*/
                    const QVariant &value, /**< Значение */
                    int role = Qt::EditRole /**< � оль */
                    );

    /** @brief Установить укзатель на основую модель*/
    void setSourceModel(QAbstractItemModel *sourceModel /**< Модель */);

public Q_SLOTS:
    /** @brief Установить индекс элемента */
    void setIndex(const QModelIndex &sourceIndex /**< Индекс элемента*/);

private Q_SLOTS:
    /** @brief Перечитать данные из модели */
    void rereadData();

private:
    /** @brief Основная модель */
    QAbstractItemModel *targetModel;
    /** @brief Индекс объекта */
    QPersistentModelIndex targetObject;

    /** @brief Список названий полей объекта */
    QStringList mFieldNames;

    /** @brief Число псевдоатрибутов, то есть атрибутов, которые не являются пропертями элемента*/
    int mPseudoAttributesCount;

    /** @brief Число псевдоатрибутов, которых можно редактировать*/
    int mEditablePseudoAttributesCount;

    qReal::EditorManager const &mEditorManager;

    int roleByIndex(int const index) const;
};
