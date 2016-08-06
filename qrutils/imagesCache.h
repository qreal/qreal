/* Copyright 2015 CyberTech Labs Ltd.
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

#include <QtGui/QPixmap>

#include <qrutils/utilsDeclSpec.h>

class QSvgRenderer;
class QFileInfo;

namespace utils {

/// Cache for images that contains them pre-loaded and parsed and is able to quickly draw it on a painter.
/// Pixmaps and svg images are contained separately as they are rendered differently.
class QRUTILS_EXPORT ImagesCache
{
public:
	static ImagesCache &instance();

	/// Draws image with given file name with a given painter in given rectangle. Note that actual file, from which
	/// an image will be loaded may be different from fileName, as described in selectBestImageFile.
	/// @see selectBestImageFile
	void drawImage(const QString &fileName, QPainter &painter, const QRect &rect, qreal zoom);

	/// Draws image with given file name with a given painter in given rectangle. Note that actual file, from which
	/// an image will be loaded may be different from fileName, as described in selectBestImageFile.
	/// Images of previous sizes and zooms are not memorized.
	void drawImageWithoutCachingSize(const QString &fileName, QPainter &painter, const QRect &rect, qreal zoom);

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

}

inline uint qHash(const QRect &rect)
{
	return qHash(rect.width()) ^ qHash(rect.height()) ^ qHash(rect.top()) ^ qHash(rect.left());
}
