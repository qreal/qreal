init();

var menuFile = api.ui().getMenu("menu_File");
assert(menuFile != null);

api.ui().activateMenu(menuFile);
api.wait(200);

var actionNewProject = api.ui().getActionInMenu(menuFile, "actionNewProject");
checkAction(actionNewProject, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionNewProject));

var actionNewDiagram = api.ui().getActionInMenu(menuFile, "actionNew_Diagram");
checkAction(actionNewDiagram, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionNewDiagram));

var actionRecentProjects = api.ui().getActionInMenu(menuFile, "Recent projects");
checkAction(actionRecentProjects, true, false, false);
assert(api.ui().isSubMenuInMenu(menuFile, actionRecentProjects));
var subMenuRecentProject = api.ui().getMenuContainedByAction(actionRecentProjects);
assert(subMenuRecentProject != null);

var actionOpen = api.ui().getActionInMenu(menuFile, "actionOpen");
checkAction(actionOpen, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionOpen));

var actionSave = api.ui().getActionInMenu(menuFile, "actionSave");
checkAction(actionSave, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionSave));

var actionSaveAs = api.ui().getActionInMenu(menuFile, "actionSave_as");
checkAction(actionSaveAs, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionSaveAs));

var actionPicture = api.ui().getActionInMenu(menuFile, "actionSave_diagram_as_a_picture");
checkAction(actionPicture, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionPicture));

var actionPrint = api.ui().getActionInMenu(menuFile, "actionPrint");
checkAction(actionPrint, false, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionPrint));

var actionQuit = api.ui().getActionInMenu(menuFile, "actionQuit");
checkAction(actionQuit, true, false, false);
assert(!api.ui().isSubMenuInMenu(menuFile, actionQuit));

api.ui().activateMenuAction(menuFile, actionQuit);
