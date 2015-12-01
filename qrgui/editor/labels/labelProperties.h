/* Copyright 2015 QReal Research Group, Dmitry Mordvinov
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

#include <QtCore/QObject>
#include <QtGui/QColor>

namespace qReal {
namespace gui {
namespace editor {

/// A container for scene label properties (view and behavioural).
/// Defines properties via Qt reflection so they can be used in property editor for dynamic Label
/// properties modification. Also this is a useful trait for easy definition of concrete GraphicalPartModels.
class LabelProperties : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int index READ index WRITE setIndex NOTIFY indexChanged)
	Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
	Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
	Q_PROPERTY(QString binding READ binding WRITE setBinding NOTIFY bindingChanged)
	Q_PROPERTY(bool isReadOnly READ isReadOnly WRITE setReadOnly NOTIFY readOnlyChanged)
	Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
	Q_PROPERTY(QColor background READ background WRITE setBackground NOTIFY backgroundChanged)
	Q_PROPERTY(bool scalingX READ scalingX WRITE setScalingX NOTIFY scalingXChanged)
	Q_PROPERTY(bool scalingY READ scalingY WRITE setScalingY NOTIFY scalingYChanged)
	Q_PROPERTY(bool isHard READ isHard WRITE setHard NOTIFY isHardChanged)
	Q_PROPERTY(QString prefix READ prefix WRITE setPrefix NOTIFY prefixChanged)
	Q_PROPERTY(QString siffix READ siffix WRITE setSuffix NOTIFY suffixChanged)

public:
	LabelProperties();
	LabelProperties(int index, qreal x, qreal y, const QString &text, qreal rotation);
	LabelProperties(int index, qreal x, qreal y, const QString &binding, bool isReadOnly, qreal rotation);
	LabelProperties(const LabelProperties &other);

	/// Returns label`s index among other siblings.
	int index() const;

	/// Sets label`s index among other siblings.
	void setIndex(int index);

	/// Returns label`s scene x-coordinate of the label relatively to parent element.
	qreal x() const;

	/// Sets label`s scene x-coordinate of the label relatively to parent element.
	void setX(qreal x);

	/// Returns label`s scene y-coordinate of the label relatively to parent element.
	qreal y() const;

	/// Sets label`s scene y-coordinate of the label relatively to parent element.
	void setY(qreal y);

	/// Returns true if this label has static text. Otherwise returns false (if label`s text
	/// binded to some property in repository).
	bool isStatic() const;

	/// Returns the static text of the label.
	QString text() const;

	/// Sets the static text of the label.
	void setText(const QString &text);

	/// Returns the property name in repository that will be synced with label text.
	QString binding() const;

	/// Sets the property name in repository that will be synced with label text.
	void setBinding(const QString &binding);

	/// Returns immutability of the label`s value by user.
	bool isReadOnly() const;

	/// Sets immutability of the label`s value by user.
	void setReadOnly(bool isReadOnly);

	/// Returns an angle of the label in degrees realatively to parent`s transform.
	qreal rotation() const;

	/// Sets an angle of the label in degrees realatively to parent`s transform.
	void setRotation(qreal degrees);

	/// Returns the background color of the label.
	QColor background() const;

	/// Sets the background color of the label.
	void setBackground(const QColor &background);

	/// Returns an ability of the label to float for saving x-coordinate ratio with parent.
	bool scalingX() const;

	/// Enabled or disables an ability of the label to float for saving x-coordinate ratio with parent.
	void setScalingX(bool enabled);

	/// Returns an ability of the label to float for saving y-coordinate ratio with parent.
	bool scalingY() const;

	/// Enabled or disables an ability of the label to float for saving y-coordinate ratio with parent.
	void setScalingY(bool enabled);

	/// Returns a property of the label to stay on the scene even if parent element is not selected.
	bool isHard() const;

	/// Enabled or disables a property of the label to stay on the scene even if parent element is not selected.
	void setHard(bool hard);

	/// Returns text drawn just before label contents.
	QString prefix() const;

	/// Sets text drawn just before label contents.
	void setPrefix(const QString &text);

	/// Returns text drawn just after label contents.
	QString siffix() const;

	/// Sets text drawn just after label contents.
	void setSuffix(const QString &text);

signals:
	/// Emitted when label`s index among other siblings changes.
	void indexChanged(int index);

	/// Emitted when label`s scene x-coordinate of the label relatively to parent element changes.
	void xChanged(qreal value);

	/// Emitted when label`s scene y-coordinate of the label relatively to parent element changes.
	void yChanged(qreal value);

	/// Emitted when the static text of the label changes.
	void textChanged(const QString &value);

	/// Emitted when the property name in repository that will be synced with label text changes.
	void bindingChanged(const QString &value);

	/// Emitted when immutability of the label`s value by user changes.
	void readOnlyChanged(bool value);

	/// Emitted when an angle of the label in degrees realatively to parent`s transform changes.
	void rotationChanged(qreal value);

	/// Emitted when the background color of the label changes.
	void backgroundChanged(const QColor &value);

	/// Emitted when an ability of the label to float for saving x-coordinate ratio with parent changes.
	void scalingXChanged(bool value);

	/// Emitted when an ability of the label to float for saving y-coordinate ratio with parent changes.
	void scalingYChanged(bool value);

	/// Emitted when a property of the label to stay on the scene even if parent element is not selected changes.
	void isHardChanged(bool value);

	/// Emitted when text drawn just before label contents changes.
	void prefixChanged(const QString &text);

	/// Emitted when text drawn just after label contents changes.
	void suffixChanged(const QString &text);

private:
	int mIndex;
	qreal mX;
	qreal mY;
	QString mText;
	QString mBinding;
	bool mReadOnly;
	qreal mRotation;
	QColor mBackground;
	bool mScalingX;
	bool mScalingY;
	bool mIsHard;
	QString mPrefix;
	QString mSuffix;
};

}
}
}
