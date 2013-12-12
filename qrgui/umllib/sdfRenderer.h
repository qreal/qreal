#pragma once

#include <QtWidgets/QWidget>
#include <QtXml/QDomDocument>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QFont>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtGui/QIconEngine>

#include <qrkernel/settingsManager.h>

#include "editorPluginInterface/sdfRendererInterface.h"
#include "editorPluginInterface/elementRepoInterface.h"

namespace qReal {

class SdfRenderer : public SdfRendererInterface
{
	Q_OBJECT

public:
	SdfRenderer();
	SdfRenderer(QString const path);
	~SdfRenderer();

	bool load (QString const &filename);
	bool load(QDomDocument const &document);
	void render(QPainter *painter, QRectF const &bounds, bool isIcon = false);
	void noScale();

	int pictureWidth() { return first_size_x; }
	int pictureHeight() { return first_size_y; }

	void setElementRepo(ElementRepoInterface *elementRepo);

private:
	QString mWorkingDirName;
	QMap<QString, QPixmap> mMapFileImage;
	QMap<QString, QByteArray> mMapSvgFileImage;
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
	ElementRepoInterface *mElementRepo;

	bool checkShowConditions(QDomElement const &element, bool isIcon) const;
	bool checkCondition(QDomElement const &condition) const;

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
	float coord_def(QDomElement &element, QString coordName, int current_size, int first_size);
	void logger(QString path, QString string);

	/** @brief checks that str[i] is not L, C, M or Z*/
	bool isNotLCMZ(QString str, int i);
};

class SdfIconEngineV2: public SdfIconEngineV2Interface
{
public:
	SdfIconEngineV2(QString const &file);
	SdfIconEngineV2(QDomDocument const &document);
	QSize preferedSize() const;
	virtual void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state);
	virtual QIconEngine *clone() const
	{
		return NULL;
	}
private:
	SdfRenderer mRenderer;
	QSize mSize;
};

}
