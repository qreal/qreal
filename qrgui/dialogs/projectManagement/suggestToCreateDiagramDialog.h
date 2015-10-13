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

#include "dialogs/projectManagement/managedClosableDialog.h"

namespace qReal {

class EditorManagerInterface;

/// @brief Dialog for create a diagram
/// This dialog prompts the user to create one of the available diagrams
class QRGUI_DIALOGS_EXPORT SuggestToCreateDiagramDialog : public ManagedClosableDialog
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramDialog(const EditorManagerInterface &editorManager
			, QWidget *parent = 0, bool isClosable = true);

signals:
	/// Emitted when user selects some item in diagrams list wishing thus to create
	/// new diagram of the type @arg diagramIdString.
	void diagramSelected(const QString &diagramIdString);
};

}
