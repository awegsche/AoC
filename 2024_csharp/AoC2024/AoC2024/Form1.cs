using System.ComponentModel;

namespace AoC2024;

public partial class Form1 : Form
{
    public Form1()
    {
        InitializeComponent();

        _backgroundWorker.DoWork += (sender, _) =>
        {
            var logger = new Logger();
            //run_day(new Day01(), logger);
            //run_day(new Day02(), logger);
            //run_day(new Day03(), logger);
            //run_day(new Day04(), logger);
            run_day(new Day05(), logger);
            
        };

        _backgroundWorker.ProgressChanged += (sender, args) =>
        {
            var log = args.UserState as LogMsg;

            if (log == null) return;
            switch (log.Type)
            {
                case LogMsg.MessageType.Info:
                    terminal.SelectionFont = _infoFont;
                    terminal.SelectionColor = _infoColor;
                    break;
                case LogMsg.MessageType.Warning:
                    terminal.SelectionFont = _warnFont;
                    terminal.SelectionColor = _infoColor;
                    break;
                case LogMsg.MessageType.Error:
                    terminal.SelectionFont = _warnFont;
                    terminal.SelectionColor = _errorColor;
                    break;
                case LogMsg.MessageType.Solution:
                    terminal.SelectionFont = _solutionFont;
                    terminal.SelectionColor = _solutionColor;
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }

            terminal.AppendText(log.Message + "\n");
        };
        
        _backgroundWorker.WorkerReportsProgress = true;
        
        _backgroundWorker.RunWorkerAsync();
    }

    private void flush_logger(Logger logger)
    {
        var logs = logger.Flush();
        foreach (var log in logs)
        {
            Thread.Sleep(200);
            _backgroundWorker.ReportProgress(0, log);
        }
    }

    private void run_day(AoCSolution day, Logger logger)
    {
        logger.Info($"\n===============================\n= Day {day.Day,2} : {day.Title,18} =\n===============================");
        try
        {
            if (day.run_tests(logger))
            {
                logger.Info("All tests passed!\n");
            }
            else
            {
                logger.Info("Tests FAILED\n");
            }

        }
        catch (NotImplementedException e)
        {
            logger.Error("--------------------\nSeems parts of the solution aren't implemented yet:");
            logger.Error(e.Message);
        }
        catch (Exception e)
        {
            logger.Error("--------------------\nUnknown execption raised:");
            logger.Error(e.Message);
        }
        flush_logger(logger);

        try
        {
            day.run_day(logger);
        }
        catch (Exception e)
        {
            logger.Error("-------------------\nERROR:");
            logger.Error(e.Message);
        }
        flush_logger(logger);
    }

    private readonly BackgroundWorker _backgroundWorker = new BackgroundWorker();
    private readonly Font _infoFont = new Font("Iosevka Term", 12, FontStyle.Regular);
    private readonly Font _warnFont = new Font("Iosevka Term", 12, FontStyle.Regular);
    private readonly Font _solutionFont = new Font("Iosevka Term", 12, FontStyle.Bold);
    
    private readonly Color _errorColor = Color.Red;
    private readonly Color _infoColor = Color.Green;
    private readonly Color _solutionColor = Color.Yellow;

}