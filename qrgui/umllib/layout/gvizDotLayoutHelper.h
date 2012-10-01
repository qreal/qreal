#pragma once

#include "abstractLayoutHelper.h"

namespace Ui {
class gvizDotSettingsForm;
}

namespace qReal {

class GvizDotLayoutHelper : public AbstractLayoutHelper
{
	Q_OBJECT
	
	
public:
	GvizDotLayoutHelper();
	virtual ~GvizDotLayoutHelper();

	virtual QMap<Graph::VertexId, QPointF> arrange(Graph const &graph
			, QMap<Graph::VertexId, QRectF> const &graphGeometry);

	virtual QString name() const;
	virtual QWidget *settingsPage() const;

private:
	enum RankDirValues
	{
		TB = 0
		, BT
		, LR
		, RL
	};
	
	QWidget *mSettingsPage;
	Ui::gvizDotSettingsForm *mSettingsUi;
};

}
