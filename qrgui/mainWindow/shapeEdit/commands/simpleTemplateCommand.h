#pragma once

#include <stdlib.h>
//#include <functional>
#include <tr1/functional>

#include <qrgui/controller/commands/abstractCommand.h>

namespace qReal {
namespace shapeEdit {

template<class T1, class T2>
class SimpleTemplateCommand : public commands::AbstractCommand
{
public:
    SimpleTemplateCommand(std::tr1::function<void (const T1, T2)> action, T2 target
                          , const T1 initialData, const T1 newData, bool isExecuted = false);

protected:
    bool execute();
    bool restoreState();

private:
    std::tr1::function<void (T1, T2)> mAction;
    T2 mTarget;
    bool mIsExecuted;
    const T1 mInitialData;
    const T1 mNewData;
};


template<class T1, class T2>
SimpleTemplateCommand<T1, T2>::SimpleTemplateCommand(std::tr1::function<void (const T1, T2)> action
                          , T2 target, const T1 initialData, const T1 newData, bool isExecuted)
    : mAction(action)
    , mTarget(target)
    , mIsExecuted(isExecuted)
    , mInitialData(initialData)
    , mNewData(newData)
{}

template<class T1, class T2>
bool SimpleTemplateCommand<T1, T2>::execute()
{
    if (!mIsExecuted) {
        mAction(mNewData, mTarget);
        mIsExecuted = true;
    }
    return mIsExecuted;
}

template<class T1, class T2>
bool SimpleTemplateCommand<T1, T2>::restoreState()
{
    if (mIsExecuted) {
        mAction(mInitialData, mTarget);
        mIsExecuted = false;
    }
    return !mIsExecuted;
}

}
}
