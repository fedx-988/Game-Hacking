using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EntityManager.Entity_Structures
{
    public class Entity
    {
        public IntPtr baseAddress { get; set; }
        public int health { get; set; }
        public string username { get; set; }
        public int player_count { get; set; }

        // Coordinates (X, Y, Z)
        public float x { get; set; }
        public float y { get; set; }
        public float z { get; set; }
    }
}
