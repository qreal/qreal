#include "miniMap.h"

MiniMap::MiniMap(QGraphicsScene *scene, QWidget *parent)
	: QGraphicsView(scene, parent)
{}

MiniMap::MiniMap(QWidget *parent)
	: QGraphicsView(parent)
{}

void MiniMap::init(qReal::MainWindow *window)
{
	mWindow = window;

	setRenderHint(QPainter::Antialiasing, true);

	setInteractive(false);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MiniMap::changeSource(int index)
{
	if (index != -1) {
		setCurrentScene();
	} else {
		setScene(NULL);
	}
}

void MiniMap::setCurrentScene()
{
	EditorView *editorView = mWindow->getCurrentTab();
	if (editorView == NULL) {
		return;
	}

	EditorViewScene *editorViewScene = static_cast<EditorViewScene *>(editorView->scene());
	if (editorViewScene->mainWindow() != NULL) {
		setScene(editorViewScene);
		// can affect zoom - need to change it if we make another desision about it
		connect(editorViewScene, SIGNAL(changed(QList<QRectF>)), this, SLOT(ensureVisible(QList<QRectF>)));
	}
}

void MiniMap::setScene(QGraphicsScene *scene)
{
	QGraphicsView::setScene(scene);
	adjustToItems();
	redrawEditorViewRect();
}

void MiniMap::adjustToItems()
{
	resetTransform();
	fitInView(scene()->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void MiniMap::ensureVisible(QList<QRectF> region)
{
	foreach (QRectF rect, region) {
		fitInView(rect, Qt::KeepAspectRatio);
	}
}

void MiniMap::redrawEditorViewRect()
{

}
