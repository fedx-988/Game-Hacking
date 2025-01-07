using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EntityManager.Game_Offsets
{
    public static class Offsets // we don't really need an instance of the offsets class so we make it static
    {
        // relative to ac_client.exe
        public static int localPlayer = 0x00195404;
        public static int entityList = 0x0018AC04; // 0x4 in hex between each entity

        // relative to entity
        public static int health = 0xEC;
        public static int username = 0x205;
        public static int player_count = 0x18AC0C;
        public static int x_coord = 0x4;
        public static int y_coord = 0x8;
        public static int z_coord = 0xC;

    }
}
