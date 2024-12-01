namespace AoC2024;

public struct TestCase
{
    public TestCase(string input, string part1, string part2)
    {
        Input = input.Split('\n');
        ExpectedPart1 = part1;
        ExpectedPart2 = part2;
    }
    public IEnumerable<string> Input { get; set; }
    public string ExpectedPart1 { get; set; }
    public string ExpectedPart2 { get; set; }
}

public abstract class AoCSolution
{
    public abstract string Day { get; }
    public abstract string Title { get; }
        
    public abstract string part1(IEnumerable<string> input, RichTextBox? terminal);
    
    public abstract string part2(IEnumerable<string> input, RichTextBox? terminal);

    public abstract TestCase[] tests();

    public void run_day(RichTextBox terminal)
    {
        terminal.AppendText(Title + "\n");
        
        terminal.AppendText("Part1\n");
        var input = File.ReadLines($"../../../../../inputs/day{Day}.txt");
        terminal.AppendText(part1(input, terminal) + "\n");
       
        terminal.AppendText("Part2\n");
        input = File.ReadLines($"../../../../../inputs/day{Day}.txt");
        terminal.AppendText(part2(input, terminal) + "\n");
        
    }
    public bool run_tests(RichTextBox terminal)
    {
        TestCase[] testCases = tests();
        
        terminal.AppendText("Running tests for part1\n");
        bool ok = true;
        foreach (var testCase in testCases)
        {
            var result = part1(testCase.Input, terminal);
            terminal.AppendText($"Expected: {testCase.ExpectedPart1}, Actual: {result} | ");
            if (result != testCase.ExpectedPart1)
            {
                terminal.AppendText("FAILED\n");
                ok = false;
            }
            else
            {
                terminal.AppendText("OK\n");
            }
        }
        
        terminal.AppendText("Running tests for part2\n");
        bool ok_part2 = true;
        foreach (var testCase in testCases)
        {
            var result = part2(testCase.Input, terminal);
            terminal.AppendText($"Expected: {testCase.ExpectedPart2}, Actual: {result} | ");
            if (result != testCase.ExpectedPart2)
            {
                terminal.AppendText("FAILED\n");
                ok_part2 = false;
            }
            else
            {
                terminal.AppendText("OK\n");
            }
        }
        return ok && ok_part2;
    }
}