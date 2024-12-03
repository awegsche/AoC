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
        
    public abstract string part1(IEnumerable<string> input, Logger logger);
    
    public abstract string part2(IEnumerable<string> input, Logger logger);

    public abstract TestCase[] tests();

    public void run_day(Logger logger)
    {
        logger.Info("--------------------\n Solving Day \n--------------------");
        
        logger.Info("Part1\n----");
        var input = File.ReadLines($"../../../../../inputs/day{Day}.txt");
        logger.Solution(part1(input, logger));
       
        logger.Info("Part2\n----");
        input = File.ReadLines($"../../../../../inputs/day{Day}.txt");
        logger.Solution(part2(input, logger));
        
    }

    public bool run_tests(Logger logger)
    {
        TestCase[] testCases = tests();
        
        logger.Info("--------------------\n Running tests for part1\n--------------------");
        bool ok = true;
        foreach (var testCase in testCases)
        {
            var result = part1(testCase.Input, logger);
            var test_info = $"Expected: {testCase.ExpectedPart1}, Actual: {result} | ";
            
            if (result != testCase.ExpectedPart1)
            {
                test_info += "FAILED";
                ok = false;
            }
            else
            {
                test_info += "OK";
            }
            
            logger.Info(test_info);
        }
        
        logger.Info("--------------------\n Running tests for part2\n--------------------");
        bool ok_part2 = true;
        foreach (var testCase in testCases)
        {
            var result = part2(testCase.Input, logger);
            var test_info = $"Expected: {testCase.ExpectedPart2}, Actual: {result} | ";
            if (result != testCase.ExpectedPart2)
            {
                test_info += "FAILED";
                ok_part2 = false;
            }
            else
            {
                test_info += "OK";
            }
            logger.Info(test_info);
        }
        return ok && ok_part2;
    }
}