#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QAction>
#include <QtCore/QModelIndex>

#include "../kernel/roles.h"
#include "../kernel/NewType.h"
#include "elementRepoInterface.h"
#include "elementTitle.h"

#include "contextMenuAction.h"
#include "elementImpl.h"

namespace UML {
	/** @class Element
	* 	@brief base class for an element on a diagram
	 * */
	class Element : public QObject, public QGraphicsItem, public ElementRepoInterface
	{
		Q_OBJECT
		Q_INTERFACES(QGraphicsItem)
	public:

		Element();
		virtual ~Element(){}
		QPersistentModelIndex index() const;

		void setIndex(QPersistentModelIndex &index);

		virtual void updateData();

                qReal::NewType newType() const;

		virtual void connectToPort() { }
		virtual QList<ContextMenuAction*> contextMenuActions();

		virtual bool initPossibleEdges() = 0;

		// for inline editing we should be able to change properties value. right now via graphical
		// representation. also labels could store indices and get data themselves
		virtual void setRoleValueByName(QString const &roleName, QString const &value);

		virtual void setColorRect(bool bl) = 0;
	protected:
		QPersistentModelIndex mDataIndex;

                qReal::NewType mType;

		bool mMoving;

		QList<ElementTitle*> mTitles;

		int roleIndexByName(QString const &roleName) const;
		QString roleValueByName(QString const &roleName) const;
		ElementImpl* elementImpl;
	};
}
