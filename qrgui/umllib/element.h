#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QAction>

#include "../../qrkernel/ids.h"
#include "../../qrkernel/settingsManager.h"

#include "../editorPluginInterface/elementRepoInterface.h"
#include "label.h"

#include "contextMenuAction.h"
#include "../editorPluginInterface/elementImpl.h"

#include "../models/graphicalModelAssistApi.h"
#include "../models/logicalModelAssistApi.h"

#include "../controller/controller.h"

namespace qReal {

/// size of a point port
const int kvadratik = 10;

/// base class for an element on a diagram
class Element : public QObject, public QGraphicsItem, public ElementRepoInterface
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsItem)

public:
	Element(ElementImpl* elementImpl);

	virtual ~Element() {}

	void setId(qReal::Id &id);

	void initEmbeddedControls();

	virtual void updateData();

	virtual qReal::Id id() const;
	virtual qReal::Id logicalId() const;
	virtual QString name() const;

	virtual void connectToPort() {}  // for edge
	virtual void checkConnectionsToPort() {}  // for node
	virtual QList<ContextMenuAction *> contextMenuActions(const QPointF &pos);

	virtual bool initPossibleEdges() = 0;
	virtual void initTitles();
	// for inline editing we should be able to change properties value. right now via graphical
	// representation. also labels could store indices and get data themselves
	virtual void setLogicalProperty(QString const &roleName, QString const &value
			, bool withUndoRedo = false);
	QString logicalProperty(QString const &roleName) const;

	virtual void setColorRect(bool bl) = 0;

	virtual void setAssistApi(qReal::models::GraphicalModelAssistApi *graphicalAssistApi
			, qReal::models::LogicalModelAssistApi *logicalAssistApi);

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
	void initTitlesBy(QRectF const& contents);
	/// Sets titles visibility without state registering
	void setTitlesVisiblePrivate(bool visible);

	bool mMoving;
	qReal::Id mId;
	ElementImpl* const mElementImpl;
	QList<Label *> mLabels;
	bool mTitlesVisible;

	qReal::models::LogicalModelAssistApi *mLogicalAssistApi;
	qReal::models::GraphicalModelAssistApi *mGraphicalAssistApi;
	qReal::Controller *mController;
};

}
