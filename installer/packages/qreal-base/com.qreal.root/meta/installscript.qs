function Component()
{
	// Executable names must be lower-case product name with hyphens instead of spaces
 	installer.executableName = installer.value("ProductName").toLowerCase().replace(/\s/g, "-");
	installer.linkExtension = installer.value("os") === "win" ? ".lnk" : "";
    	installer.execExtension = installer.value("os") === "win" ? ".exe" : "";
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    component.addOperation("CreateShortcut"
            , "@TargetDir@/" + installer.executableName + installer.execExtension
            , "@StartMenuDir@/@ProductName@" + installer.linkExtension);
    component.addOperation("CreateShortcut"
            , "@TargetDir@/maintenance" + installer.execExtension
            , "@StartMenuDir@/Uninstall @ProductName@" + installer.linkExtension);
}
