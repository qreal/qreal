#pragma once

#include "../../../../../qrgui/plugins/editorPluginInterface/portHelpers.h"

	class TableLeft : public PortImpl
	{
	public:
		~TableLeft() {}
		virtual QString type() const { return "TableLeft"; }
	};

	class TableRight : public PortImpl
	{
	public:
		~TableRight() {}
		virtual QString type() const { return "TableRight"; }
	};

	class TableVertical : public PortImpl
	{
	public:
		~TableVertical() {}
		virtual QString type() const { return "TableVertical"; }
	};

