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

#pragma once

#include <QtScript/QScriptEngine>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

Q_DECLARE_METATYPE(QMenu*)
Q_DECLARE_METATYPE(QAction*)

namespace qReal {
namespace gui {
namespace scriptUtils {

QScriptValue qMenuToScriptValue(QScriptEngine *engine, QMenu * const &in);
void qMenuFromScriptValue(const QScriptValue &object, QMenu *&out);

QScriptValue qActionToScriptValue(QScriptEngine *engine, QAction * const &in);
void qActionFromScriptValue(const QScriptValue &object, QAction *&out);

void registerDeclaredTypes(QScriptEngine *engine);

}
}
}
