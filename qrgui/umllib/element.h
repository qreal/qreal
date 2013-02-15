#pragma once

#include <QtGui/QGraphicsProxyWidget>
#include <QtGui/QGraphicsItem>
#include <QtGui/QAction>

#include "../../qrkernel/ids.h"
#include "../../qrkernel/settingsManager.h"

#include "../editorPluginInterface/elementRepoInterface.h"
#include "elementTitle.h"

#include "contextMenuAction.h"
#include "../editorPluginInterface/elementImpl.h"

#include "../models/graphicalModelAssistApi.h"
#include "../models/logicalModelAssistApi.h"

/** @brief size of a point port */
const int kvadratik = 10;

/**
 * @brief base class for an element on a diagram
 */
class Element : public QGraphicsProxyWidget, public ElementRepoInterface
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	Element(ElementImpl* elementImpl);
	
	virtual ~Element() {}

	virtual void setId(qReal::Id &id);

	void initEmbeddedControls();

	virtual void updateData();

	virtual qReal::Id id() const;
	virtual qReal::Id logicalId() const;
	virtual QString name() const;

	virtual void connectToPort() {}  // for edge
	virtual void checkConnectionsToPort() {}  // for node
	virtual QList<ContextMenuAction *> contextMenuActions(QPointF const &pos);

	virtual bool initPossibleEdges() = 0;
	virtual void initTitles();
	// for inline editing we should be able to change properties value. right now via graphical
	// representation. also labels could store indices and get data themselves
	virtual void setLogicalProperty(QString const &roleName, QVariant const &value);
	QVariant logicalProperty(QString const &roleName) const;

	virtual void setColorRect(bool bl) = 0;

	virtual void setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi
			, qReal::models::LogicalModelAssistApi *logicalAssistApi);

	ElementImpl* elementImpl() const;
	/// Perform element-specific actions before being deleted
	virtual void deleteFromScene() = 0;

public slots:
	virtual void singleSelectionState(bool const singleSelected);
	virtual void selectionState(bool const selected);

signals:
	void switchFolding(bool);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void initTitlesBy(QRectF const& contents);

	bool mMoving;
	qReal::Id mId;
	ElementImpl* const mElementImpl;
	QList<ElementTitle *> mTitles;

	qReal::models::LogicalModelAssistApi *mLogicalAssistApi;
	qReal::models::GraphicalModelAssistApi *mGraphicalAssistApi;
};
