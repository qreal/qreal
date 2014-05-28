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
		
		void OnButton_03Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_04();
		}

		void OnButton_01Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_02();
		}

		void OnButton_02Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_03();
		}

		void OnButton_04Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateSlide_02();
		}

		private VisualElement CreateSlide_02()
		{
			var mainGrid = new Grid(2, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			var button_Button_02 = new Button();
			button_Button_02.Pressed += OnButton_02Clicked;
			button_Button_02.Pressed += ByButton02Handler;
			mainGrid[2, 3] =  button_Button_02;

			var text_Text_02 = new TextBlock() { Text = "choosePlayerText" };
			mainGrid[1, 1] =  text_Text_02;

			var list = new ListBox();
			var text_Text_21 = new TextBlock() { Text = "OText" };
			list.Append(text_Text_21)
			var text_Text_22 = new TextBlock() { Text = "XText" };
			list.Append(text_Text_22)
			mainGrid[1, 2] = list;

			var exitButton_ExitButton_02 = new Button();
			exitButton_ExitButton_02.Pressed += OnExitButton_02Clicked;
			mainGrid[1, 3] =  exitButton_ExitButton_02;

			return mainGrid;
		}

		void OnExitButton_02Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitForm();
		}

		void ByButton02Handler(object sender, EventArgs args)
		{
			// TODO
		}

		private VisualElement CreateSlide_04()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			var button_Button_04 = new Button();
			button_Button_04.Pressed += OnButton_04Clicked;
			button_Button_04.Pressed += ByButton04Handler;
			mainGrid[1, 2] =  button_Button_04;

			var text_Text_04 = new TextBlock() { Text = "gameStatusText" };
			mainGrid[1, 1] =  text_Text_04;

			var exitButton_ExitButton_04 = new Button();
			exitButton_ExitButton_04.Pressed += OnExitButton_04Clicked;
			mainGrid[1, 3] =  exitButton_ExitButton_04;

			return mainGrid;
		}

		void OnExitButton_04Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitForm();
		}

		void ByButton04Handler(object sender, EventArgs args)
		{
			// TODO
		}

		private VisualElement CreateMainSlide_01()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			var button_Button_01 = new Button();
			button_Button_01.Pressed += OnButton_01Clicked;
			button_Button_01.Pressed += ByButton01Handler;
			mainGrid[1, 2] =  button_Button_01;

			var text_Text_01 = new TextBlock() { Text = "gameName" };
			mainGrid[1, 1] =  text_Text_01;

			var exitButton_ExitButton_01 = new Button();
			exitButton_ExitButton_01.Pressed += OnExitButton_01Clicked;
			mainGrid[1, 3] =  exitButton_ExitButton_01;

			return mainGrid;
		}

		void OnExitButton_01Clicked(object sender, EventArgs args)
		{
			Screen.Content = CreateExitForm();
		}

		void ByButton01Handler(object sender, EventArgs args)
		{
			// TODO
		}

		private VisualElement CreateSlide_03()
		{
			var mainGrid = new Grid(1, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			var button_Button_03 = new Button();
			button_Button_03.Pressed += OnButton_03Clicked;
			button_Button_03.Pressed += ByButton03Handler;
			mainGrid[1, 3] =  button_Button_03;

			var text_Text_03 = new TextBlock() { Text = "mySignText" };
			mainGrid[1, 1] =  text_Text_03;

			var grid = new Grid(3, 3)
			{
				Margin = new Thickness(5, 5, 5, 5),
				VerticalAlignment = VerticalAlignment.Stretch,
				HorizontalAlignment = HorizontalAlignment.Stretch
			};
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					var button_Button_31 = new Button();
					button_Button_31.Pressed += ByGridButtonHandler;
					grid[i, j] = button_Button_31;
				}
			}
			mainGrid[1, 2] = grid;
			
			return mainGrid;
		}

		void ByButton03Handler(object sender, EventArgs args)
		{
			// TODO
		}

		void ByGridButtonHandler(object sender, EventArgs args)
		{
			// TODO
		}
	}
}
