#pragma once

#include <models/logicalModelAssistApi.h>

#include "saveLoadLogic.h"

class SaveLoadLogicUsingModel : public SaveLoadLogic
{
public:
    SaveLoadLogicUsingModel(IShapeEdit *parent
                            , Scene *scene
                            , qReal::models::LogicalModelAssistApi &modelApi
                            , const QPersistentModelIndex &parentIndex
                            , const int parentRole
                            , const bool isUsingTypedPorts);

private:
    void doSave();

    void doLoadPortTypes(QStringList &result);
    void doLoadProperties(QMap<QString, PropertyInfo> &result);

    qReal::models::LogicalModelAssistApi &mModelApi;
    const QPersistentModelIndex &mParentIndex;
    const int mParentRole;
    const bool mIsUsingTypedPorts;
};
