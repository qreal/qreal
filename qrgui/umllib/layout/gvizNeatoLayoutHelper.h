#pragma once

#include "abstractLayoutHelper.h"

namespace Ui {
class gvizNeatoSettingsForm;
}

namespace qReal {

class GvizNeatoLayoutHelper : public AbstractLayoutHelper
{
	Q_OBJECT

public:
	GvizNeatoLayoutHelper();
	virtual ~GvizNeatoLayoutHelper();

	virtual QMap<Graph::VertexId, QPointF> arrange(Graph const &graph
			, QMap<Graph::VertexId, QRectF> const &graphGeometry);

	virtual QString name() const;
	virtual QWidget *settingsPage() const;

private:
	QWidget *mSettingsPage;
	Ui::gvizNeatoSettingsForm *mSettingsUi;
};

}
