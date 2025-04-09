namespace UpdateAMOLED
{
    internal class Program
    {
        static void Main(string[] args)
        {
            var telemetry = new UpdateTelemetry();
            while (true)
            {
                var r = telemetry.GetAllTelemetry();
                r.Wait();
            }
            Console.ReadLine();
        }
    }
}
