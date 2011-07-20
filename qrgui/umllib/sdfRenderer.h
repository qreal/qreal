#pragma once

#include <QtGui/QWidget>
#include <QtXml/QDomDocument>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QFont>
#include <QFile>
#include <QTextStream>
#include <QtGui/QIconEngine>
#include <QDebug>
#include "../pluginInterface/sdfRendererInterface.h"
#include "../kernel/settingsManager.h"

class SdfRenderer : public SdfRendererInterface
{
	Q_OBJECT

public:
	SdfRenderer();
	SdfRenderer(const QString path);
	~SdfRenderer();

	bool load (const QString &filename);
	void render(QPainter *painter, const QRectF &bounds);
	void noScale();

	int pictureWidth() { return first_size_x; }
	int pictureHeight() { return first_size_y; }

private:
	QString mWorkingDirName;
	QMap<QString, QPixmap> mMapFileImage;
	int first_size_x;
	int first_size_y;
	int current_size_x;
	int current_size_y;
	int mStartX;
	int mStartY;
	int i;
	int j;
	int sep;
	QPainter *painter;
	QPen pen;
	QBrush brush;
	QString s1;
	QString s2;
	QFont font;
	QFile log;
	QTextStream logtext;
	QDomDocument doc;

	/** @brief is false if we don't need to scale according to absolute
	 * coords, is useful for rendering icons. default is true
	**/
	bool mNeedScale;

	void line(QDomElement &element);
	void ellipse(QDomElement &element);
	void arc(QDomElement &element);
	void parsestyle(QDomElement &element);
	void background(QDomElement &element);
	void draw_text(QDomElement &element);
	void rectangle(QDomElement &element);
	void polygon(QDomElement &element);
	QPoint *getpoints(QDomElement &element, int n);
	void point (QDomElement &element);
	void defaultstyle();
	void path_draw(QDomElement &element);
	void stylus_draw(QDomElement &element);
	void curve_draw(QDomElement &element);
	void image_draw(QDomElement &element);
	float x1_def(QDomElement &element);
	float y1_def(QDomElement &element);
	float x2_def(QDomElement &element);
	float y2_def(QDomElement &element);
	float coord_def(QDomElement &element, QString coordName, int current_size,
		int first_size);
	void logger(QString path, QString string);

	/** @brief checks that str[i] is not L, C, M or Z*/
	bool isNotLCMZ(QString str, int i);
};

class SdfIconEngineV2: public SdfIconEngineV2Interface
{
public:
	SdfIconEngineV2(QString const &file);
	virtual void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state);
private:
	SdfRenderer mRenderer;
};
