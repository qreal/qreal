#pragma once

#include "diffScene.h"
#include "detailsWidget/diffDetailsWidget.h"
#include <qrgui/editor/editorView.h>
#include "model/diffModel.h"
#include <qrgui/models/models.h>
#include <qrgui/editor/sceneCustomizer.h>
#include <qrgui/controller/controller.h>

namespace versioning
{
namespace details
{

class DiffView : public EditorView
{
	Q_OBJECT

public:
	DiffView(QWidget *parent
			, versioning::DiffModel *diffModel
			, bool isOldModel
			, Controller &controller
			, SceneCustomizer const &customizer
			, Id const &rootId);
	void setDetailsWidget(DiffDetailsWidget *widget);

public slots:
	void adjustZoom(int zoom);

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

private slots:
	void hintItem(qReal::Id const &graphicalId);
	void unhintItem(qReal::Id const &graphicalId);

private:
	DiffScene mScene;
	EditorViewMViface mMVIface;
	void highlightElements();
	void highlight(qReal::Id const &id);

	DiffModel *mDiffModel;
	bool mIsOldModel;
	models::Models *mModel;
	DiffDetailsWidget *mDetailsWidget;
};

}
}
