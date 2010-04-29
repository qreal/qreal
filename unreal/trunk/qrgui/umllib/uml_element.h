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
#include "elementImpl.h"

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

		// Для инлайн-редактирования, мы должны иметь возможность менять проперти объекта, пока через его графическое представление.
		// Можно сделать, чтобы сами текстовые лейблы хранили индекс модели и делали всё сами.
		void setRoleValueByName(QString const &roleName, QString const &value);
	protected:
		/** @brief Индекс элемента в модели */
		QPersistentModelIndex mDataIndex;

		/** @brief Идентификатор элемента */
		qReal::Id mUuid;

		/** @brief Индикатор перемещения элемента */
		bool mMoving;

		QList<ElementTitle*> mTitles;

		int roleIndexByName(QString const &roleName) const;
		QString roleValueByName(QString const &roleName) const;
		ElementImpl* elementImpl;
	};
}
