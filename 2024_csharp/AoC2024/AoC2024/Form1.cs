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
            run_day(new Day02(), logger);
        };

        backgroundWorker.ProgressChanged += (sender, args) =>
        {
            var logs = args.UserState as string[];

            foreach (var log in logs)
            {
                terminal.AppendText(log + "\n");
            }
        };
        
        backgroundWorker.WorkerReportsProgress = true;
        
        backgroundWorker.RunWorkerAsync();
    }

    private void flush_logger(Logger logger)
    {
        var logs = logger.Flush();
        backgroundWorker.ReportProgress(0, logs);
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
            logger.Info("--------------------\nSeems parts of the solution aren't implemented yet:");
            logger.Info(e.Message);
        }
        catch (Exception e)
        {
            logger.Info("--------------------\nUnknown execption raised:");
            logger.Info(e.Message);
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
    
}