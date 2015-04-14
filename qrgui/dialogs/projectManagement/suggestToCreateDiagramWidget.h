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

#include <qrkernel/ids.h>

#include "listWidget.h"

namespace qReal {

class EditorManagerInterface;

/// @brief ListWidget filled with available diagram types
/// Associated with item data are diagram Id
/// When you select an item class manually close the parent dialog
class SuggestToCreateDiagramWidget : public ListWidget
{
	Q_OBJECT

public:
	explicit SuggestToCreateDiagramWidget(const EditorManagerInterface &editorManager
			, QWidget *parent = 0);

private:
	void addItem(const Id &editor, const Id &diagram);

	const EditorManagerInterface &mEditorManager;
};

}
