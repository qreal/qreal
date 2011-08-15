#pragma once

#include <QGraphicsScene>
#include <QGraphicsLineItem>
#include <QSignalMapper>
#include "../kernel/roles.h"
#include "../umllib/uml_nodeelement.h"
#include "gestures/mousemovementmanager.h"

//const int indexGrid = 30; // distance between two lines in the grid

namespace qReal {
	class EditorViewMViface;
	class EditorView;
	class MainWindow;
}

class EditorViewScene : public QGraphicsScene
{
	Q_OBJECT

public:
	explicit EditorViewScene(QObject *parent = 0);
	~EditorViewScene();

	void clearScene();

	virtual int launchEdgeMenu(UML::EdgeElement* edge, UML::NodeElement* node, QPointF scenePos);
	virtual qReal::Id *createElement(const QString &, QPointF scenePos);
	virtual void createElement(const QMimeData *mimeData, QPointF scenePos);

	// is virtual only to trick linker. is used from plugins and generators and we have no intention of
	// including the scene (with dependencies) there
	virtual UML::Element *getElem(qReal::Id const &id);

	virtual qReal::Id rootItemId() const;
	void setMainWindow(qReal::MainWindow *mainWindow);
	qReal::MainWindow *mainWindow() const;
	void setEnabled(bool enabled);

	void setNeedDrawGrid(bool show);
	double realIndexGrid();
	void setRealIndexGrid(double newIndexGrid);

	bool canBeContainedBy(qReal::Id container, qReal::Id candidate);
	bool getNeedDrawGrid();

	UML::Element* getLastCreated();

	void wheelEvent(QGraphicsSceneWheelEvent *wheelEvent);
	
	void highlight(qReal::Id const &graphicalId, bool exclusive = true);
	void dehighlight(qReal::Id const &graphicalId);
	void dehighlight();

//	void initForDiff();
//	void highlight(const qReal::Id &graphicalId, qReal::diffManager::details::DiffState state);

signals:
	void elementCreated(qReal::Id const &id);
	void zoomIn();
	void zoomOut();

protected:
	virtual void dragEnterEvent( QGraphicsSceneDragDropEvent *event);
	virtual void dragMoveEvent( QGraphicsSceneDragDropEvent *event);
	virtual void dragLeaveEvent( QGraphicsSceneDragDropEvent *event);
	virtual void dropEvent ( QGraphicsSceneDragDropEvent *event);

	virtual void keyPressEvent( QKeyEvent *event);

	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent ( QGraphicsSceneMouseEvent * mouseEvent );
	virtual void mouseMoveEvent (QGraphicsSceneMouseEvent *event);

	virtual void mouseDoubleClickEvent( QGraphicsSceneMouseEvent *event);

	virtual void drawBackground( QPainter *painter, const QRectF &rect);

private:
	UML::Element* lastCreatedWithEdge;

	bool mRightButtonPressed;
	bool mNeedDrawGrid; // if true, the grid will be shown (as scene's background)
	qreal mWidthOfGrid;
	double mRealIndexGrid;

	void getObjectByGesture();
	void getLinkByGesture(UML::NodeElement * parent, UML::NodeElement const & child);
	void drawGesture();
	void deleteGesture();
	void createEdgeMenu(QList<QString> const & ids);

	void drawGrid(QPainter *painter, const QRectF &rect);
	void redraw();

	UML::Element *getElemAt(const QPointF &position);

	void createConnectionSubmenus(QMenu &contextMenu, UML::Element const * const element) const;
	void createGoToSubmenu(QMenu * const goToMenu, QString const &name, qReal::IdList const &ids) const;
	void createAddConnectionMenu(UML::Element const * const element
								 , QMenu &contextMenu, QString const &menuName
								 , qReal::IdList const &connectableTypes, qReal::IdList const &alreadyConnectedElements
								 , qReal::IdList const &connectableDiagrams, const char *slot) const;

	void createDisconnectMenu(UML::Element const * const element
							  , QMenu &contextMenu, QString const &menuName
							  , qReal::IdList const &outgoingConnections, qReal::IdList const &incomingConnections
							  , const char *slot) const;

	void initContextMenu(UML::Element *e, QPointF const & pos);

	QPointF newElementsPosition;
	QList<QGraphicsItem*> mGesture;

	qReal::EditorViewMViface *mv_iface;
	qReal::EditorView *view;

	qReal::MainWindow *mWindow;

	QPointF mPrevPosition;
	QGraphicsItem *mPrevParent;

	QPointF mCreatePoint;

	MouseMovementManager * mouseMovementManager;

	QSignalMapper *mActionSignalMapper;
	
	QSet<UML::Element *> mHighlightedElements;

	friend class qReal::EditorViewMViface;

public slots:

	qReal::Id *createElement(const QString &);
	// TODO: get rid of it here
private slots:

	void connectActionTriggered();
	void goToActionTriggered();
	void disconnectActionTriggered();
	void addUsageActionTriggered();
	void deleteUsageActionTriggered();
	void printElementsOfRootDiagram();
	void drawIdealGesture();
	void initMouseMoveManager();
	void createEdge(QString const &);
};
