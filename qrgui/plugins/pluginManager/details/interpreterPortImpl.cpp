#include "interpreterPortImpl.h"

InterpreterPortImpl::InterpreterPortImpl(QString const &type)
	: mType(type)
{}

QString InterpreterPortImpl::type() const
{
	return mType;
}
