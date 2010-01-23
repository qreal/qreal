/** @file uml_element.h
 * 	@brief Базовый класс элемента на диаграмме
 * */
#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QAction>
#include <QtCore/QModelIndex>

#include "../kernel/definitions.h"
#include "elementTitle.h"
#include "contextMenuAction.h"

namespace UML {
	/** @class Element
	* 	@brief Базовый класс элемента на диаграмме
	 * */
	class Element : public QObject, public QGraphicsItem
	{
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)
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
		virtual QList<ContextMenuAction*> contextMenuActions();

	protected:
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex mDataIndex;

		/** @brief Идентификатор элемента */
		qReal::Id mUuid;

		/** @brief Индикатор перемещения элемента */
		bool mMoving;

		QList<ElementTitle*> mTitles;
	};
}
