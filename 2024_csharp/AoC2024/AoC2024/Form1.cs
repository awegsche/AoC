using System.ComponentModel;
using Microsoft.VisualBasic.Logging;

namespace AoC2024;

public partial class Form1 : Form
{
    public Form1()
    {
        InitializeComponent();
        
        terminal.Text = "Hello World!\n";

        backgroundWorker.DoWork += (sender, args) =>
        {
            Logger logger = new Logger();
            //run_day(new Day01(), logger);
            run_day(new Day03(), logger);
        };

        backgroundWorker.ProgressChanged += (sender, args) =>
        {
            var log = args.UserState as LogMsg;

            if (log == null) return;
            switch (log.Type)
            {
                case LogMsg.MessageType.Info:
                    terminal.SelectionFont = InfoFont;
                    terminal.SelectionColor = InfoColor;
                    break;
                case LogMsg.MessageType.Warning:
                    terminal.SelectionFont = WarnFont;
                    terminal.SelectionColor = InfoColor;
                    break;
                case LogMsg.MessageType.Error:
                    terminal.SelectionFont = WarnFont;
                    terminal.SelectionColor = ErrorColor;
                    break;
                case LogMsg.MessageType.Solution:
                    terminal.SelectionFont = SolutionFont;
                    terminal.SelectionColor = SolutionColor;
                    break;
                default:
                    throw new ArgumentOutOfRangeException();
            }

            terminal.AppendText(log?.Message + "\n");
        };
        
        backgroundWorker.WorkerReportsProgress = true;
        
        backgroundWorker.RunWorkerAsync();
    }

    private void flush_logger(Logger logger)
    {
        var logs = logger.Flush();
        foreach (var log in logs)
        {
            Thread.Sleep(10);
            backgroundWorker.ReportProgress(0, log);
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
            logger.Info("-------------------\nERROR:");
            logger.Info(e.Message);
        }
        flush_logger(logger);
    }

    private readonly BackgroundWorker backgroundWorker = new BackgroundWorker();
    private readonly Font InfoFont = new Font("Iosevka Term", 12, FontStyle.Regular);
    private readonly Font WarnFont = new Font("Iosevka Term", 12, FontStyle.Regular);
    private readonly Font SolutionFont = new Font("Iosevka Term", 12, FontStyle.Bold);
    
    private readonly Color ErrorColor = Color.Red;
    private readonly Color InfoColor = Color.Green;
    private readonly Color SolutionColor = Color.Yellow;

}