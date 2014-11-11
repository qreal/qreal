#pragma once

#include <QtCore/QScopedPointer>

#include <gtest/gtest.h>

#include "qrtext/lua/luaToolbox.h"

namespace qrTest {

class LuaToolboxTest : public testing::Test
{
protected:
	void SetUp() override;

	QSharedPointer<qrtext::core::ast::Node> parse(QString const &code);

	QScopedPointer<qrtext::lua::LuaToolbox> mToolbox;
};

}
