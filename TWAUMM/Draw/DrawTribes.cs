using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Drawing.Processing;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using TWAUMM.Tribes;
using TWAUMM.Utility;

namespace TWAUMM.Draw
{
    public class DrawTribes
    {
        private static void DrawTribesMap(Image img, Dictionary<UInt64, Tribe> tribes, Rgba32 borderColor, float alpha = 0.0f, Func<Tribe, string>? sidebarTopTextFunc = null, Func<Tribe, string>? sidebarBottomTextFunc = null)
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

            foreach (KeyValuePair<UInt64, Tribe> tribe in tribes)
            {
                var configColor = configInfo?.colors[(Int32)tribe.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2], alpha);
                foreach (var player in tribe.Value.players)
                {
                    Common.DrawPlayerVillages(img, player.villages, zoom, 1, borderColor);
                    Common.DrawPlayerVillages(img, player.villages, zoom, 0, color);
                }
                if (sidebarTopTextFunc != null && sidebarBottomTextFunc != null)
                {
                    Common.DrawImageTopInformation<Tribe>(img, tribe.Value.name, tribe.Value, sidebarTopTextFunc, sidebarBottomTextFunc, tribe.Key - 1, color);
                }
            }

            Common.DrawKontinentLines(img, worldLength, kLength, partialK);
        }

        private static void DrawTopTribesMap(string world, Dictionary<UInt64, Tribe> tribes, Func<Tribe, string> sidebarTopTextFunc, Func<Tribe, string> sidebarBottomTextFunc, string mapName, string filename)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            var configInfo = Config.GetInstance().GetConfigInfo();
            DrawTribesMap(
                img,
                tribes,
                Common.charcoalColor,
                Common.noAlpha,
                sidebarTopTextFunc,
                sidebarBottomTextFunc
            );
            Common.DrawImageHeader(img, world, mapName);
            string outputFile = configInfo?.outputDir + "/" + world + "/" + filename + ".png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopTribes(string world)
        {
            DrawTopTribesMap(
                world,
                Tribes.Tribes.GetInstance().GetTopTribes(),
                (Tribe tribe) => tribe.totalPoints.ToString("N0") + " points",
                (Tribe tribe) => tribe.villageCount.ToString("N0") + " villages",
                "Top Tribes Map",
                "topTribes"
            );
        }

        public static void DrawTopODTribes(string world)
        {
            DrawTopTribesMap(
                world,
                Tribes.Tribes.GetInstance().GetTopODTribes(),
                (Tribe tribe) => tribe.od.ToString("N0") + " OD",
                (Tribe tribe) => tribe.villageCount.ToString("N0") + " villages",
                "Top OD Tribes Map",
                "topODTribes"
            );
        }

        public static void DrawTopODATribes(string world)
        {
            DrawTopTribesMap(
                world,
                Tribes.Tribes.GetInstance().GetTopODATribes(),
                (Tribe tribe) => tribe.oda.ToString("N0") + " ODA",
                (Tribe tribe) => tribe.villageCount.ToString("N0") + " villages",
                "Top ODA Tribes Map",
                "topODATribes"
            );
        }

        public static void DrawTopODDTribes(string world)
        {
            DrawTopTribesMap(world,
                Tribes.Tribes.GetInstance().GetTopODDTribes(),
                (Tribe tribe) => tribe.odd.ToString("N0") + " ODD",
                (Tribe tribe) => tribe.villageCount.ToString("N0") + " villages",
                "Top ODD Tribes Map",
                "topODDTribes"
            );
        }

        public static void DrawTopConquerTribes(string world)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            DrawTribesMap(
                img,
                Tribes.Tribes.GetInstance().GetTopConquerTribes(),
                Common.alphaCharcoalColor,
                Common.alpha
            );

            var configInfo = Config.GetInstance().GetConfigInfo();
            var zoom = Villages.Villages.GetInstance().GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = 100 * zoom;
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            foreach (var tribe in Tribes.Tribes.GetInstance().GetTopConquerTribes())
            {
                var configColor = configInfo?.colors[(Int32)tribe.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2]);
                foreach (var player in tribe.Value.players)
                {
                    Common.DrawPlayerVillages(img, player.conquers, zoom, 2, Common.charcoalColor);
                    Common.DrawPlayerVillages(img, player.conquers, zoom, 1, color);
                }
                Common.DrawImageTopInformation(
                    img,
                    tribe.Value.name,
                    tribe.Value,
                    (Tribe tribe) => tribe.conquerPoints.ToString("N0") + " points",
                    (Tribe tribe) => tribe.conquers.Count.ToString("N0") + " villages",
                    tribe.Key - 1,
                    color
                );
            }

            Common.DrawImageHeader(img, world, "Top Nobling Tribes Map");
            string outputFile = configInfo?.outputDir + "/" + world + "/topConqTribes.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopLossTribes(string world)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            DrawTribesMap(img, Tribes.Tribes.GetInstance().GetTopLossTribes(), Common.alphaCharcoalColor, Common.alpha);

            var configInfo = Config.GetInstance().GetConfigInfo();
            var zoom = Villages.Villages.GetInstance().GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = 100 * zoom;
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            foreach (var tribe in Tribes.Tribes.GetInstance().GetTopLossTribes())
            {
                var configColor = configInfo?.colors[(Int32)tribe.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2]);
                foreach (var player in tribe.Value.players)
                {
                    Common.DrawPlayerVillages(img, player.conquers, zoom, 2, Common.charcoalColor);
                    Common.DrawPlayerVillages(img, player.conquers, zoom, 1, color);
                }
                Common.DrawImageTopInformation(
                    img,
                    tribe.Value.name,
                    tribe.Value,
                    (Tribe tribe) => tribe.lossPoints.ToString("N0") + " points",
                    (Tribe tribe) => tribe.losses.Count.ToString("N0") + " villages",
                    tribe.Key - 1,
                    color
                );
            }

            Common.DrawImageHeader(img, world, "Top Nobled Tribes Map");
            string outputFile = configInfo?.outputDir + "/" + world + "/topLossTribes.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }
    }
}