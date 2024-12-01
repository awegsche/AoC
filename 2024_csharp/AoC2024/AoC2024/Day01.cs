namespace AoC2024;

public class Day01: AoCSolution
{
    public override string Day => "01";
    public override string Title => "Historian";

    private (List<int>, List<int>) parse_input(IEnumerable<string> input, RichTextBox? terminal)
    {
        List<int> a = new List<int>();
        List<int> b = new List<int>();

        foreach (var line in input)
        {
            var s = line.Split(new char[0], StringSplitOptions.RemoveEmptyEntries);
            if (s.Length != 2)
            {
                terminal?.AppendText("ERROR\n");
                terminal?.AppendText($"ERROR: {line}\n");
                terminal?.AppendText($"s length: {s.Length}\n");
                break;
            }
            
            a.Add(Convert.ToInt32(s[0]));
            b.Add(Convert.ToInt32(s[1]));
        }

        a.Sort();
        b.Sort();
        
        
        return (a, b);
    }
    public override string part1(IEnumerable<string> input, RichTextBox? terminal)
    {
        var (a, b) = parse_input(input, terminal);
        return a.Zip(b)
            .Select(tuple => Math.Abs(tuple.First - tuple.Second))
            .Sum()
            .ToString();
    }

    public override string part2(IEnumerable<string> input, RichTextBox? terminal)
    {
        var (a, b) = parse_input(input, terminal);
        
        Dictionary<int, int> occurances = new Dictionary<int, int>();


        terminal.AppendText($"counting occurances\n");
        foreach (var i in b)
        {
            if (!occurances.TryAdd(i, i))
            {
                occurances[i] += i;
            }
        }
        
        terminal.AppendText("calculating similarity score\n");
        var scores = a.Select(id =>
        {
            int value = 0;
            occurances.TryGetValue(id, out value);
            return value;
        }).ToArray();
            
        return scores.Sum().ToString();
    }

    public override TestCase[] tests()
    {
        return new[]
        {
            new TestCase (  "3   4\n 4   3\n 2   5\n 1   3\n 3   9\n 3   3", "11", "31" )
        };
    }
}