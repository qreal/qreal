/* Copyright 2007-2015 QReal Research Group
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

#include "abstractRecognizer.h"
#include "geometricForms.h"

namespace qReal {
namespace gestures {

class NearestPosGridGesturesManager : public GesturesRecognizer<qreal *>
{
public:
	NearestPosGridGesturesManager();
	~NearestPosGridGesturesManager();
	qreal getMaxDistance(const QString &);
	bool isMultistroke();
	qreal getDistance(qreal * const & key1, qreal * const &key2);

	qreal getDistance(QString const &item) override
	{
		const auto key = mGestures[item];
		return getDistance(mKey, key);
	}

	qreal *getKey(const PathVector &path);
};

}
}
