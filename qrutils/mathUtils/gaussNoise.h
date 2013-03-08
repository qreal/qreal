#pragma once

#include <QtCore/QtGlobal>

#include "../utilsDeclSpec.h"

namespace mathUtils
{

qreal const mu = 0.5;
qreal const var = 0.083; // 1/12

/// Provides Gauss Noise Generator
class QRUTILS_EXPORT GaussNoise
{
public:
	GaussNoise();
	GaussNoise(unsigned int const approximationLevel, qreal const variance);
	~GaussNoise();

	/// Gauss noise generator. Uses Central Limit Thorem Method
	qreal generate(unsigned int const approximationLevel, qreal const variance) const;
	qreal generate() const;

	/// Functions for working with properties
	void setApproximationLevel(unsigned int const approximationLevel);
	void setDispersion(qreal const variance);
	unsigned int getApproximationLevel() const;
	qreal getDispersion() const;

	GaussNoise operator >> (qreal &left);

private:
	/// Body of function 'generate'. Uses for various realizations of 'generate'
	qreal genBody(unsigned int const approximationLevel, qreal const variance) const;

	unsigned int mApproximationLevel;
	qreal mDispersion;
};

}
