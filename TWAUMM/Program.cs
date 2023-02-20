﻿using TWAUMM.Utility;
using TWAUMM.Draw;

namespace TWAUMM.Program
{
    partial class Program
    {
        static void Main(string[] args)
        {
            var config = Config.GetInstance();

            if (!Directory.Exists(config.GetConfigInfo().outputDir))
            {
                Directory.CreateDirectory(config.GetConfigInfo().outputDir);
            }

            while (true)
            {
                DelayToNextDay();
                config.RefreshConfig("./config.json");
                IterateWorlds();
            }
        }

        /// <summary>
        /// Pause execution until the next day
        /// </summary>
        static void DelayToNextDay()
        {
            var sleepTime = DateTime.Today.AddDays(1).Subtract(DateTime.Now);
            Thread.Sleep((int)sleepTime.TotalMilliseconds);
        }

        /// <summary>
        /// Cycles through all worlds in the config to generate maps
        /// </summary>
        static void IterateWorlds()
        {
            var config = Config.GetInstance();

            config.GetConfigInfo()?.worlds?.ForEach(world =>
            {
                
                var worldTag = WorldTag.GetWorldTag(world);
                var worldPath = config.GetConfigInfo()?.outputDir + "/" + worldTag;
                var worldUrl = WorldToUrl.GetUrl(world);

                if (!Directory.Exists(worldPath))
                {
                    Directory.CreateDirectory(worldPath);
                }

                var tribes = Tribes.Tribes.GetInstance();
                tribes.ReadTribeData(worldUrl);

                var players = Players.Players.GetInstance();
                players.ReadPlayerData(worldUrl);

                var villages = Villages.Villages.GetInstance();
                villages.ReadVillageData(worldUrl);

                Conquers.Conquers.ReadConquerData(worldUrl);

                Draws.DrawMaps(worldUrl);
            });
        }
    }
}