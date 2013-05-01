namespace @@programName@@
{
    using Ubiq.Graphics;
    using System;
    
    public sealed class @@programName@@Forms: MExtendedThreadApp
    {
        protected override void MainOverride()
        {
            Screen.Init();
			var startForm = Create@@startFormName@@();
            Screen.Content = startForm;
            Screen.ControlMode = true;
            
            WaitForInput("7890\r");            
        }
		
		@@createFormDescriptions@@
		
		@@onButtonClickedDescriptions@@
    }
}
