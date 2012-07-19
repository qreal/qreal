#include "progress.h"

using namespace invokation;

Progress::Progress(int minimum, int maximum)
	: mMinimum(INT_MIN), mMaximum(INT_MAX) // for setMin/Max function worked correctly
{
	setMinimum(minimum);
	setMaximum(maximum);
	setValue(minimum);
}

int Progress::value() const
{
	return mValue;
}

int Progress::minimum() const
{
	return mMinimum;
}

int Progress::maximum() const
{
	return mMaximum;
}

void Progress::setValue(int value)
{
	if (value < mMinimum || value > mMaximum) {
		return;
	}
	bool const changed = value != mValue;
	mValue = value;
	if (changed) {
		emit valueChanged(value);
	}
}

void Progress::setMinimum(int minimum)
{
	if (minimum > maximum()) {
		return;
	}
	bool const changed = minimum != mMinimum;
	mMinimum = minimum;
	if (changed) {
		emit minimumChanged(minimum);
	}
}

void Progress::setMaximum(int maximum)
{
	if (maximum < minimum()) {
		return;
	}
	bool const changed = maximum != mMaximum;
	mMaximum = maximum;
	if (changed) {
		emit maximumChanged(maximum);
	}
}
