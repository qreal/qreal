#include "abstractLayoutHelper.h"

#ifndef GVIZNEATOLAYOUTHELPER_H
#define GVIZNEATOLAYOUTHELPER_H

namespace Ui {
class gvizNeatoSettingsForm;
}

class gvizNeatoLayoutHelper : /*private QObject, */public AbstractLayoutHelper
{
//	Q_OBJECT

public:
	gvizNeatoLayoutHelper();

	virtual QMap<Graph::VertexId, QPointF> arrange(Graph const &graph
			, QMap<Graph::VertexId, QRectF> const &graphGeometry);

	virtual QString name() const { return "graphViz Neato"; }
	virtual QWidget* settingsPage() const;

private:
	QWidget *mSettingsPage;
	Ui::gvizNeatoSettingsForm *mSettingsUi;

//	QString mMode;
//	int mSep;

//private slots:
//	void configModeChanged(QString);
//	void configSepChanged(int);
};

#endif // GVIZNEATOLAYOUTHELPER_H
