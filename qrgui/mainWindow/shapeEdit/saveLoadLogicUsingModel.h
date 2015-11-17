#pragma once

#include <models/logicalModelAssistApi.h>

#include "saveLoadLogic.h"

namespace qReal {
namespace shapeEdit {

class SaveLoadLogicUsingModel : public SaveLoadLogic
{
public:
    SaveLoadLogicUsingModel(IShapeEdit *parent
                            , Scene *scene
                            , qReal::models::LogicalModelAssistApi &modelApi
                            , const QPersistentModelIndex &index
                            , const int role
                            , const bool isUsingTypedPorts);

private:
    void doSave();

    void doLoadPortTypes(QStringList &result);
    void doLoadProperties(QMap<QString, PropertyInfo> &result);

    qReal::models::LogicalModelAssistApi &mModelApi;
    const QPersistentModelIndex &mIndex;
    const int mRole;
    const bool mIsUsingTypedPorts;
};

}
}
