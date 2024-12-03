namespace AoC2024;

public class LogMsg(string message, LogMsg.MessageType type = LogMsg.MessageType.Info)
{
   public enum MessageType
   {
      Info,
      Warning,
      Error,
      Solution
   }

   public string Message { get; private set; } = message;
   public MessageType Type { get; private set; } = type;
}

public class Logger
{
   private List<LogMsg> _log = []; 
   
   public void Info(string message) => _log.Add(new LogMsg(message));
   public void Solution(string message) => _log.Add(new LogMsg(message, LogMsg.MessageType.Solution));
   public void Error(string message) => _log.Add(new LogMsg(message, LogMsg.MessageType.Error));

   public LogMsg[] Flush()
   {
      var oldLog = _log.ToArray();
      _log = [];
      return oldLog;
   }
}