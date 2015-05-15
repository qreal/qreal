#include "labelProperties.h"

using namespace qReal;

LabelProperties::LabelProperties()
	: mIndex(0)
	, mX(0)
	, mY(0)
	, mText()
	, mBinding()
	, mReadOnly(true)
	, mRotation(0)
	, mBackground(Qt::transparent)
	, mScalingX(false)
	, mScalingY(false)
	, mIsHard(false)
{
}

LabelProperties::LabelProperties(int index, qreal x, qreal y, const QString &text, qreal rotation)
	: mIndex(index)
	, mX(x)
	, mY(y)
	, mText(text)
	, mBinding()
	, mReadOnly(true)
	, mRotation(rotation)
	, mBackground(Qt::transparent)
	, mScalingX(false)
	, mScalingY(false)
	, mIsHard(false)
{
}

LabelProperties::LabelProperties(int index, qreal x, qreal y, const QString &binding, bool readOnly, qreal rotation)
	: mIndex(index)
	, mX(x)
	, mY(y)
	, mText()
	, mBinding(binding)
	, mReadOnly(readOnly)
	, mRotation(rotation)
	, mBackground(Qt::transparent)
	, mScalingX(false)
	, mScalingY(false)
	, mIsHard(false)
{
}

LabelProperties::LabelProperties(const LabelProperties &other)
{
	this->mIndex = other.mIndex;
	this->mX = other.mX;
	this->mY = other.mY;
	this->mText = other.mText;
	this->mBinding = other.mBinding;
	this->mReadOnly = other.mReadOnly;
	this->mRotation = other.mRotation;
	this->mBackground = other.mBackground;
	this->mScalingX = other.mScalingX;
	this->mScalingY = other.mScalingY;
	this->mIsHard = other.mIsHard;
}

int LabelProperties::index() const
{
	return mIndex;
}

void LabelProperties::setIndex(int index)
{
	if (index != mIndex) {
		mIndex = index;
		emit indexChanged(index);
	}
}

qreal LabelProperties::x() const
{
	return mX;
}

void LabelProperties::setX(qreal x)
{
	if (mX != x) {
		mX = x;
		emit xChanged(x);
	}
}

qreal LabelProperties::y() const
{
	return mY;
}

void LabelProperties::setY(qreal y)
{
	if (mY != y) {
		mY = y;
		emit yChanged(y);
	}
}

bool LabelProperties::isStatic() const
{
	return mBinding.isEmpty();
}

QString LabelProperties::text() const
{
	return mText;
}

void LabelProperties::setText(const QString &text)
{
	if (mText != text) {
		mText = text;
		emit textChanged(text);
	}
}

QString LabelProperties::binding() const
{
	return mBinding;
}

void LabelProperties::setBinding(const QString &binding)
{
	if (mBinding != binding) {
		mBinding = binding;
		emit bindingChanged(binding);
	}
}

bool LabelProperties::isReadOnly() const
{
	return mReadOnly;
}

void LabelProperties::setReadOnly(bool readOnly)
{
	if (mReadOnly != readOnly) {
		mReadOnly = readOnly;
		emit readOnlyChanged(readOnly);
	}
}

qreal LabelProperties::rotation() const
{
	return mRotation;
}

void LabelProperties::setRotation(qreal degrees)
{
	if (mRotation != degrees) {
		mRotation = degrees;
		emit rotationChanged(degrees);
	}
}

QColor LabelProperties::background() const
{
	return mBackground;
}

void LabelProperties::setBackground(const QColor &background)
{
	if (mBackground != background) {
		mBackground = background;
		emit backgroundChanged(background);
	}
}

bool LabelProperties::scalingX() const
{
	return mScalingX;
}

void LabelProperties::setScalingX(bool enabled)
{
	if (mScalingX != enabled) {
		mScalingX = enabled;
		emit scalingXChanged(enabled);
	}
}

bool LabelProperties::scalingY() const
{
	return mScalingY;
}

void LabelProperties::setScalingY(bool enabled)
{
	if (mScalingY != enabled) {
		mScalingY = enabled;
		emit scalingYChanged(enabled);
	}
}

bool LabelProperties::isHard() const
{
	return mIsHard;
}

void LabelProperties::setHard(bool hard)
{
	if (mIsHard != hard) {
		mIsHard = hard;
		emit isHardChanged(hard);
	}
}
