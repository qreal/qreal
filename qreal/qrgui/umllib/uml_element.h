#pragma once

#include <QtGui/QGraphicsItem>
#include <QtGui/QAction>

#include "../kernel/ids.h"
#include "../pluginInterface/elementRepoInterface.h"
#include "elementTitle.h"

#include "contextMenuAction.h"
#include "../pluginInterface/elementImpl.h"

#include "../models/graphicalModelAssistApi.h"
#include "../models/logicalModelAssistApi.h"

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

		void setId(qReal::Id &id);

		virtual void updateData();

		virtual qReal::Id id() const;
		virtual qReal::Id logicalId() const;
		virtual QString name() const;

		virtual void connectToPort() {};//for edge
		virtual void checkConnectionsToPort() {};//for node
		virtual QList<ContextMenuAction*> contextMenuActions();

		virtual bool initPossibleEdges() = 0;
		virtual void initTitles();
		// for inline editing we should be able to change properties value. right now via graphical
		// representation. also labels could store indices and get data themselves
		virtual void setLogicalProperty(QString const &roleName, QString const &value);

		virtual void setColorRect(bool bl) = 0;

		void setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi, qReal::models::LogicalModelAssistApi *logicalAssistApi);

	protected:
		qReal::Id mId;

		bool mMoving;

		QList<ElementTitle*> mTitles;

		ElementImpl* mElementImpl;

		qReal::models::GraphicalModelAssistApi *mGraphicalAssistApi;
		qReal::models::LogicalModelAssistApi *mLogicalAssistApi;

		QString logicalProperty(QString const &roleName) const;
		void initTitlesBy(QRectF const& contents);
	};
}
