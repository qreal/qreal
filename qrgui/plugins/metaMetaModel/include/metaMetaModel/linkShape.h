/* Copyright 2016 Dmitry Mordvinov
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

namespace qReal {

/// Represents variants of shapes of edge elements on scene.
enum class LinkShape
{
	/// Link has piecewise-linear shape with breaks under arbitrary angle.
	broken = 0

	/// Link has piecewise-linear shape with breaks under right angle.
	, square

	/// Link has cubic bezier curve shape with two control points.
	, curve
};

}
