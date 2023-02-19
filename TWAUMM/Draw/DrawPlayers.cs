using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Drawing.Processing;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using System.Numerics;
using TWAUMM.Players;
using TWAUMM.Utility;

namespace TWAUMM.Draw
{
    public class DrawPlayers
    {
        private static void DrawPlayersMap(Image img, Dictionary<UInt64, Player> players, Rgba32 borderColor, float alpha = 0.0f, bool drawSidebar = true)
        {
            var configInfo = Config.GetInstance().GetConfigInfo();
            var zoom = Villages.Villages.GetInstance().GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = 100 * zoom;
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            // fill in primary areas for drawing
            var headerRect = new Rectangle(0, 0, 1250, 30);
            var backgroundRect = new Rectangle(0, 30, 1000, 1000);
            var sidebarRect = new Rectangle(1000, 30, 250, 1000);
            img.Mutate(x => x.Fill(Common.blackColor, headerRect));
            img.Mutate(x => x.Fill(Common.backgroundColor, backgroundRect));
            img.Mutate(x => x.Fill(Common.whiteColor, sidebarRect));

            foreach (KeyValuePair<UInt64, Player> player in players)
            {
                var configColor = configInfo?.colors[(Int32)player.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2], alpha);
                Common.DrawPlayerVillages(img, player.Value.villages, zoom, 1, borderColor);
                Common.DrawPlayerVillages(img, player.Value.villages, zoom, 0, color);
                if (drawSidebar)
                {
                    Common.DrawImageTopInformation(img, player.Value.name, player.Value.points, (UInt64)player.Value.villages.Count, player.Key - 1, color);
                }
            }

            Common.DrawKontinentLines(img, worldLength, kLength, partialK);
        }

        private static void DrawTopPlayersMap(string world, Dictionary<UInt64, Player> players, string mapName, string filename)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            var configInfo = Config.GetInstance().GetConfigInfo();
            DrawPlayersMap(img, players, Common.charcoalColor, Common.noAlpha, true);
            Common.DrawImageHeader(img, world, mapName);
            string outputFile = configInfo?.outputDir + "/" + world + "/" + filename + ".png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopPlayers(string world)
        {
            DrawTopPlayersMap(world, Players.Players.GetInstance().GetTopPlayers(), "Top Players Map", "topPlayers");
        }

        public static void DrawTopODPlayers(string world)
        {
            DrawTopPlayersMap(world, Players.Players.GetInstance().GetTopODPlayers(), "Top OD Players Map", "topODPlayers");
        }

        public static void DrawTopODAPlayers(string world)
        {
            DrawTopPlayersMap(world, Players.Players.GetInstance().GetTopODAPlayers(), "Top ODA Players Map", "topODAPlayers");
        }

        public static void DrawTopODDPlayers(string world)
        {
            DrawTopPlayersMap(world, Players.Players.GetInstance().GetTopODDPlayers(), "Top ODD Players Map", "topODDPlayers");
        }

        public static void DrawTopConqPlayers(string world)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            DrawPlayersMap(img, Players.Players.GetInstance().GetTopConqerPlayers(), Common.alphaCharcoalColor, Common.alpha, false);

            var configInfo = Config.GetInstance().GetConfigInfo();
            var zoom = Villages.Villages.GetInstance().GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = 100 * zoom;
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            foreach (var player in Players.Players.GetInstance().GetTopConqerPlayers())
            {
                var configColor = configInfo?.colors[(Int32)player.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2]);
                Common.DrawPlayerVillages(img, player.Value.conquers, zoom, 2, Common.charcoalColor);
                Common.DrawPlayerVillages(img, player.Value.conquers, zoom, 1, color);
                Common.DrawImageTopInformation(img, player.Value.name, player.Value.conquerPoints, (UInt64)player.Value.conquers.Count, player.Key - 1, color);
            }
            Common.DrawImageHeader(img, world, "Top Nobling Players Map");
            string outputFile = configInfo?.outputDir + "/" + world + "/topConqPlayers.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopLossPlayers(string world)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            DrawPlayersMap(img, Players.Players.GetInstance().GetTopLossPlayers(), Common.alphaCharcoalColor, Common.alpha, false);

            var configInfo = Config.GetInstance().GetConfigInfo();
            var zoom = Villages.Villages.GetInstance().GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = 100 * zoom;
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            foreach (var player in Players.Players.GetInstance().GetTopLossPlayers())
            {
                var configColor = configInfo?.colors[(Int32)player.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2]);
                Common.DrawPlayerVillages(img, player.Value.losses, zoom, 2, Common.charcoalColor);
                Common.DrawPlayerVillages(img, player.Value.losses, zoom, 1, color);
                Common.DrawImageTopInformation(img, player.Value.name, player.Value.lossPoints, (UInt64)player.Value.losses.Count, player.Key - 1, color);
            }

            Common.DrawImageHeader(img, world, "Top Nobled Players Map");
            string outputFile = configInfo?.outputDir + "/" + world + "/topLossPlayers.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }
    }
}