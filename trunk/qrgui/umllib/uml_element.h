/** @file uml_element.h
 * 	@brief Базовый класс элемента на диаграмме
 * */
#pragma once

#include <QtGui/QGraphicsItem>
#include <QtCore/QModelIndex>

#include "../kernel/definitions.h"

using namespace qReal;

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

		/** @brief Получить идентификатор элемента
		 *	@brief @return Идентификатор элемента
		 * */
		IdType uuid() const;

		virtual void connectToPort() { }

	protected:
		//       void mousePressEvent ( QGraphicsSceneMouseEvent * event );
		//       void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex dataIndex;

		/** @brief Идентификатор элемента */
		IdType m_uuid;
		/** @brief Тип элемента */

		/** @brief Индикатор перемещения элемента */
		bool moving;
	};
}
