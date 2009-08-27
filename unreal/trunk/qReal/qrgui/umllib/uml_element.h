/** @file uml_element.h
 * 	@brief Базовый класс элемента на диаграмме
 * */
#pragma once

#include <QtGui/QGraphicsItem>
#include <QtCore/QModelIndex>

#include "../kernel/definitions.h"

namespace UML {
	/** @class Element
	* 	@brief Базовый класс элемента на диаграмме
	 * */
	class Element : public QGraphicsItem
	{
	public:
		Element();
		virtual ~Element(){}
		/** @brief Получить индекс элемента в модели
		 *	@brief @return Индекс элемента
		 * */
		QPersistentModelIndex index() const;

		/** @brief Установить индекс элемента */
		void setIndex(QPersistentModelIndex &index /**< Индекс */);

		/** @brief Обновить данные элемента */
		virtual void updateData();

		/** @brief Получить идентификатор элемента
		 *	@brief @return Идентификатор элемента
		 * */
		qReal::Id uuid() const;

		virtual void connectToPort() { }

	protected:
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex mDataIndex;

		/** @brief Идентификатор элемента */
		qReal::Id mUuid;

		/** @brief Индикатор перемещения элемента */
		bool mMoving;
	};
}
