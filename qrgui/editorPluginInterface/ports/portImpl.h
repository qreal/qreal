#pragma once

class PortImpl
{
public:
	virtual ~PortImpl() {}

	virtual QString type() const = 0;
};

class NonTyped : public PortImpl
{
public:
	virtual QString type() const {
		return "NonTyped";
	}
};
