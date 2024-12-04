namespace AoC2024;

public struct Position(int x, int y)
{
    public int X = x;
    public int Y = y;

    public static Position operator +(Position a, Position b) => new(a.X + b.X, a.Y + b.Y);
}

public class Day04 : AoCSolution
{
    public override string Day => "04";
    public override string Title => "Ceres Search";

    private char this[Position p] => _grid[p.Y * _width + p.X];
    
    private int CheckCrossMasAtPosition(int x, int y)
    {
        var p = new Position(x, y);
        if (this[p] != 'A') return 0;
        
        char[] downdiag = [this[p + new Position(-1, -1)], this[p + new Position(1,1)]];
        char[] updiag = [this[p + new Position(1, -1)], this[p + new Position(-1,1)]];

        char[] ms = ['M', 'S'];
        char[] sm = ['S', 'M'];

        if (CheckDiag(downdiag) && CheckDiag(updiag)) return 1;
        return 0;

        bool CheckDiag(char[] diag) => diag.SequenceEqual(ms) || diag.SequenceEqual(sm);
    }

    private int CheckXmas(Position p, Position direction)
    {
        var positions = new Position[4];
        positions[0] = p;
        positions[1] = p + direction;
        positions[2] = positions[1] + direction;
        positions[3] = positions[2] + direction;

        if (positions[3].X < 0 || positions[3].X >= _width || positions[3].Y < 0 ||
            positions[3].Y >= _height) return 0;

        if (this[positions[0]] == 'X'
               && this[positions[1]] == 'M'
               && this[positions[2]] == 'A'
               && this[positions[3]] == 'S')
            return 1;
        return 0;
    }

    private int CheckXmasAtPosition(int x, int y)
    {
        var xmas = 0;
        var pos = new Position(x, y);
        for (var j = -1; j <= 1; j++)
        {
            for (var i = -1; i <= 1; i++)
            {
                if (i == 0 && j == 0) continue;

                xmas += CheckXmas(pos, new Position(i, j));
            }
        }

        return xmas;
    }

    public override string part1(IEnumerable<string> input, Logger logger)
    {
        _height = 0;
        var grid = new List<char>();

        foreach (var line in input)
        {
            if (line.Length == 0) continue;
            _width = line.Length;
            _height += 1;
            grid.AddRange(line);
        }
        
        _grid = grid.ToArray();

        var xmas = 0;
        for (int y = 0; y < _height; y++)
        {
            for (int x = 0; x < _width; x++)
            {
                xmas += CheckXmasAtPosition(x, y);
            }
        }

        return xmas.ToString();
    }

    public override string part2(IEnumerable<string> input, Logger logger)
    {
        _height = 0;
        var grid = new List<char>();

        foreach (var line in input)
        {
            if (line.Length == 0) continue;
            _width = line.Length;
            _height += 1;
            grid.AddRange(line);
        }
        
        _grid = grid.ToArray();

        var xmas = 0;
        for (int y = 1; y < _height-1; y++)
        {
            for (int x = 1; x < _width-1; x++)
            {
                xmas += CheckCrossMasAtPosition(x, y);
            }
        }

        return xmas.ToString();
    }

    public override TestCase[] tests()
    {
        return
        [
            new TestCase("MMMSXXMASM\nMSAMXMSMSA\nAMXSXMAAMM\nMSAMASMSMX\nXMASAMXAMM\nXXAMMXXAMA\nSMSMSASXSS\nSAXAMASAAA\nMAMMMXMMMM\nMXMXAXMASX",
                "18",
                "9")
        ];
    }
    
    private int _width = 0;   
    private int _height = 0;   
    private char[] _grid =[];
}                            