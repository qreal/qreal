/** @file uml_element.h
 * 	@brief Базовый класс элемента на диаграмме
 * */
#ifndef UML_ELEMENT_H
#define UML_ELEMENT_H

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
		};
		/** @brief Установить индекс элемента */
		void setIndex(QPersistentModelIndex &index /**< Индекс */);

		/** @brief Обновить данные элемента */
		virtual void updateData();

		/** @brief Получить идентификатор элемента
		 *	@brief @return Идентификатор элемента
		 * */
		int uuid() const;
		/** @brief Получить тип элемента
		 *	@brief @return Тип элемента
		 * */
		int type() const;

		virtual void connectToPort() { }

	protected:
		//       void mousePressEvent ( QGraphicsSceneMouseEvent * event );
		//       void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex dataIndex;

		/** @brief Идентификатор элемента */
		int m_uuid;
		/** @brief Тип элемента */
		int m_type;

		/** @brief Индикатор перемещения элемента */
		bool moving;
	};
};

#endif
