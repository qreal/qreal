var mainWindow = api.ui().mainWindow();
assert(mainWindow != null);
api.changeWindow(mainWindow);
api.wait(500);

var actionNewProject = api.ui().widget("QToolButton", "actionNewProject");
assert(actionNewProject != null);
assert(api.ui().isEnabledAndVisible(actionNewProject));
closeExpectedDialog(mainWindow, "Create project", 500);
api.cursor().moveTo(actionNewProject, 50);
api.cursor().leftButtonPress(actionNewProject);
api.cursor().leftButtonRelease(actionNewProject, 50);
api.wait(550);

var actionNew_Diagram = api.ui().widget("QToolButton", "actionNew_Diagram");
assert(actionNew_Diagram != null);
assert(api.ui().isEnabledAndVisible(actionNew_Diagram));
closeExpectedDialog(mainWindow, "Create diagram", 500);
api.cursor().moveTo(actionNew_Diagram, 50);
api.cursor().leftButtonPress(actionNew_Diagram);
api.cursor().leftButtonRelease(actionNew_Diagram, 50);
api.wait(550);

var actionOpen = api.ui().widget("QToolButton", "actionOpen");
assert(actionOpen != null);
assert(api.ui().isEnabledAndVisible(actionOpen));
closeExpectedDialog(mainWindow, "Open existing project", 2000);
api.cursor().moveTo(actionOpen, 50);
api.cursor().leftButtonPress(actionOpen);
api.cursor().leftButtonRelease(actionOpen, 50);
api.wait(2500);

var actionSave = api.ui().widget("QToolButton", "actionSave");
assert(actionSave != null);
assert(!api.ui().isEnabledAndVisible(actionSave));
api.cursor().moveTo(actionSave, 50);
api.cursor().leftButtonPress(actionSave);
api.cursor().leftButtonRelease(actionSave, 50);
api.wait(150);

var actionUndo = api.ui().widget("QToolButton", "actionUndo");
assert(actionUndo != null);
assert(!api.ui().isEnabledAndVisible(actionUndo));
api.cursor().moveTo(actionUndo, 50);
api.cursor().leftButtonPress(actionUndo);
api.cursor().leftButtonRelease(actionUndo, 50);
api.wait(250);

var actionRedo = api.ui().widget("QToolButton", "actionRedo");
assert(actionRedo != null);
assert(!api.ui().isEnabledAndVisible(actionRedo));
api.cursor().moveTo(actionRedo, 50);
api.cursor().leftButtonPress(actionRedo);
api.cursor().leftButtonRelease(actionRedo, 50);
api.wait(250);

var actionZoom_In = api.ui().widget("QToolButton", "actionZoom_In");
assert(actionZoom_In != null);
assert(!api.ui().isEnabledAndVisible(actionZoom_In));
api.cursor().moveTo(actionZoom_In, 50);
api.cursor().leftButtonPress(actionZoom_In);
api.cursor().leftButtonRelease(actionZoom_In, 50);
api.wait(250);

var actionZoom_Out = api.ui().widget("QToolButton", "actionZoom_Out");
assert(actionZoom_Out != null);
assert(!api.ui().isEnabledAndVisible(actionZoom_Out));
api.cursor().moveTo(actionZoom_Out, 50);
api.cursor().leftButtonPress(actionZoom_Out);
api.cursor().leftButtonRelease(actionZoom_Out, 50);
api.wait(250);

var actionFullscreen = api.ui().widget("QToolButton", "actionFullscreen");
assert(actionFullscreen != null);
api.cursor().moveTo(actionFullscreen, 50);
api.cursor().leftButtonPress(actionFullscreen);
api.cursor().leftButtonRelease(actionFullscreen, 50);
api.wait(250);


var menu_View = api.ui().getMenu("menu_View");
assert(menu_View != null);
api.ui().activateMenu(menu_View);
api.wait(200);

var actionMenuPanels = api.ui().getActionInMenu(menu_View, "Panels");
assert(actionMenuPanels != null);
assert(api.ui().isEnabledAndVisible(actionMenuPanels));
assert(!api.ui().actionIsCheckable(actionMenuPanels));
assert(!api.ui().actionIsChecked(actionMenuPanels));
assert(api.ui().isSubMenuInMenu(menu_View, actionMenuPanels));

var subMenuPanels = api.ui().getMenuContainedByAction(actionMenuPanels);
assert(subMenuPanels != null);

var actionMiniMap = api.ui().getActionInMenu(subMenuPanels, "Mini Map");
assert(actionMiniMap != null);
assert(api.ui().isEnabledAndVisible(actionMiniMap));
assert(api.ui().actionIsCheckable(actionMiniMap));
assert(!api.ui().actionIsChecked(actionMiniMap));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionMiniMap));

var actionPalette = api.ui().getActionInMenu(subMenuPanels, "Palette");
assert(actionPalette != null);
assert(api.ui().isEnabledAndVisible(actionPalette));
assert(api.ui().actionIsCheckable(actionPalette));
assert(api.ui().actionIsChecked(actionPalette));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionPalette));


var actionLogicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Logical Model Explorer");
assert(actionLogicalExplorer != null);
assert(api.ui().isEnabledAndVisible(actionLogicalExplorer));
assert(api.ui().actionIsCheckable(actionLogicalExplorer));
assert(!api.ui().actionIsChecked(actionLogicalExplorer));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionLogicalExplorer));


var actionErrors = api.ui().getActionInMenu(subMenuPanels, "Errors");
assert(actionErrors != null);
assert(api.ui().isEnabledAndVisible(actionErrors));
assert(api.ui().actionIsCheckable(actionErrors));
assert(!api.ui().actionIsChecked(actionErrors));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionErrors));


var actionGraphicalExplorer = api.ui().getActionInMenu(subMenuPanels, "Graphical Model Explorer");
assert(actionGraphicalExplorer != null);
assert(api.ui().isEnabledAndVisible(actionGraphicalExplorer));
assert(api.ui().actionIsCheckable(actionGraphicalExplorer));
assert(!api.ui().actionIsChecked(actionGraphicalExplorer));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGraphicalExplorer));


var actionProperty = api.ui().getActionInMenu(subMenuPanels, "Property Editor");
assert(actionProperty != null);
assert(api.ui().isEnabledAndVisible(actionProperty));
assert(api.ui().actionIsCheckable(actionProperty));
assert(!api.ui().actionIsChecked(actionProperty));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionProperty));


var actionFileToolbar = api.ui().getActionInMenu(subMenuPanels, "File Toolbar");
assert(actionFileToolbar != null);
assert(api.ui().isEnabledAndVisible(actionFileToolbar));
assert(api.ui().actionIsCheckable(actionFileToolbar));
assert(api.ui().actionIsChecked(actionFileToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionFileToolbar));


var actionEditToolbar = api.ui().getActionInMenu(subMenuPanels, "Edit Toolbar");
assert(actionEditToolbar != null);
assert(api.ui().isEnabledAndVisible(actionEditToolbar));
assert(api.ui().actionIsCheckable(actionEditToolbar));
assert(api.ui().actionIsChecked(actionEditToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionEditToolbar));


var actionViewToolbar = api.ui().getActionInMenu(subMenuPanels, "View Toolbar");
assert(actionViewToolbar != null);
assert(api.ui().isEnabledAndVisible(actionViewToolbar));
assert(api.ui().actionIsCheckable(actionViewToolbar));
assert(api.ui().actionIsChecked(actionViewToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionViewToolbar));


var actionInterpreterToolbar = api.ui().getActionInMenu(subMenuPanels, "Interpreter Toolbar");
assert(actionInterpreterToolbar != null);
assert(api.ui().isEnabledAndVisible(actionInterpreterToolbar));
assert(api.ui().actionIsCheckable(actionInterpreterToolbar));
assert(!api.ui().actionIsChecked(actionInterpreterToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionInterpreterToolbar));


var actionGeneratorsToolbar = api.ui().getActionInMenu(subMenuPanels, "Generators Toolbar");
assert(actionGeneratorsToolbar != null);
assert(api.ui().isEnabledAndVisible(actionGeneratorsToolbar));
assert(api.ui().actionIsCheckable(actionGeneratorsToolbar));
assert(!api.ui().actionIsChecked(actionGeneratorsToolbar));
assert(!api.ui().isSubMenuInMenu(subMenuPanels, actionGeneratorsToolbar));
