using TWAUMM.Utility;
using TWAUMM.Draw;

namespace TWAUMM.Program
{
    partial class Program
    {
        static void Main(string[] args)
        {
            var config = Config.Instance;

            while (true)
            {
                DelayToNextDay();
                config.RefreshConfig();
                if (!Directory.Exists(config.GetConfigInfo()?.outputDir))
                {
                    Directory.CreateDirectory(config.GetConfigInfo()?.outputDir);
                }
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
            var config = Config.Instance;

            config.GetConfigInfo()?.worlds?.ForEach(world =>
            {
                var worldTag = WorldTag.GetWorldTag(world.world);
                var worldPath = config.GetConfigInfo()?.outputDir + "/" + worldTag;
                var worldUrl = WorldToUrl.GetUrl(world.world);

                if (!FileManager.WorldExistsOnDisk(worldPath))
                {
                    FileManager.CreateDirectory(worldPath);
                }
                else
                {
                    FileManager.CopyExistingFilesToBackup(worldPath);
                }

                var tribes = Tribes.Tribes.Instance;
                tribes.ReadTribeData(worldUrl);

                var players = Players.Players.Instance;
                players.ReadPlayerData(worldUrl);

                var villages = Villages.Villages.Instance;
                villages.ReadVillageData(worldUrl);

                Conquers.Conquers.ReadConquerData(worldUrl, world.duration);

                Draws.DrawMaps(worldTag, world.duration);

                tribes.ClearTribeData();
                players.ClearPlayerData();
                villages.ClearVillageData();
            });
        }
    }
}