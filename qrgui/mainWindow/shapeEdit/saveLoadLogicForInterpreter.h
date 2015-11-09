#pragma once

#include "editor/editorView.h"

#include "saveLoadLogic.h"

class SaveLoadLogicForInterpreter : public SaveLoadLogic
{
public:

    SaveLoadLogicForInterpreter(IShapeEdit *parent
                                , Scene *scene
                                , const Id &parentId
                                , const EditorManagerInterface &editorManagerProxy
                                , const IdList &graphicalElements
                                , EditorView *editorView);

    ~SaveLoadLogicForInterpreter();

private:
    void doSave();

    const Id &mParentId;
    const EditorManagerInterface &mEditorManager;
    const IdList &mGraphicalElements;
    EditorView *mEditorView;
};
