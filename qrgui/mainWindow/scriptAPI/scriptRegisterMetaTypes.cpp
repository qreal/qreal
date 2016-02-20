/* Copyright 2015-2016 QReal Research Group, CyberTech Labs Ltd.
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

#include "scriptRegisterMetaTypes.h"

using namespace qReal::gui::scriptUtils;

void qReal::gui::scriptUtils::registerDeclaredTypes(QScriptEngine *engine)
{
	qScriptRegisterMetaType(engine, qMenuToScriptValue, qMenuFromScriptValue);
	qScriptRegisterMetaType(engine, qActionToScriptValue, qActionFromScriptValue);
}

QScriptValue qReal::gui::scriptUtils::qMenuToScriptValue(QScriptEngine *engine, QMenu * const &in)
{
	return engine->newQObject(in);
}

void qReal::gui::scriptUtils::qMenuFromScriptValue(const QScriptValue &object, QMenu *&out)
{
	out = qobject_cast<QMenu *>(object.toQObject());
}

QScriptValue qReal::gui::scriptUtils::qActionToScriptValue(QScriptEngine *engine, QAction * const &in)
{
	return engine->newQObject(in);
}

void qReal::gui::scriptUtils::qActionFromScriptValue(const QScriptValue &object, QAction *&out)
{
	out = qobject_cast<QAction *>(object.toQObject());
}
