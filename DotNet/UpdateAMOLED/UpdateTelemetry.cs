using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.NetworkInformation;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;
using System.Runtime.InteropServices;


namespace UpdateAMOLED
{
    internal class UpdateTelemetry
    {
        const string _URL = "http://192.168.88.249/data";
        public async Task<TelemetryInfo> GetAllTelemetry()
        {
            Console.WriteLine("Gathering system metrics...");
            var ti = new TelemetryInfo();

            ti.CPU = await GetCpuUsageAsync();
            ti.RAM = GetRamUsage();
            ti.LAN = GetNetworkLoad();
            ti.CDrive = GetDriveFreeSpace("C");
            ti.DDrive = GetDriveFreeSpace("D");
            ti.FDrive = GetDriveFreeSpace("F");
            ti.GDrive = GetDriveFreeSpace("G");

            await SendTelemetryToOLED(ti);
            
            return ti;
        }

        private async Task<bool> SendTelemetryToOLED(TelemetryInfo ti)
        {
            using var client = new HttpClient();

            try
            {
                string url = $"{_URL}?r={(int)ti.RAM}&l={(int)ti.LAN}&c={(int)ti.CPU}&dc={(int)ti.CDrive}&dd={(int)ti.DDrive}&df={(int)ti.FDrive}&dg={(int)ti.GDrive}";
                HttpResponseMessage response = await client.GetAsync(url);
                response.EnsureSuccessStatusCode(); // Throws if not 200–299
                string responseBody = await response.Content.ReadAsStringAsync();
                Console.WriteLine("Response:");
                Console.WriteLine(responseBody);
            }
            catch (HttpRequestException ex)
            {
                Console.WriteLine($"Request failed: {ex.Message}");
                return false;
            }
            return true;
        }

        private async Task<int> GetCpuUsageAsync()
        {
            using var cpuCounter = new PerformanceCounter("Processor", "% Processor Time", "_Total");
            _ = cpuCounter.NextValue(); // First call always returns 0
            await Task.Delay(1000); // Wait a second for accurate reading
            return (int)cpuCounter.NextValue();
        }

        private int GetRamUsage()
        {
            using var ramCounter = new PerformanceCounter("Memory", "% Committed Bytes In Use");
            return (int)ramCounter.NextValue();
        }

        private int GetNetworkLoad()
        {
            var interfaces = NetworkInterface.GetAllNetworkInterfaces()
                .Where(i => i.OperationalStatus == OperationalStatus.Up &&
                            i.NetworkInterfaceType != NetworkInterfaceType.Loopback);

            float totalBytes = 0;

            foreach (var ni in interfaces)
            {
                var stats = ni.GetIPv4Statistics();
                totalBytes += stats.BytesReceived + stats.BytesSent;
            }

            Thread.Sleep(1000);

            float totalBytesAfter = 0;

            foreach (var ni in interfaces)
            {
                var stats = ni.GetIPv4Statistics();
                totalBytesAfter += stats.BytesReceived + stats.BytesSent;
            }

            float bytesPerSecond = (totalBytesAfter - totalBytes) / 1024f; // in KB/s

            float maxBytesPerSecond = 100_000_000f / 8; // 100 Mbps = 12.5 MBps

            int lanPercent = (int)Math.Round((bytesPerSecond / maxBytesPerSecond) * 100);
            lanPercent = Math.Clamp(lanPercent, 0, 100);

            return lanPercent;
        }

        private int GetDriveFreeSpace(string driveLetter)
        {
            DriveInfo drive = new DriveInfo(driveLetter);
            long total = drive.TotalSize;
            long free = drive.AvailableFreeSpace;

            int usedPercent = (int)Math.Round((1.0 - (double)free / total) * 100);
            return usedPercent;
        }
    }
}
