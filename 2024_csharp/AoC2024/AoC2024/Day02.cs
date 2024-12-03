using System.Diagnostics;

namespace AoC2024;

internal readonly struct Report(string line)
{
    private int[] Levels { get; } = line.Split().Select(int.Parse).ToArray();

    public bool IsSafeOneRemoved()
    {
        var report = this;
        return Levels.Where((t, i) => IsSafe(report.Levels.Take(i).Concat(report.Levels.Skip(i + 1)))).Any();
    }

    public bool IsSafe()
    {
        return IsSafe(Levels);
    }

    private static bool IsSafe(IEnumerator<int> enumerator, Predicate<int> predicate)
    {
        enumerator.MoveNext();

        var last = enumerator.Current;
        while (enumerator.MoveNext())
        {
            if (!predicate(enumerator.Current - last)) return false;
            last = enumerator.Current;
        }

        return true;
    }

    private static bool IsSafe(IEnumerable<int> levels)
    {
        return IsSafe(levels.GetEnumerator(), diff => diff is > 0 and < 4)
               || IsSafe(levels.GetEnumerator(), diff => diff is < 0 and > -4);
    }
}

public class Day02 : AoCSolution
{
    public override string Day => "02";
    public override string Title => "Red-Nosed Reports";

    private static Report[] GetReports(IEnumerable<string> input)
    {
        var reports = new List<Report>();
        foreach (var line in input)
        {
            reports.Add(new Report(line));
        }

        return reports.ToArray();
    }

    public override string part1(IEnumerable<string> input, Logger logger)
    {
        return GetReports(input).Count(report => report.IsSafe()).ToString();
    }

    public override string part2(IEnumerable<string> input, Logger logger)
    {
        return GetReports(input).Count(report => report.IsSafeOneRemoved()).ToString();
    }

    public override TestCase[] tests()
    {
        return
        [
            new TestCase("7 6 4 2 1\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9",
                "2", "4"),
            new TestCase("1 10 11 12 13\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9",
                "1", "4"),
            new TestCase("1 2 3 4 10\n1 2 7 8 9\n9 7 6 2 1\n1 3 2 4 5\n8 6 4 4 1\n1 3 6 7 9",
                "1", "4")
        ];
    }
}