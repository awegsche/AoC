namespace AoC2024;

public interface ICommand
{
    public int Eval();
}

public struct Mul(int x, int y): ICommand
{
    public int X = x;
    public int Y = y;

    public int Eval() => X * Y;

    public override string ToString() => $"{X} * {Y}";
}

public struct Dont : ICommand
{
    public int Eval() => 0;
    
    public override string ToString() => "Don't()";
}

public struct Do : ICommand
{
    public int Eval() => 0;
    
    public override string ToString() => "Do()";
}

public struct OtherCommand : ICommand
{
    private Token[] tokens;
    public int Eval() => 0;
    
    public override string ToString() => "OtherCommand()";
}

public struct Token
{
    public enum TokenKind
    {
        Word,
        Number,
        ParenthOpen,
        ParenthClose,
        Comma,
        Other,
    }

    public readonly TokenKind Kind;
    public readonly string Value;

    public Token(TokenKind kind, string value)
    {
        this.Kind = kind;
        this.Value = value;
    }
}


public class EnumIterator<T>(IEnumerator<T> enumerator)
{
    public T Current;
    public bool AtEnd { get; private set; }
    private IEnumerator<T> _enumerator = enumerator;


    public bool Next()
    {
        AtEnd = !_enumerator.MoveNext();
        if (AtEnd) return false;
        Current = _enumerator.Current;
        return true;
    }
}

public class CharIterator(CharEnumerator enumerator)
{
    public char Current;
    public bool AtEnd { get; private set; }
    private CharEnumerator _enumerator = enumerator;


    public bool Next()
    {
        AtEnd = !_enumerator.MoveNext();
        if (AtEnd) return false;
        Current = _enumerator.Current;
        return true;
    }
}

public class Day03 : AoCSolution
{
    public override string Day => "03";
    public override string Title => "Mull It Over";

    public static Token read_token(ref CharIterator enumerator)
    {
        var word = read_word(ref enumerator);
        if (word != null) return new Token(Token.TokenKind.Word, word);
        var number = read_int(ref enumerator);
        if (number != null) return new Token(Token.TokenKind.Number, number);
        var c = enumerator.Current;
        enumerator.Next();
        return c switch
        {
            '(' => new Token(Token.TokenKind.ParenthOpen, ""),
            ')' => new Token(Token.TokenKind.ParenthClose, ""),
            ',' => new Token(Token.TokenKind.Comma, ""),
            _ => new Token(Token.TokenKind.Other, c.ToString())
        };
    }

    public static bool next_kind(ref EnumIterator<Token> enumerator, Token.TokenKind kind)
    {
        if (!enumerator.Next()) return false;
        return enumerator.Current.Kind == kind;
    }
    
    public static bool this_kind(ref EnumIterator<Token> enumerator, Token.TokenKind kind)
    {
        return enumerator.Current.Kind == kind;
    }

    private static ICommand Parse(ref EnumIterator<Token> enumerator)
    {
        if (next_kind(ref enumerator, Token.TokenKind.Word))
        {
            if (enumerator.Current.Value.EndsWith("mul"))
            {
                var mul = read_mul(ref enumerator);
                if (mul != null) return mul.Value;
            }
            else if (enumerator.Current.Value.EndsWith("don't"))
            {
                if (!next_kind(ref enumerator, Token.TokenKind.ParenthOpen)) return new OtherCommand();
                if (!next_kind(ref enumerator, Token.TokenKind.ParenthClose)) return new OtherCommand();
                return new Dont();
            }
            else if (enumerator.Current.Value.EndsWith("do"))
            {
                if (!next_kind(ref enumerator, Token.TokenKind.ParenthOpen)) return new OtherCommand();
                if (!next_kind(ref enumerator, Token.TokenKind.ParenthClose)) return new OtherCommand();
                return new Do();
            }
        }
        return new OtherCommand();
    }
    

    private static Mul? read_mul(ref EnumIterator<Token> enumerator)
    {
        if (!next_kind(ref enumerator, Token.TokenKind.ParenthOpen)) return null;
        if (!next_kind(ref enumerator, Token.TokenKind.Number)) return null;

        var x = int.Parse(enumerator.Current.Value);
        if (!next_kind(ref enumerator, Token.TokenKind.Comma)) return null;
        if (!next_kind(ref enumerator, Token.TokenKind.Number)) return null;
        var y = int.Parse(enumerator.Current.Value);
        if (!next_kind(ref enumerator, Token.TokenKind.ParenthClose)) return null;
        return new Mul(x, y);
    }

    public static string? read_word(ref CharIterator enumerator)
    {
        if (!char.IsAsciiLetter(enumerator.Current) && enumerator.Current != '_')
            return null;

        string word = "";
        word += enumerator.Current;
        while (enumerator.Next() && (char.IsAsciiLetter(enumerator.Current) || enumerator.Current == '_' || enumerator.Current == '\''))
            word += enumerator.Current;
        return word;
    }

    public static string? read_int(ref CharIterator enumerator)
    {
        if (!char.IsDigit(enumerator.Current)) return null;

        var result = "";
        result += enumerator.Current;

        while (enumerator.Next() && char.IsDigit(enumerator.Current))
            result += enumerator.Current;

        return result;
    }

    private List<ICommand> getCommands(IEnumerable<string> input)
    {
        var tokens = new List<Token>();
        foreach (var line in input)
        {
            var enumerator = new CharIterator(line.GetEnumerator());
            enumerator.Next();

            while (!enumerator.AtEnd)
            {
                tokens.Add(read_token(ref enumerator));
            }
        }

        var cmds = new List<ICommand>();
        var iterator = new EnumIterator<Token>(tokens.GetEnumerator());
        while (!iterator.AtEnd)
        {
            cmds.Add(Parse(ref iterator));
        }
        return cmds;
    }

    public override string part1(IEnumerable<string> input, Logger logger)
    {

        return getCommands(input).Select(cmd =>
        {
            //logger.Info(cmd.ToString());
            return cmd.Eval();
        }).Sum().ToString();
    }

    public override string part2(IEnumerable<string> input, Logger logger)
    {
        int result = 0;
        bool active = true;

        foreach (var cmd in getCommands(input))
        {
            switch (cmd)
            {
                case Dont:
                    active = false;
                    break;
                case Do:
                    active = true;
                    break;
                case Mul:
                    if (active) result += cmd.Eval();
                    break;
            }
        }

        return result.ToString();
    }

    public override TestCase[] tests()
    {
        return
        [
            new TestCase("xmul(2,4)&mul[3,7]!^don't()_mul(5,5)+mul(32,64](mul(11,8)undo()?mul(8,5))", "161", "48")
        ];
    }
}