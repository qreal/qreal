#pragma once

#include "diffscene.h"
#include "detailsWidget/diffdetailswidget.h"
#include "../../view/editorview.h"
#include "../details/diffprovider.h"
#include "../../mainwindow/mainwindow.h"
#include "../../models/models.h"

namespace qReal
{
namespace diffManager
{
namespace diffView
{

class DiffView : public EditorView
{
	Q_OBJECT

public:
	DiffView(qReal::MainWindow *mainWindow, details::DiffProvider *diffProvider, bool isOldModel, QWidget *parent = 0);
	void setDetailsWidget(detailsWidget::DiffDetailsWidget *widget);

protected:
	virtual void mousePressEvent(QMouseEvent *event);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dragLeaveEvent(QDragLeaveEvent *event);
	virtual void dragMoveEvent(QDragMoveEvent *event);
	virtual void dropEvent(QDropEvent *event);
	virtual void keyPressEvent(QKeyEvent *event);
	virtual void keyReleaseEvent(QKeyEvent *event);
	virtual void mouseDoubleClickEvent(QMouseEvent *event);
	virtual void mouseMoveEvent(QMouseEvent *event);
	virtual void mouseReleaseEvent(QMouseEvent *event);

	virtual void onClick(qReal::Id const &id);
	virtual void onClickCancel();

private:
	details::DiffProvider *mDiffProvider;
	bool mIsOldModel;
	models::Models *mModel;
	detailsWidget::DiffDetailsWidget *mDetailsWidget;

	DiffScene *diffScene();
	void highlightElements();
	void highlight(qReal::Id const &id);

private slots:
	void hintItem(qReal::Id const &graphicalId);
	void unhintItem(qReal::Id const &graphicalId);
};

}
}
}
