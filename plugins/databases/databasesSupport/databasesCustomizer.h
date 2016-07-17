/* Copyright 2014-2015 Anastasia Semenova
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

#include <QtCore/QObject>
#include <QtWidgets/QDockWidget>
#include <QIcon>
#include <QObject>

#include <../../../qrkernel/settingsManager.h>
#include <qrgui/plugins/toolPluginInterface/customizer.h>

namespace qReal {
namespace databasesSupport {

// TODO
/// Class for databases customisation
class DatabasesCustomizer : public QObject, public qReal::Customizer
{
	Q_OBJECT
public:
	~DatabasesCustomizer();

signals:

public slots:

};

}
}
