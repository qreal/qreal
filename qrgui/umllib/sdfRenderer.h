#pragma once

#include <QtWidgets/QWidget>
#include <QtXml/QDomDocument>
#include <QtGui/QPen>
#include <QtGui/QBrush>
#include <QtGui/QPainter>
#include <QtGui/QFont>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QHash>
#include <QtCore/QSharedPointer>
#include <QtGui/QIconEngine>
#include <QtSvg/QSvgRenderer>

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

	/// Cache for images that contains them pre-loaded and parsed and is able to quickly draw it on a painter.
	/// Pixmaps and svg images are contained separately as they are rendered differently.
	class ImagesCache {
	public:
		/// Draws image with given file name on given painter in given rectangle. Note that actual file, from which
		/// an image will be loaded may be different from fileName, as described in selectBestImageFile.
		/// @see selectBestImageFile
		void drawImage(QString const &fileName, QPainter &painter, QRect const &rect);

	private:
		/// Selects "best available" image file, using following rules:
		/// - if there is .svg file with given name in a directory from filePath, it is used as actual image file.
		/// - else if there is a file with other extension but with correct name, it is used.
		/// - else, if there is no such file, it tries to select a file with name "default" in given directory, using the
		///   rules above.
		/// - if everything above fails, system default image file, from qrgui/icons (or, when compiled,
		///   from ":/icons/default.svg"), is used.
		static QFileInfo selectBestImageFile(QString const &filePath);

		/// Loads pixmap from given file, returns empty QByteArray if file does not exist.
		static QByteArray loadPixmap(QFileInfo const &fileInfo);

		/// Maps file name to pre-loaded pixmap with image.
		QHash<QString, QPixmap> mFileNamePixmapMap;

		/// Maps file name to a svg renderer object.
		QHash<QString, QSharedPointer<QSvgRenderer>> mFileNameSvgRendererMap;
	};

	QString mWorkingDirName;

	/// Smart cache for images, to avoid loading image from disc on every paint() call.
	ImagesCache mImagesCache;

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

	/// checks that str[i] is not L, C, M or Z
	/// @todo Not so helpful comment
	bool isNotLCMZ(QString str, int i);
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
