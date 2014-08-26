function Component()
{
}

Component.prototype.createOperations = function()
{
    component.createOperations();
    component.addOperation("CreateShortcut", "@TargetDir@/" + installer.executableName + installer.execExtension
            , "@DesktopDir@/@ProductName@" + installer.linkExtension);
}
