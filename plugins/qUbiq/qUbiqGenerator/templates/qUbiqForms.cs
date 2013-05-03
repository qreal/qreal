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
		
		private VisualElement CreateExitForm()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			mainGrid[1, 1] = new TextBlock() { Text = "That's all" };
			return mainGrid;
		}
		
@@createFormDescriptions@@
		
@@onButtonClickedDescriptions@@
	}
}
