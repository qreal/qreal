/* Copyright 2016 CyberTech Labs Ltd.
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

#include "twoDModel/engine/model/image.h"

#include <QtGui/QPainter>

#include <qrutils/imagesCache.h>

using namespace twoDModel::model;

Image::Image()
	: mExternal(true)
{
}

Image::Image(const QString &path, bool memorize)
	: mExternal(!memorize)
	, mPath(path)
	, mImage(path)
{
}

Image Image::deserialize(const QDomElement &element)
{
	const bool external = element.attribute("external") == "true";
	const QString path = element.attribute("path");
	Image image(path, !external);
	if (!external) {
		const QByteArray bytes = QByteArray::fromBase64(element.text().toLatin1());
		QDataStream stream(bytes);
		stream >> image.mImage;
	}

	return image;
}

void Image::serialize(QDomElement &target) const
{
	if (isLoaded()) {
		target.setAttribute("path", mPath);
		target.setAttribute("external", mExternal ? "true" : "false");
		if (!mExternal) {
			QByteArray bytes;
			QDataStream stream(bytes);
			stream << mImage;
			const QDomText text = target.ownerDocument().createTextNode(bytes.toBase64());
			target.appendChild(text);
		}
	}
}

bool Image::isLoaded() const
{
	return !mImage.isNull();
}

QSize Image::preferedSize() const
{
	return mImage.size();
}

bool Image::external() const
{
	return mExternal;
}

void Image::setExternal(bool external)
{
	mExternal = external;
}

void Image::draw(QPainter &painter, const QRect &rect, qreal zoom)
{
	if (mExternal && !mPath.isEmpty()) {
		utils::ImagesCache::instance().drawImageWithoutCachingSize(mPath, painter, rect, zoom);
	} else if (!mImage.isNull()) {
		painter.drawImage(rect, mImage);
	}
}

bool Image::operator==(const Image &other) const
{
	return other.mPath == mPath;
}

bool Image::operator!=(const Image &other) const
{
	return !(other == *this);
}
