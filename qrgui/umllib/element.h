#pragma once

#include <QtWidgets/QGraphicsProxyWidget>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QAction>
#include <QtWidgets/QGraphicsSceneMouseEvent>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>

#include "editorPluginInterface/elementRepoInterface.h"
#include "umllib/label.h"

#include "umllib/contextMenuAction.h"
#include "editorPluginInterface/elementImpl.h"

#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"

#include "controller/controller.h"

namespace qReal {

/// size of a point port
const int kvadratik = 10;

/// base class for an element on a diagram
class Element : public QGraphicsProxyWidget, public ElementRepoInterface
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	/// Constructor
	/// @param elementImpl - pointer to implementation of the element. Takes ownership.
	Element(ElementImpl *elementImpl
			, Id const &id
			, models::GraphicalModelAssistApi &graphicalAssistApi
			, models::LogicalModelAssistApi &logicalAssistApi
			);

	virtual ~Element() {}

	virtual void setId(qReal::Id &id);

	void initEmbeddedControls();

	virtual void updateData();

	virtual Id id() const;
	virtual Id logicalId() const;
	virtual QString name() const;

	virtual void connectToPort() {}  // for edge
	virtual void checkConnectionsToPort() {}  // for node
	virtual QList<ContextMenuAction *> contextMenuActions(QPointF const &pos);

	virtual bool initPossibleEdges() = 0;
	virtual void initTitles();
	// for inline editing we should be able to change properties value. right now via graphical
	// representation. also labels could store indices and get data themselves

	virtual void setLogicalProperty(QString const &roleName, QString const &value
			, bool withUndoRedo = false);
	QVariant logicalProperty(QString const &roleName) const;

	virtual void setColorRect(bool bl) = 0;

	// TODO: Move this to constructor.
	void setController(qReal::Controller *controller);
	qReal::Controller *controller() const;

	ElementImpl* elementImpl() const;
	bool createChildrenFromMenu() const;

public slots:
	virtual void singleSelectionState(bool const singleSelected);
	virtual void selectionState(bool const selected);
	void setTitlesVisible(bool visible);

signals:
	void switchFolding(bool);

protected:
	virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void initTitlesBy(QRectF const& contents);
	/// Sets titles visibility without state registering
	void setTitlesVisiblePrivate(bool visible);

	bool mMoving;
	Id const mId;
	ElementImpl * const mElementImpl;  // Has ownership.
	QList<Label *> mLabels;
	bool mTitlesVisible;

	models::LogicalModelAssistApi &mLogicalAssistApi;
	models::GraphicalModelAssistApi &mGraphicalAssistApi;
	Controller *mController;
};

}
