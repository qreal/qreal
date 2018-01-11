/* Copyright 2018 CyberTech Labs Ltd.
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

#include <QObject>
#include <Box2D/Box2D.h>

class ContactListener : public QObject, public b2ContactListener
{
Q_OBJECT

public:
	void EndContact(b2Contact* contact) override;
	void BeginContact(b2Contact *contact) override;

signals:
	void bodyPositionChanged(b2Body * body, b2Vec2 position, float32 angle);

private:
	void handleContact(b2Contact *contact);
};
