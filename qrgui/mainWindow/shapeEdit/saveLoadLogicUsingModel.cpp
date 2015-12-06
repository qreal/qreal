#include "saveLoadLogicUsingModel.h"

using namespace qReal;
using namespace qReal::shapeEdit;

SaveLoadLogicUsingModel::SaveLoadLogicUsingModel(IShapeEdit *parent
        , Scene *scene
        , qReal::models::LogicalModelAssistApi &modelApi
        , const QPersistentModelIndex &index
        , const int role
        , const bool isUsingTypedPorts)
    : SaveLoadLogic(parent, scene, isUsingTypedPorts)
    , mModelApi(modelApi)
    , mIndex(index)
    , mRole(role)
{}

void SaveLoadLogicUsingModel::doSave()
{
    QDomDocument doc = generateDom();
    if (mIndex.isValid()) {
        emit mParent->shapeSaved(doc.toString(4), mIndex, mRole);
    }
}

void SaveLoadLogicUsingModel::doLoadPortTypes(QStringList &result)
{
    if (mIsUsingTypedPorts) {
        qrRepo::RepoApi *repoApi = dynamic_cast<qrRepo::RepoApi *>(&mModelApi.mutableLogicalRepoApi());
        if (repoApi) {
            foreach (const qReal::Id &port, repoApi->elementsByType("MetaEntityPort")) {
                if (repoApi->isLogicalElement(port)) {
                    result << repoApi->name(port);
                }
            }
        }
    }
}

void SaveLoadLogicUsingModel::doLoadProperties(QMap<QString, PropertyInfo> &result)
{
    qrRepo::RepoApi *repoApi = dynamic_cast<qrRepo::RepoApi *>(&mModelApi.mutableLogicalRepoApi());
    qReal::IdList enums = repoApi->elementsByType("MetaEntityEnum");

    foreach (const qReal::Id &child, repoApi->children(mModelApi.idByIndex(mIndex))) {
        if (child.element() != "MetaEntity_Attribute") {
            continue;
        }

        QString type = repoApi->stringProperty(child, "attributeType");
        if (type == "int") {
            result.insert(repoApi->name(child), PropertyInfo(PropertyInfo::Int, QStringList()));
        } else if (type == "bool") {
            result.insert(repoApi->name(child), PropertyInfo(PropertyInfo::Bool, QStringList() << "true" << "false"));
        } else if (type == "string") {
            result.insert(repoApi->name(child), PropertyInfo(PropertyInfo::String, QStringList()));
        } else {
            foreach (const qReal::Id &enumElement, enums) {
                if (!repoApi->isLogicalElement(enumElement)) {
                    continue;
                }

                if (repoApi->name(enumElement) == type) {
                    QStringList enumValues;
                    foreach (const qReal::Id &value, repoApi->children(enumElement)) {
                        enumValues << repoApi->stringProperty(value, "valueName");
                    }

                    result.insert(repoApi->name(child), PropertyInfo(PropertyInfo::Enum, enumValues));
                }
            }
        }
    }
}
