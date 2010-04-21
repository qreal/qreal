/** @file propertyeditorproxymodel.h
 *	@brief Модель редактора свойств
 * */
#pragma once

#include <QAbstractTableModel>
#include <QtCore/QStringList>

#include "editormanager.h"

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
						int role = Qt::DisplayRole /**< Роль */
						) const;
	/** @brief Получить данные модели
	 *	@brief @return Данные модели
	 * */
	QVariant data(const QModelIndex &index, /**< Индекс элемента*/
					int role = Qt::DisplayRole /**< Роль */
					) const;

	/** @brief Сохранить данные в модели
	 *	@brief @return Успешность выполнения операции
	 * */
	bool setData(const QModelIndex &index, /**< Индекс элемента*/
					const QVariant &value, /**< Значение */
					int role = Qt::EditRole /**< Роль */
					);

	/** @brief Установить укзатель на основую модель*/
	void setSourceModel(QAbstractItemModel *sourceModel /**< Модель */);

public slots:
	/** @brief Установить индекс элемента */
	void setIndex(const QModelIndex &sourceIndex /**< Индекс элемента*/);

private slots:
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
