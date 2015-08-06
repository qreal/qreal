var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var newProject = api.ui().widget("QToolButton", "actionNewProject");
assert(newProject != null);
expectDialog(mainWindow, "Create project", "Robot`s Behaviour Diagram", 3000);
api.cursor().moveTo(newProject, 400);
api.cursor().leftButtonPress(newProject);
api.cursor().leftButtonRelease(newProject, 400);


