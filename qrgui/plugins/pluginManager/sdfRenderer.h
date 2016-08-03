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
#include <QtGui/QPainter>
#include <QtGui/QFont>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtCore/QFileInfo>
#include <QtCore/QSharedPointer>
#include <QtGui/QIconEngine>
#include <QtSvg/QSvgRenderer>

#include <qrkernel/settingsManager.h>
#include <metaMetaModel/elementRepoInterface.h>

#include "plugins/pluginManager/pluginsManagerDeclSpec.h"

#include "pluginsManagerDeclSpec.h"

namespace qReal {

class QRGUI_PLUGINS_MANAGER_EXPORT SdfRenderer : public QObject
{
	Q_OBJECT

public:
	SdfRenderer();
	explicit SdfRenderer(const QString path);
	~SdfRenderer();

	bool load (const QString &filename);
	bool load(const QDomDocument &document);
	bool load(const QDomElement &picture);
	void render(QPainter *painter, const QRectF &bounds, bool isIcon = false);
	void noScale();

	int pictureWidth() { return first_size_x; }
	int pictureHeight() { return first_size_y; }

	void setElementRepo(ElementRepoInterface *elementRepo);

public slots:
	/// Sets current zoom in editor to render images in more suitable resolution.
	void setZoom(qreal zoomFactor);

private:
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
class SdfIconEngineV2 : public QIconEngine
{
public:
	explicit SdfIconEngineV2(const QString &file);
	explicit SdfIconEngineV2(const QDomDocument &document);
	explicit SdfIconEngineV2(const QDomElement &picture);
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
	/// Returns a pixmap of element in specified sdf-description. Descriptions are cached by id.
	static QIcon iconOf(const Id &id, const QDomElement &sdf);

	/// Returns a size of the pixmap of element in specified sdf-description. Descriptions are cached by id.
	static QSize preferedSizeOf(const Id &id, const QDomElement &sdf);

private:
	static SdfIconLoader *instance();
	static QIcon loadPixmap(const Id &id, const QDomElement &sdf);

	SdfIconLoader();
	~SdfIconLoader();

	QMap<Id, QIcon> mLoadedIcons;
	QMap<Id, QSize> mPreferedSizes;
};

}
