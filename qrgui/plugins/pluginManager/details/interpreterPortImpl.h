#pragma once

#include "plugins/editorPluginInterface/portHelpers.h"

class InterpreterPortImpl : public PortImpl
{
public:
	InterpreterPortImpl(QString const &type);
	virtual QString type() const;

private:
	QString mType;
};

