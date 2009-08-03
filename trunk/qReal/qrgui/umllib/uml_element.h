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
		qReal::IdType uuid() const;

		virtual void connectToPort() { }

		virtual int type() const {return UserType + 1;}
	protected:
		//       void mousePressEvent ( QGraphicsSceneMouseEvent * event );
		//       void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex dataIndex;

		/** @brief Идентификатор элемента */
		qReal::IdType m_uuid;
		/** @brief Тип элемента */

		/** @brief Индикатор перемещения элемента */
		bool moving;
	};
}
