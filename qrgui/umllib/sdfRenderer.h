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
	SdfRenderer(QDomDocument const &document);
	~SdfRenderer();

	bool load(QString const &filename);
	bool load(QDomDocument const &document);
	void render(QPainter *painter, QRectF const &bounds, bool isIcon = false);
	void noScale();

	int pictureWidth() const { return mFirstSizeX; }
	int pictureHeight() const { return mFirstSizeY; }

	void setElementRepo(ElementRepoInterface *elementRepo);

private:
	void initWorkingDir();
	void initFirstSizes();

	QString mWorkingDirName;
	QMap<QString, QString> mReallyUsedFiles;
	QMap<QString, QByteArray> mMapFileImage;
	int first_size_x;
	int first_size_y;
	int current_size_x;
	int current_size_y;
	int mFirstSizeX;
	int mFirstSizeY;
	int mCurrentSizeX;
	int mCurrentSizeY;
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

	qreal x1_def(QDomElement &element);
	qreal y1_def(QDomElement &element);
	qreal x2_def(QDomElement &element);
	qreal y2_def(QDomElement &element);
	qreal coord_def(QDomElement &element, QString const &coordName
		, int current_size, int first_size);
	void logger(QString const &path, QString const &string);

	/// Reads byte array from the file that is obtained by inner rules from the given one.
	/// Specified file path may be modified with storing into it really read file path.
	QByteArray loadPixmap(QString &filePath);
	QByteArray loadPixmapFromExistingFile(QString &filePath);

	/** @brief checks that str[i] is not L, C, M or Z*/
	bool isNotLCMZ(QString const &str, int i);

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

};

/// Constructs QIcon instance by a given sdf description
class SdfIconEngineV2 : public SdfIconEngineV2Interface
{
public:
	explicit SdfIconEngineV2(QString const &file);
	explicit SdfIconEngineV2(QDomDocument const &document);
	QSize preferedSize() const;
	virtual void paint(QPainter *painter, QRect const &rect, QIcon::Mode mode, QIcon::State state);
	virtual QIconEngine *clone() const;

private:
	SdfRenderer mRenderer;
	QSize mSize;
};

/// Caches sdf-descripted icons
class SdfIconLoader
{
public:
	/// Returns a pixmap of element in specified sdf-file
	static QIcon iconOf(QString const &fileName);

	/// Returns a size of the pixmap of element in specified sdf-file
	static QSize preferedSizeOf(QString const &fileName);

private:
	static SdfIconLoader *instance();
	static QIcon loadPixmap(QString const &fileName);

	SdfIconLoader();
	~SdfIconLoader();

	QMap<QString, QIcon> mLoadedIcons;
	QMap<QString, QSize> mPreferedSizes;
};

}
