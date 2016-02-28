/* Copyright 2007-2016 QReal Research Group
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

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

#include <metaMetaModel/sdfRendererInterface.h>
#include <metaMetaModel/elementRepoInterface.h>
#include "plugins/pluginManager/pluginsManagerDeclSpec.h"

#include "pluginsManagerDeclSpec.h"

namespace qReal {

class QRGUI_PLUGINS_MANAGER_EXPORT SdfRenderer : public SdfRendererInterface
{
	Q_OBJECT

public:
	SdfRenderer();
	explicit SdfRenderer(const QString path);
	~SdfRenderer();

	bool load (const QString &filename);
	bool load(const QDomDocument &document);
	void render(QPainter *painter, const QRectF &bounds, bool isIcon = false);
	void noScale();

	int pictureWidth() { return first_size_x; }
	int pictureHeight() { return first_size_y; }

	void setElementRepo(ElementRepoInterface *elementRepo);

public slots:
	/// Sets current zoom in editor to render images in more suitable resolution.
	void setZoom(qreal zoomFactor);

private:

	/// Cache for images that contains them pre-loaded and parsed and is able to quickly draw it on a painter.
	/// Pixmaps and svg images are contained separately as they are rendered differently.
	class ImagesCache
	{
	public:
		static ImagesCache &instance();

		/// Draws image with given file name on given painter in given rectangle. Note that actual file, from which
		/// an image will be loaded may be different from fileName, as described in selectBestImageFile.
		/// @see selectBestImageFile
		void drawImage(const QString &fileName, QPainter &painter, const QRect &rect, qreal zoom);

	private:
		ImagesCache();
		~ImagesCache();

		/// Selects "best available" image file, using following rules:
		/// - if there is .svg file with given name in a directory from filePath, it is used as actual image file.
		/// - else if there is a file with other extension but with correct name, it is used.
		/// - else, if there is no such file, it tries to select a file with name "default" in given directory, using
		///   the rules above.
		/// - if everything above fails, system default image file, from qrgui/icons (or, when compiled,
		///   from ":/icons/default.svg"), is used.
		static QFileInfo selectBestImageFile(const QString &filePath);

		/// Loads pixmap from given file, returns empty QByteArray if file does not exist.
		static QByteArray loadPixmap(const QFileInfo &fileInfo);

		/// Maps file name to pre-loaded pixmap with image.
		QHash<QString, QPixmap> mFileNamePixmapMap;

		/// Maps file name to a svg renderer object.
		QHash<QString, QSharedPointer<QSvgRenderer>> mFileNameSvgRendererMap;

		/// Maps file name to pixmaps with pre-rendered svg images.
		QHash<QString, QHash<QRect, QPixmap>> mPrerenderedSvgs;
	};

	QString mWorkingDirName;

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
	qreal mZoom = 1.0;
	ElementRepoInterface *mElementRepo;

	bool checkShowConditions(const QDomElement &element, bool isIcon) const;
	bool checkCondition(const QDomElement &condition) const;

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
	explicit SdfIconEngineV2(const QString &file);
	explicit SdfIconEngineV2(const QDomDocument &document);
	QSize preferedSize() const;
	virtual void paint(QPainter *painter, const QRect &rect, QIcon::Mode mode, QIcon::State state);
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
	static QIcon iconOf(const QString &fileName);

	/// Returns a size of the pixmap of element in specified sdf-file
	static QSize preferedSizeOf(const QString &fileName);

private:
	static SdfIconLoader *instance();
	static QIcon loadPixmap(const QString &fileName);

	SdfIconLoader();
	~SdfIconLoader();

	QMap<QString, QIcon> mLoadedIcons;
	QMap<QString, QSize> mPreferedSizes;
};

}

inline uint qHash(const QRect &rect)
{
	return qHash(rect.width()) ^ qHash(rect.height()) ^ qHash(rect.top()) ^ qHash(rect.left());
}
