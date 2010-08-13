/*
 * This file is part of the mouse gesture package.
 * Copyright (C) 2006 Johan Thelin <e8johan@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or
 * without modification, are permitted provided that the
 * following conditions are met:
 *
 *   - Redistributions of source code must retain the above
 *     copyright notice, this list of conditions and the
 *     following disclaimer.
 *   - Redistributions in binary form must reproduce the
 *     above copyright notice, this list of conditions and
 *     the following disclaimer in the documentation and/or
 *     other materials provided with the distribution.
 *   - The names of its contributors may be used to endorse
 *     or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
 * GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "mousegesturerecognizer.h"

static const int minimumMovement2 = 25;
static const double minimumMatch = 0.9;

// Support functions

PosList limitDirections( const PosList &positions );
PosList simplify( const PosList &positions );
PosList removeShortest( const PosList &positions );
int calcLength( const PosList &positions );

// Class implementation

MouseGestureRecognizer::MouseGestureRecognizer()
{
}

MouseGestureRecognizer::~MouseGestureRecognizer()
{
}

void MouseGestureRecognizer::addGestureDefinition(const QString &name, const QList<QPoint> &path)
{
	PosList idealPath = simplify(limitDirections(path));
	mGestures.push_back(GestureDefinition(idealPath, name));
}

void MouseGestureRecognizer::clearGestureDefinitions()
{
	mGestures.clear();
}

PosList MouseGestureRecognizer::filterGesture(const PosList &path)
{
	PosList filterGesture;
	if (path.isEmpty())
		return filterGesture;
	filterGesture.push_back(path[0]);
	for (int i = 1; i < path.size(); i++)
	{
		int dx = path.at(i).x() - path.at(i - 1).x();
		int dy = path.at(i).y() - path.at(i - 1).y();

		if (dx * dx + dy * dy >= minimumMovement2)
			filterGesture.push_back(path.at(i));
	}
	return filterGesture;
}

QString MouseGestureRecognizer::recognizeGesture(const QList<QPoint> &gesture)
{
	PosList directions = filterGesture(gesture);
	directions = simplify( limitDirections(directions));
	double minLength = calcLength( directions ) * minimumMatch;

	while( directions.size() > 0 && calcLength(directions) > minLength )
	{
		foreach(GestureDefinition gestureDefinition, mGestures)
		{
			if(gestureDefinition.directions.size() == directions.size())
			{
				bool match = true;
				for (int i = 0; i < directions.size(); i++)
				{
					QPoint idealVector = gestureDefinition.directions.at(i);
					QPoint vector = directions.at(i);
					if (vector.x() * idealVector.x() <= 0 && vector.y() * idealVector.y() <=0)
						match = false;
				}

				if( match )
				{
					return gestureDefinition.object;
				}
			}
		}
		directions = simplify(removeShortest(directions ));
	}
	return "";
}

// Support functions implementation

/*
 *  limitDirections - limits the directions of a list to up, down, left or right.
 *
 *  Notice! This function converts the list to a set of relative moves instead of a set of screen coordinates.
 */
PosList limitDirections( const PosList &positions )
{
	PosList res;
	int lastx = 0;
	int lasty = 0;
	bool firstTime = true;

	foreach (QPoint point, positions)
	{
		if( firstTime )
		{
			lastx = point.x();
			lasty = point.y();

			firstTime = false;
		}
		else
		{
			int dx = point.x() - lastx;
			int dy = point.y() - lasty;

			if (dy > 0)
			{
				if( dx > dy || -dx > dy )
					dy = 0;
				else
					dx = 0;
			}
			else
			{
				if (dx > -dy || -dx > -dy)
					dy = 0;
				else
					dx = 0;
			}

			res.push_back(QPoint(dx, dy));

			lastx = point.x();
			lasty = point.y();
		}
	}

	return res;
}

/*
 *  simplify - joins together contignous movements in the same direction.
 *
 *  Notice! This function expects a list of limited directions.
 */
PosList simplify(const PosList &positions)
{
	PosList res;
	int lastdx = 0;
	int lastdy = 0;
	bool firstTime = true;

	foreach (QPoint point, positions)
	{
		if (firstTime)
		{
			lastdx = point.x();
			lastdy = point.y();

			firstTime = false;
		}
		else
		{
			bool joined = false;

			if ((lastdx > 0 && point.x() > 0) || (lastdx < 0 && point.x() < 0))
			{
				lastdx += point.x();
				joined = true;
			}

			if( (lastdy > 0 && point.y() > 0) || (lastdy < 0 && point.y() < 0) )
			{
				lastdy += point.y();
				joined = true;
			}

			if( !joined )
			{
				res.push_back(QPoint(lastdx, lastdy));

				lastdx = point.x();
				lastdy = point.y();
			}
		}
	}

	if(lastdx != 0 || lastdy != 0)
	{
		res.push_back(QPoint(lastdx, lastdy));
	}

	return res;
}

/*
 *  removeShortest - removes the shortest segment from a list of movements.
 *
 *  If there are several equally short segments, the first one is removed.
 */
PosList removeShortest( const PosList &positions )
{
	PosList res;

	int shortestSoFar = 0;
	int shortest = 0;
	bool firstTime = true;

	for (int i = 0; i < positions.length(); i++)
	{
		int length = positions.at(i).x() * positions.at(i).x()
					 + positions.at(i).y() * positions.at(i).y();
		if (firstTime)
		{
			shortestSoFar =  length;
			shortest = i;
			firstTime = false;
		}
		else
		{
			if (length < shortestSoFar)
			{
				shortestSoFar = length;
				shortest = i;
			}
		}
	}

	for (int i = 0; i < positions.length(); i++)
	{
		if (i != shortest)
			res.push_back(positions.at(i));
	}

	return res;
}

/*
 *  calcLength - calculates the total length of the movements from a list of relative movements.
 */
int calcLength( const QList<QPoint> &positions )
{
	int res = 0;

	foreach(QPoint point, positions)
	{
		if(point.x() > 0)
			res += point.x();
		else if(point.x() < 0)
			res -= point.x();
		else if(point.y() > 0)
			res += point.y();
		else
			res -= point.y();
	}

	return res;
}
