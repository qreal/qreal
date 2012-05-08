#include "abstractLayoutHelper.h"

#ifndef GVIZDOTLAYOUTHELPER_H
#define GVIZDOTLAYOUTHELPER_H

namespace Ui {
class gvizDotSettingsForm;
}

class gvizDotLayoutHelper : public AbstractLayoutHelper
{
public:
	gvizDotLayoutHelper();

	virtual QMap<Graph::VertexId, QPointF> arrange(Graph const &graph
			, QMap<Graph::VertexId, QRectF> const &graphGeometry);

	virtual QString name() const { return "graphViz Dot"; }
	virtual QWidget* settingsPage() const;

private:
	QWidget *mSettingsPage;
	Ui::gvizDotSettingsForm *mSettingsUi;
};

#endif // GVIZDOTLAYOUTHELPER_H
