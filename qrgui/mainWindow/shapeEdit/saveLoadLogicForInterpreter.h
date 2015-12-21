#pragma once

#include "editor/editorView.h"

#include "saveLoadLogic.h"

namespace qReal {
namespace shapeEdit {

class SaveLoadLogicForInterpreter : public SaveLoadLogic
{
public:

    SaveLoadLogicForInterpreter(IShapeEdit *parent
                                , Scene *scene
                                , const Id id
                                , const EditorManagerInterface &editorManagerProxy
                                , const IdList graphicalElements
                                , qReal::gui::editor::EditorView *editorView
                                , const bool isUsingTypedPorts);

    ~SaveLoadLogicForInterpreter();

private:
    void doSave();

    const Id mId;
    const EditorManagerInterface &mEditorManager;
    const IdList mGraphicalElements;
    qReal::gui::editor::EditorView *mEditorView;
};

}
}
