var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);

var newProject = api.ui().widget("QToolButton", "actionNewProject");
assert(newProject != null);
