using TWAUMM.Utility;
using TWAUMM.Draw;

namespace TWAUMM.Program
{
    partial class Program
    {
        static void Main(string[] args)
        {
            var config = Config.GetInstance();
            config.RefreshConfig("./config.json");

            if (!Directory.Exists(config.GetConfigInfo().outputDir))
            {
                Directory.CreateDirectory(config.GetConfigInfo().outputDir);
            }

            IterateWorlds();

            /*while (true)
            {
                DelayToNextDay();
                config.RefreshConfig("./config.json");
                IterateWorlds();
            }*/
        }

        static void DelayToNextDay()
        {
            var sleepTime = DateTime.Today.AddDays(1).Subtract(DateTime.Now);
            Thread.Sleep((int)sleepTime.TotalMilliseconds);
        }

        static void IterateWorlds()
        {
            var config = Config.GetInstance();

            config.GetConfigInfo()?.worlds?.ForEach(world =>
            {
                
                var worldTag = WorldTag.GetWorldTag(world);
                var worldPath = config.GetConfigInfo().outputDir + "/" + worldTag;
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

                DrawPlayers.DrawTopPlayers(worldTag);
                DrawPlayers.DrawTopODPlayers(worldTag);
                DrawPlayers.DrawTopODAPlayers(worldTag);
                DrawPlayers.DrawTopODDPlayers(worldTag);
                DrawPlayers.DrawTopConqPlayers(worldTag);
                DrawPlayers.DrawTopLossPlayers(worldTag);

                DrawTribes.DrawTopTribes(worldTag);
                DrawTribes.DrawTopODTribes(worldTag);
                DrawTribes.DrawTopODATribes(worldTag);
                DrawTribes.DrawTopODDTribes(worldTag);
                DrawTribes.DrawTopConquerTribes(worldTag);
                DrawTribes.DrawTopLossTribes(worldTag);
            });
        }
    }
}