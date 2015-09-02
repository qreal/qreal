init();

var actionNewProject = api.ui().widget("QToolButton", "actionNewProject");
assert(actionNewProject != null);
assert(api.ui().isEnabledAndVisible(actionNewProject));
closeExpectedDialog(mainWindow, "Create project", 500);
leftClick(actionNewProject);
api.wait(550);

var actionNew_Diagram = api.ui().widget("QToolButton", "actionNew_Diagram");
assert(actionNew_Diagram != null);
assert(api.ui().isEnabledAndVisible(actionNew_Diagram));
closeExpectedDialog(mainWindow, "Create diagram", 500);
leftClick(actionNew_Diagram);
api.wait(550);

var actionOpen = api.ui().widget("QToolButton", "actionOpen");
assert(actionOpen != null);
assert(api.ui().isEnabledAndVisible(actionOpen));
closeExpectedDialog(mainWindow, "Open existing project", 2000);
leftClick(actionOpen);
api.wait(2500);

var actionSave = api.ui().widget("QToolButton", "actionSave");
assert(actionSave != null);
assert(!api.ui().isEnabledAndVisible(actionSave));
leftClick(actionSave);
api.wait(150);

var actionUndo = api.ui().widget("QToolButton", "actionUndo");
assert(actionUndo != null);
assert(!api.ui().isEnabledAndVisible(actionUndo));
leftClick(actionUndo);
api.wait(250);

var actionRedo = api.ui().widget("QToolButton", "actionRedo");
assert(actionRedo != null);
assert(!api.ui().isEnabledAndVisible(actionRedo));
leftClick(actionRedo);
api.wait(250);

var actionZoom_In = api.ui().widget("QToolButton", "actionZoom_In");
assert(actionZoom_In != null);
assert(!api.ui().isEnabledAndVisible(actionZoom_In));
leftClick(actionZoom_In);
api.wait(250);

var actionZoom_Out = api.ui().widget("QToolButton", "actionZoom_Out");
assert(actionZoom_Out != null);
assert(!api.ui().isEnabledAndVisible(actionZoom_Out));
leftClick(actionZoom_Out);
api.wait(250);

var actionFullscreen = api.ui().widget("QToolButton", "actionFullscreen");
assert(actionFullscreen != null);
leftClick(actionFullscreen);
api.wait(250);


var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);
api.ui().activateMenu(menu_View);
api.wait(200);

var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
checkAction(actionMenuPanels, true, false, false);
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionMiniMap = api.ui().getActionInMenu(subMenuPanels, "Mini Map");
checkAction(actionMiniMap, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionMiniMap));

var actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
checkAction(actionPalette, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionPalette));

var actionLogicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Logical Model Explorer");
checkAction(actionLogicalExplorer, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));

var actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
checkAction(actionErrors, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));

var actionGraphicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Graphical Model Explorer");
checkAction(actionGraphicalExplorer, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));

var actionProperty = api.ui().getActionInMenu(subMenuPanels, "Property Editor");
checkAction(actionProperty, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionProperty));

var actionFileToolbar = api.ui().getActionInMenu(subMenuPanels, "File Toolbar");
checkAction(actionFileToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionFileToolbar));

var actionEditToolbar = api.ui().getActionInMenu(subMenuPanels, "Edit Toolbar");
checkAction(actionEditToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionEditToolbar));

var actionViewToolbar = api.ui().getActionInMenu(subMenuPanels, "View Toolbar");
checkAction(actionViewToolbar, true, true, true);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionViewToolbar));

var actionInterpreterToolbar = api.ui().getActionInMenu(subMenuPanels, "Interpreter Toolbar");
checkAction(actionInterpreterToolbar, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));

var actionGeneratorsToolbar = api.ui().getActionInMenu(subMenuPanels, "Generators Toolbar");
checkAction(actionGeneratorsToolbar, true, true, false);
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));
