#include "saveLoadLogicForInterpreter.h"

using namespace qReal;
using namespace qReal::shapeEdit;

SaveLoadLogicForInterpreter::SaveLoadLogicForInterpreter(IShapeEdit *parent
        , Scene *scene
        , const Id id
        , const EditorManagerInterface &editorManagerProxy
        , const IdList graphicalElements
        , qReal::gui::editor::EditorView *editorView
        , const bool isUsingTypedPorts)
    : SaveLoadLogic(parent, scene, isUsingTypedPorts)
    , mId(id)
    , mEditorManager(editorManagerProxy)
    , mGraphicalElements(graphicalElements)
    , mEditorView(editorView)
{}

SaveLoadLogicForInterpreter::~SaveLoadLogicForInterpreter()
{
    delete mEditorView;
}

void SaveLoadLogicForInterpreter::doSave()
{
    QDomDocument doc = generateDom();
    mEditorManager.updateShape(mId, doc.toString(4));

    foreach (const Id graphicalElement, mGraphicalElements) {
        mEditorManager.updateShape(graphicalElement, doc.toString(4));

        for (QGraphicsItem * const item : mEditorView->editorViewScene().items()) {
            qReal::gui::editor::NodeElement * const element = dynamic_cast<qReal::gui::editor::NodeElement *>(item);
            if (element && element->id().type() == mId.type()) {
                element->updateShape(doc.toString(4));
            }
        }
    }
}
