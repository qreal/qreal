#include "saveLoadLogicForInterpreter.h"

SaveLoadLogicForInterpreter::SaveLoadLogicForInterpreter(IShapeEdit *parent
        , Scene *scene
        , const Id &parentId
        , const EditorManagerInterface &editorManagerProxy
        , const IdList &graphicalElements
        , EditorView *editorView) :
    SaveLoadLogic(parent, scene),
    mParentId(parentId),
    mEditorManager(editorManagerProxy),
    mGraphicalElements(graphicalElements),
    mEditorView(editorView)
{}

SaveLoadLogicForInterpreter::~SaveLoadLogicForInterpreter()
{
    delete mEditorView;
}

void SaveLoadLogicForInterpreter::doSave()
{
    QDomDocument doc = generateDom();
    mEditorManager.updateShape(mParentId, doc.toString(4));

    foreach (const Id graphicalElement, mGraphicalElements) {
        mEditorManager.updateShape(graphicalElement, doc.toString(4));

        for (QGraphicsItem * const item : mEditorView->editorViewScene().items()) {
            NodeElement * const element = dynamic_cast<NodeElement *>(item);
            if (element && element->id().type() == mParentId.type()) {
                element->updateShape(doc.toString(4));
            }
        }
    }
}
