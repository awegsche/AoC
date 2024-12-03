namespace AoC2024;

public class Logger
{
   private List<string> _log = []; 
   
   public void Info(string message) => _log.Add(message);

   public string[] Flush()
   {
      var oldLog = _log.ToArray();
      _log = [];
      return oldLog;
   }
}