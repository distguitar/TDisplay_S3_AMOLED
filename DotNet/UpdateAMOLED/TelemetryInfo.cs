using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace UpdateAMOLED
{
    internal class TelemetryInfo
    {
        public int CPU { get; set; }
        public int RAM { get; set; }
        public int LAN { get; set; }
        public int CDrive { get; set; }
        public int DDrive { get; set; }
        public int FDrive { get; set; }
        public int GDrive { get; set; }
    }
}
