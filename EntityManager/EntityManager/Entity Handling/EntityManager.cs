using EntityManager.Entity_Structures;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EntityManager.Entity_Handling
{
    public abstract class EntityManager // this will be our base structure for managing entities
    {
        protected Entity localPlayer = new Entity(); // our player
        protected List<Entity> entities = new List<Entity>(); // all other entites excluding the local player


        // methods we want to define in a uniqe way each time

        public abstract void UpdateEntity(Entity entity); // update any entity
        public abstract void UpdateLocalPlayer(); // different method for the local player only
        public abstract void UpdateEntities(); // Update all of our nice ents

        // methods that we use in almost all cases

        public List<Entity> GetEntities()
        {
            return entities;
        }

        public Entity GetLocalPlayer()
        {
            return localPlayer;
        }


        // here we just build on more and more and more...

    }
}
