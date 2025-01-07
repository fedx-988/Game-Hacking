using System;
using System.Collections.Generic;
using Swed32;
using EntityManager;
using EntityManager.Entity_Handling;
using EntityManager.Entity_Structures;

class Program
{
    static void Main(string[] args)
    {
        Swed swed = new Swed("ac_client");
        IntPtr moduleBase = swed.GetModuleBase(".exe");

        ACEntityManager aCEntityManager = new ACEntityManager(swed, moduleBase);

        // Update entities and local player
        aCEntityManager.UpdateEntities();
        aCEntityManager.UpdateLocalPlayer();

        // Get and display the player count
        int playerCount = aCEntityManager.GetPlayerCount();
        Console.WriteLine($"Player Count: {playerCount}");
        Console.WriteLine("-------------------------------");

        // Set to track unique usernames
        HashSet<string> seenUsernames = new HashSet<string>();

        // Display details for each entity
        foreach (Entity entity in aCEntityManager.GetEntities())
        {
            // Only display entities with a valid username
            if (!string.IsNullOrEmpty(entity.username))
            {
                // If the username is unique or the base address is different
                if (!seenUsernames.Contains(entity.username) || !seenUsernames.Contains(entity.baseAddress.ToString("X")))
                {
                    Console.WriteLine($"Base: {entity.baseAddress.ToString("X")} ||| Health: {entity.health} ||| Username: {entity.username} ||| Coordinates: X: {entity.x} || Y: {entity.y} || Z: {entity.z}");
                    seenUsernames.Add(entity.username); // Mark username as seen
                    seenUsernames.Add(entity.baseAddress.ToString("X")); // Mark base address as seen
                }
            }
        }

    }
}
