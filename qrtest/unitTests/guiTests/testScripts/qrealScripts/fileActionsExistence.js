var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);
var menuFile = api.ui().getMenu("menu_File");
assert(menuFile != null);

api.ui().activateMenu(menuFile);
api.wait(200);

var actionNewProject = api.ui().getActionInMenu(menuFile, "actionNewProject");
assert(actionNewProject != null);
assert(api.ui().isEnabledAndVisible(actionNewProject));
assert(!api.ui().actionIsCheckable(actionNewProject));
assert(!api.ui().actionIsChecked(actionNewProject));
assert(!api.ui().isSubMenuInMenu(menuFile, actionNewProject));

var actionNewDiagram = api.ui().getActionInMenu(menuFile, "actionNew_Diagram");
assert(actionNewDiagram != null);
assert(api.ui().isEnabledAndVisible(actionNewDiagram));
assert(!api.ui().actionIsCheckable(actionNewDiagram));
assert(!api.ui().actionIsChecked(actionNewDiagram));
assert(!api.ui().isSubMenuInMenu(menuFile, actionNewDiagram));

var actionRecentProjects = api.ui().getActionInMenu(menuFile, "Recent projects");
assert(actionRecentProjects != null);
assert(api.ui().isEnabledAndVisible(actionRecentProjects));
assert(!api.ui().actionIsCheckable(actionRecentProjects));
assert(!api.ui().actionIsChecked(actionRecentProjects));
assert(api.ui().isSubMenuInMenu(menuFile, actionRecentProjects));
var subMenuRecentProject = api.ui().getMenuContainedByAction(actionRecentProjects);
assert(subMenuRecentProject != null);

var actionOpen = api.ui().getActionInMenu(menuFile, "actionOpen");
assert(actionOpen != null);
assert(api.ui().isEnabledAndVisible(actionOpen));
assert(!api.ui().actionIsCheckable(actionOpen));
assert(!api.ui().actionIsChecked(actionOpen));
assert(!api.ui().isSubMenuInMenu(menuFile, actionOpen));

var actionSave = api.ui().getActionInMenu(menuFile, "actionSave");
assert(actionSave != null);
assert(!api.ui().isEnabledAndVisible(actionSave));
assert(!api.ui().actionIsCheckable(actionSave));
assert(!api.ui().actionIsChecked(actionSave));
assert(!api.ui().isSubMenuInMenu(menuFile, actionSave));

var actionSaveAs = api.ui().getActionInMenu(menuFile, "actionSave_as");
assert(actionSaveAs != null);
assert(!api.ui().isEnabledAndVisible(actionSaveAs));
assert(!api.ui().actionIsCheckable(actionSaveAs));
assert(!api.ui().actionIsChecked(actionSaveAs));
assert(!api.ui().isSubMenuInMenu(menuFile, actionSaveAs));

var actionPicture = api.ui().getActionInMenu(menuFile, "actionSave_diagram_as_a_picture");
assert(actionPicture != null);
assert(!api.ui().isEnabledAndVisible(actionPicture));
assert(!api.ui().actionIsCheckable(actionPicture));
assert(!api.ui().actionIsChecked(actionPicture));
assert(!api.ui().isSubMenuInMenu(menuFile, actionPicture));

var actionPrint = api.ui().getActionInMenu(menuFile, "actionPrint");
assert(actionPrint != null);
assert(!api.ui().isEnabledAndVisible(actionPrint));
assert(!api.ui().actionIsCheckable(actionPrint));
assert(!api.ui().actionIsChecked(actionPrint));
assert(!api.ui().isSubMenuInMenu(menuFile, actionPrint));

var actionQuit = api.ui().getActionInMenu(menuFile, "actionQuit");
assert(actionQuit != null);
assert(api.ui().isEnabledAndVisible(actionQuit));
assert(!api.ui().actionIsCheckable(actionQuit));
assert(!api.ui().actionIsChecked(actionQuit));
assert(!api.ui().isSubMenuInMenu(menuFile, actionQuit));

//api.ui().activateMenuAction(menuFile, actionQuit);
//api.wait(200);
