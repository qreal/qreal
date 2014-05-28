namespace NewXO_MainForm
{
	using Ubiq.Graphics;
	using System;
    
	public sealed class NewXO_MainFormForms: MExtendedThreadApp
	{
		protected override void MainOverride()
		{
			Screen.Init();
			var startForm = CreateMainSlide_01();
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
		
		private VisualElement CreateMainSlide_01()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			var text_0 = new TextBlock() { Text = "gameName" };
			mainGrid[1, 1] = text_0;

			var button_1 = new Button();
			button_1.Pressed += OnExitButton_01Clicked;
			mainGrid[1, 3] = button_1;

			var button_2 = new Button();
			button_2.Pressed += OnButton_01Clicked;
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
			var text_0 = new TextBlock() { Text = "gameStatusText" };
			mainGrid[1, 1] = text_0;

			var button_1 = new Button();
			button_1.Pressed += OnExitButton_04Clicked;
			mainGrid[1, 3] = button_1;

			var button_2 = new Button();
			button_2.Pressed += OnButton_04Clicked;
			mainGrid[1, 2] = button_2;

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
			var list_0 = new ListBox();
			var text_0_0 = new TextBlock() { Text = "OText" };
			list_0.Append(text_0_0);
			var text_0_1 = new TextBlock() { Text = "XText" };
			list_0.Append(text_0_1);

			mainGrid[1, 2] = list_0;

			var text_1 = new TextBlock() { Text = "choosePlayerText" };
			mainGrid[1, 1] = text_1;

			var button_2 = new Button();
			button_2.Pressed += OnButton_02Clicked;
			mainGrid[2, 3] = button_2;

			var button_3 = new Button();
			button_3.Pressed += OnExitButton_02Clicked;
			mainGrid[1, 3] = button_3;

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
			var text_0 = new TextBlock() { Text = "mySignText" };
			mainGrid[1, 1] = text_0;

			var grid_1 = new Grid(3, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					var button_1_0 = new Button();
					button_1_0.Pressed += ByGridButtonHandler;
					grid_1[i, j] = button_1_0;
				}
			}
			mainGrid[1, 2] = grid_1;

			var button_2 = new Button();
			button_2.Pressed += OnButton_03Clicked;
			mainGrid[1, 3] = button_2;

			return mainGrid;
		}

		void OnExitButton_01Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitForm();
		}
		void OnButton_01Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_02();
		}
		void OnExitButton_04Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitForm();
		}
		void OnButton_04Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_02();
		}
		void OnButton_02Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_03();
		}
		void OnExitButton_02Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitForm();
		}
		void OnButton_03Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_04();
		}


	}
}
