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

#include "contactListener.h"

#include<QDebug>
void ContactListener::EndContact(b2Contact *contact)
{
	handleContact(contact);
}

void ContactListener::BeginContact(b2Contact *contact)
{
	handleContact(contact);
}

void ContactListener::handleContact(b2Contact *contact)
{
	while (contact) {
		b2Body *bodyA = contact->GetFixtureA()->GetBody();
		b2Body *bodyB = contact->GetFixtureB()->GetBody();
//		qDebug() << Q_FUNC_INFO << bodyA << bodyA->GetAngle() << bodyA->GetPosition().x << bodyA->GetPosition().y;
//		qDebug() << Q_FUNC_INFO << bodyB << bodyB->GetAngle() << bodyB->GetPosition().x << bodyB->GetPosition().y;
		contact = contact->GetNext();

		emit bodyPositionChanged(bodyA, bodyA->GetPosition(), bodyA->GetAngle());
		emit bodyPositionChanged(bodyB, bodyB->GetPosition(), bodyB->GetAngle());
	}
}
