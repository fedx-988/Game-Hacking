using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using EntityManager.Entity_Structures;
using EntityManager.Game_Offsets;
using Swed32;

namespace EntityManager.Entity_Handling
{
    public class ACEntityManager : EntityManager
    {
        // unique variables for assault cube
        private Swed swed;
        private IntPtr mainModule;

        // constructor because we want fresh data from certain variables

        public ACEntityManager(Swed swedInstance, IntPtr mainModule)
        {
            this.swed = swedInstance;
            this.mainModule = mainModule;
        }

        public string ReadString(IntPtr baseAddress, int offset, int maxLength)
        {
            byte[] buffer = swed.ReadBytes(baseAddress + offset, maxLength); // Read bytes from memory
            int stringLength = Array.IndexOf(buffer, (byte)0); // Find the null terminator

            if (stringLength < 0)
            {
                stringLength = maxLength; // If no null terminator, use the max length
            }

            return Encoding.UTF8.GetString(buffer, 0, stringLength); // Convert bytes to string
        }

        public int GetPlayerCount()
        {
            return swed.ReadInt(mainModule, Offsets.player_count);
        }

        public override void UpdateEntity(Entity entity)
        {
            // Update health as before
            entity.health = swed.ReadInt(entity.baseAddress, Offsets.health);
            entity.username = ReadString(entity.baseAddress, Offsets.username, 16); // Assuming max username length is 16 characters

            // Read and update the coordinates (X, Y, Z)
            entity.x = swed.ReadFloat(entity.baseAddress, Offsets.x_coord);
            entity.y = swed.ReadFloat(entity.baseAddress, Offsets.y_coord);
            entity.z = swed.ReadFloat(entity.baseAddress, Offsets.z_coord);
        }


        public override void UpdateLocalPlayer()
        {
            localPlayer.baseAddress = swed.ReadPointer(mainModule, Offsets.localPlayer);
            UpdateEntity(localPlayer);
            // then specific localplayer stuff after I suppose
        }

        public override void UpdateEntities()
        {
            entities.Clear();

            for (int i = 0; i < 32; i++) //loop through 10 entities
            {
                // get current entity
                IntPtr entityAddress = swed.ReadPointer(mainModule, Offsets.entityList, i * 0x4); // 0x4 between each entity..

                // do example check to see that entity is valid
                if (entityAddress == IntPtr.Zero)
                    continue;

                // create new entity and get its information.
                Entity entity = new Entity();
                entity.baseAddress = entityAddress;

                UpdateEntity(entity);
                entities.Add(entity); // also add to entity list
            } 
        }
    }
}
