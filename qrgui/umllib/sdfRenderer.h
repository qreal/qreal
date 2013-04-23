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
#include <QtCore/QDebug>

#include "../editorPluginInterface/sdfRendererInterface.h"
#include "../../qrkernel/settingsManager.h"

class SdfRenderer : public SdfRendererInterface
{
	Q_OBJECT

public:
	SdfRenderer();
	SdfRenderer(QString const path);
	SdfRenderer(QDomDocument const &document);
	~SdfRenderer();

	bool load(QString const &filename);
	void load(QDomDocument const &document);
	void render(QPainter *mPainter, const QRectF &bounds);
	void noScale();

	int pictureWidth() const { return mFirstSizeX; }
	int pictureHeight() const { return mFirstSizeY; }

private:
	void initWorkingDir();
	void initFirstSizes();

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
	qreal x1_def(QDomElement &element);
	qreal y1_def(QDomElement &element);
	qreal x2_def(QDomElement &element);
	qreal y2_def(QDomElement &element);
	qreal coord_def(QDomElement &element, QString const &coordName
		, int current_size, int first_size);
	void logger(QString const &path, QString const &string);
	/** @brief checks that str[i] is not L, C, M or Z*/
	bool isNotLCMZ(QString const &str, int i);

	QString mWorkingDirName;
	QMap<QString, QPixmap> mMapFileImage;
	int mFirstSizeX;
	int mFirstSizeY;
	int mCurrentSizeX;
	int mCurrentSizeY;
	int mStartX;
	int mStartY;
	int i;
	int j;
	QPainter *mPainter;
	QPen mPen;
	QBrush mBrush;
	QString mS1;
	QString mS2;
	QFont mFont;
	QFile mLog;
	QTextStream mLogText;
	QDomDocument mDoc;
	/** @brief is false if we don't need to scale according to absolute
	 * coords, is useful for rendering icons. default is true
	**/
	bool mNeedScale;

};

class SdfIconEngineV2: public SdfIconEngineV2Interface
{
public:
	SdfIconEngineV2(QString const &file);
	QSize preferedSize() const;

protected:
	virtual void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state);
	virtual QIconEngine *clone() const
	{
		return NULL;
	}
private:
	SdfRenderer mRenderer;
	QSize mSize;
};
