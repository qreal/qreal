/** @file uml_element.h
 * 	@brief Базовый класс элемента на диаграмме
 * */
#pragma once

#include <QtGui/QGraphicsItem>
#include <QtCore/QModelIndex>

namespace UML {
	/** @class Element
	* 	@brief Базовый класс элемента на диаграмме
	 * */
	class Element : public QGraphicsItem
	{
	public:
		Element();
		/** @brief Получить индекс элемента в модели
		 *	@brief @return Индекс элемента
		 * */
		QPersistentModelIndex index() const
		{
			return dataIndex;
		}
		/** @brief Установить индекс элемента */
		void setIndex(QPersistentModelIndex &index /**< Индекс */);

		/** @brief Обновить данные элемента */
		virtual void updateData();

		virtual void connectToPort() { }

	protected:
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex dataIndex;

		/** @brief Индикатор перемещения элемента */
		bool moving;
	};
}
