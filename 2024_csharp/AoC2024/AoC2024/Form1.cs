namespace AoC2024;

public partial class Form1 : Form
{
    public Form1()
    {
        InitializeComponent();
        
        terminal.Text = "Hello World!\n";
        
        run_day1();
    }

    private async void run_day1()
    {
        Day01 day01 = new Day01();

        if (day01.run_tests(terminal))
        {
            terminal.AppendText("All tests passed!\n");
        }
        else
        {
            terminal.AppendText("Tests FAILED\n");
        }
        
        day01.run_day(terminal);
    }
    
}