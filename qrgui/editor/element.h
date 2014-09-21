#pragma once

#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QAction>

#include <qrkernel/ids.h>
#include <qrkernel/settingsManager.h>

#include "plugins/editorPluginInterface/elementRepoInterface.h"
#include "plugins/editorPluginInterface/elementImpl.h"

#include "editor/label.h"
#include "editor/contextMenuAction.h"

#include "models/graphicalModelAssistApi.h"
#include "models/logicalModelAssistApi.h"

#include "controller/controller.h"
#include "models/exploser.h"

namespace qReal {

/// size of a point port
const int kvadratik = 10;

/// base class for an element on a diagram
class Element : public QObject, public QGraphicsItem, public ElementRepoInterface
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

	void initEmbeddedControls();

	virtual void updateData();

	virtual Id id() const;
	virtual Id logicalId() const;
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

	// TODO: Move this to constructor.
	void setController(qReal::Controller *controller);
	qReal::Controller *controller() const;

	ElementImpl* elementImpl() const;
	bool createChildrenFromMenu() const;

	/// Checks if this element is disabled from palette and if it is grayscales it.
	void updateEnabledState();

public slots:
	virtual void select(bool const singleSelected);
	virtual void setSelectionState(bool const selected);
	void setTitlesVisible(bool visible);

signals:
	void switchFolding(bool);

protected:
	void initTitlesBy(QRectF const& contents);
	/// Sets titles visibility without state registering
	void setTitlesVisiblePrivate(bool visible);

	void keyPressEvent(QKeyEvent *event) override;

	bool mMoving;
	bool mEnabled;
	Id const mId;
	ElementImpl * const mElementImpl;  // Has ownership.
	QList<Label *> mLabels;
	bool mTitlesVisible;

	models::LogicalModelAssistApi &mLogicalAssistApi;
	models::GraphicalModelAssistApi &mGraphicalAssistApi;
	Controller *mController;
};

}
