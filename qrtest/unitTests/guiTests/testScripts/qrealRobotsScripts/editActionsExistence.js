var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.switchToWindow(mainWindow);
api.wait(500);
var newProject = api.ui().widget("QToolButton", "actionNewProject");
assert(newProject != null);
expectDialog(mainWindow, "Create project", "Robot`s Behaviour Diagram", 3000);
api.cursor().moveTo(newProject, 400);
api.cursor().leftButtonPress(newProject);
api.cursor().leftButtonRelease(newProject, 400);





//expectDialog(mainWindow, "Create project", "Block Diagram", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "Deployment Sequence Diagram", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "DomainAnalysisEditor", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "Dragon Diagram", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "Meta Editor", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "Requirements Diagram", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "Refactoring Diagram", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
//expectDialog(mainWindow, "Create project", "Robot`s Behaviour Diagram", 3000);
//api.cursor().moveTo(newProject, 400);
//api.cursor().leftButtonPress(newProject);
//api.cursor().leftButtonRelease(newProject, 400);
