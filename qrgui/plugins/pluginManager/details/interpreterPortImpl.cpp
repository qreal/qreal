#include "interpreterPortImpl.h"

InterpreterPortImpl::InterpreterPortImpl(const QString &type)
	: mType(type)
{}

QString InterpreterPortImpl::type() const
{
	return mType;
}
