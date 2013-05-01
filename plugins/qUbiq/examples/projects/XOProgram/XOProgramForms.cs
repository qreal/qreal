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
			mainGrid[1, 1] = "XO Game";
			var button_1 = new Button(1, 3, this);
			button_1.Clicked += OnExitButton_01Clicked;
			mainGrid[1, 3] = button_1;
			var button_2 = new Button(1, 2, this);
			button_2.Clicked += OnPlayButtonClicked;
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
			mainGrid[1, 1] = "You are " + XOProgramVariables.gameStatus.ToString() + " !";
			var button_1 = new Button(1, 3, this);
			button_1.Clicked += OnExitButton_04Clicked;
			mainGrid[1, 3] = button_1;
			var button_2 = new Button(1, 2, this);
			button_2.Clicked += OnNewPlayButtonClicked;
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
			mainGrid[1, 1] = "You play for " + XOProgramVariables.mySign.ToString() + " ";
			var grid_1 = new Grid();
			mainGrid[1, 2] = grid_1;
			var button_2 = new Button(1, 3, this);
			button_2.Clicked += OnGiveUpButtonClicked;
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
			mainGrid[1, 1] = "Choose your player";
			var button_2 = new Button(1, 3, this);
			button_2.Clicked += OnExitButton_02Clicked;
			mainGrid[1, 3] = button_2;
			var button_3 = new Button(2, 3, this);
			button_3.Clicked += OnOkButtonClicked;
			mainGrid[2, 3] = button_3;

			return mainGrid;
		}

		
		void OnExitButton_01Clicked()
		{
			Exit();
		}
		void OnPlayButtonClicked()
		{
			Screen.Content = CreateSlide_02();
		}
		void OnExitButton_04Clicked()
		{
			Exit();
		}
		void OnNewPlayButtonClicked()
		{
			Screen.Content = CreateSlide_02();
		}
		void OnGiveUpButtonClicked()
		{
			Screen.Content = CreateSlide_04();
		}
		void OnExitButton_02Clicked()
		{
			Exit();
		}
		void OnOkButtonClicked()
		{
			Screen.Content = CreateSlide_03();
		}

    }
}
