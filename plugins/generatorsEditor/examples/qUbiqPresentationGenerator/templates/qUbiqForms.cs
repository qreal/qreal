namespace @@programName@@
{
	using Ubiq.Graphics;
	using System;
    
	public sealed class @@programName@@Forms: MExtendedThreadApp
	{
@@elements_TransitionLink_BeginNodeInPresentationEditor_Slide@@

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
		
@@elements_TransitionLink_Button_Slide@@
@@elements_Slide@@
	}
}
