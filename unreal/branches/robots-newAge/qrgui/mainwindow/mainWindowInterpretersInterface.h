#pragma once

namespace qReal {

namespace gui {

class MainWindowInterpretersInterface
{
public:
	~MainWindowInterpretersInterface() {};
	virtual void selectItem(Id const &graphicalId) = 0;
	virtual void highlight(Id const &graphicalId, bool exclusive = true) = 0;
	virtual void dehighlight(Id const &graphicalId) = 0;
};

}

}
