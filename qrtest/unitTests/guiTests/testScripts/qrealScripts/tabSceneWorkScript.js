var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);

var openInterpreted = api.ui().getStartButton("Open interpreted diagram");
assert(openInterpreted != null);
assert(api.ui().isEnabledAndVisible(openInterpreted));
closeExpectedDialog(mainWindow, "Select file with metamodel to open", 1000);
api.cursor().moveTo(openInterpreted, 50);
api.cursor().leftButtonPress(openInterpreted);
api.cursor().leftButtonRelease(openInterpreted);
api.wait(2500);

var openInterpreted = api.ui().getStartButton("Open interpreted diagram");
assert(openInterpreted != null);
assert(api.ui().isEnabledAndVisible(openInterpreted));
var createInterpreted = api.ui().getStartButton("Create interpreted diagram");
assert(createInterpreted != null);
assert(api.ui().isEnabledAndVisible(createInterpreted));
closeExpectedDialog(mainWindow, "Enter the diagram name:", 1000);
printInExspectedActiveWindow(mainWindow, "Enter the diagram name:", "qwe", 1000)
okExpectedDialog(mainWindow, "Enter the diagram name:", 1500);
api.cursor().moveTo(createInterpreted, 50);
api.cursor().leftButtonPress(createInterpreted);
api.cursor().leftButtonRelease(createInterpreted);
api.wait(2000);
