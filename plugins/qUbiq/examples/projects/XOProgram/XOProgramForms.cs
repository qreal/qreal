namespace XOProgram
{
	using Ubiq.Graphics;
	using System;
    
	public sealed class XOProgramForms: MExtendedThreadApp
	{
		protected override void MainOverride()
		{
			Screen.Init();
			var startForm = CreateMainSlide_01();
			Screen.Content = startForm;
			Screen.ControlMode = true;
            
			WaitForInput("7890\r");            
		}
		
		private VisualElement CreateMainSlide_01()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			mainGrid[1, 1] = new TextBlock() { Text = "XO Game" };
			var button_1 = new Button();
			button_1.Pressed += OnExitButton_01Clicked;
			mainGrid[1, 3] = button_1;
			var button_2 = new Button();
			button_2.Pressed += OnPlayButtonClicked;
			mainGrid[1, 2] = button_2;

			return mainGrid;
		}
		private VisualElement CreateSlide_04()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			mainGrid[1, 1] = new TextBlock() { Text = "You are " + XOProgramVariables.gameStatus.ToString() + " !" };
			var button_1 = new Button();
			button_1.Pressed += OnExitButton_04Clicked;
			mainGrid[1, 3] = button_1;
			var button_2 = new Button();
			button_2.Pressed += OnNewPlayButtonClicked;
			mainGrid[1, 2] = button_2;

			return mainGrid;
		}
		private VisualElement CreateSlide_03()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			mainGrid[1, 1] = new TextBlock() { Text = "You play for " + XOProgramVariables.mySign.ToString() + " " };
			var grid_1 = new Grid();
			mainGrid[1, 2] = grid_1;
			var button_2 = new Button();
			button_2.Pressed += OnGiveUpButtonClicked;
			mainGrid[1, 3] = button_2;

			return mainGrid;
		}
		private VisualElement CreateSlide_02()
		{
			var mainGrid = new Grid(2, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			var list_0 = new List();
			mainGrid[1, 2] = list_0;
			mainGrid[1, 1] = new TextBlock() { Text = "Choose your player" };
			var button_2 = new Button();
			button_2.Pressed += OnExitButton_02Clicked;
			mainGrid[1, 3] = button_2;
			var button_3 = new Button();
			button_3.Pressed += OnOkButtonClicked;
			mainGrid[2, 3] = button_3;

			return mainGrid;
		}

		
		void OnExitButton_01Clicked()
		{
			Exit();
		}
		void OnPlayButtonClicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_02();
		}
		void OnExitButton_04Clicked()
		{
			Exit();
		}
		void OnNewPlayButtonClicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitButton_02();
		}
		void OnGiveUpButtonClicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_04();
		}
		void OnExitButton_02Clicked()
		{
			Exit();
		}
		void OnOkButtonClicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_03();
		}

	}
}
