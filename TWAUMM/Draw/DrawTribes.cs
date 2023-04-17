using SixLabors.Fonts;
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
            var configInfo = Config.Instance.GetConfigInfo();
            var zoom = Villages.Villages.Instance.GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = (UInt64)(100.0f * zoom);
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
                }
                foreach (var player in tribe.Value.players)
                {
                    Common.DrawPlayerVillages(img, player.villages, zoom, 0, color);
                }
                if (sidebarTopTextFunc != null && sidebarBottomTextFunc != null)
                {
                    Common.DrawImageTopInformation<Tribe>(img, tribe.Value.name, tribe.Value, sidebarTopTextFunc, sidebarBottomTextFunc, tribe.Key - 1, color);
                }
            }

            Common.DrawKontinentDetails(img, worldLength, kLength, partialK);
        }

        private static void DrawTopTribesMap(string world, Dictionary<UInt64, Tribe> tribes, Func<Tribe, string> sidebarTopTextFunc, Func<Tribe, string> sidebarBottomTextFunc, string mapName, string filename)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            var configInfo = Config.Instance.GetConfigInfo();
            DrawTribesMap(
                img,
                tribes,
                Common.charcoalColor,
                Common.noAlpha,
                sidebarTopTextFunc,
                sidebarBottomTextFunc
            );
            Common.DrawTopImageHeader(img, world, mapName);
            string outputFile = configInfo?.outputDir + "/" + world + "/" + filename + ".png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopTribes(string world)
        {
            DrawTopTribesMap(
                world,
                Tribes.Tribes.Instance.GetTopTribes(),
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
                Tribes.Tribes.Instance.GetTopODTribes(),
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
                Tribes.Tribes.Instance.GetTopODATribes(),
                (Tribe tribe) => tribe.oda.ToString("N0") + " ODA",
                (Tribe tribe) => tribe.villageCount.ToString("N0") + " villages",
                "Top ODA Tribes Map",
                "topODATribes"
            );
        }

        public static void DrawTopODDTribes(string world)
        {
            DrawTopTribesMap(world,
                Tribes.Tribes.Instance.GetTopODDTribes(),
                (Tribe tribe) => tribe.odd.ToString("N0") + " ODD",
                (Tribe tribe) => tribe.villageCount.ToString("N0") + " villages",
                "Top ODD Tribes Map",
                "topODDTribes"
            );
        }

        public static void DrawTopConquerTribes(string world, uint duration)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            DrawTribesMap(
                img,
                Tribes.Tribes.Instance.GetTopConquerTribes(),
                Common.alphaCharcoalColor,
                Common.alpha
            );

            var configInfo = Config.Instance.GetConfigInfo();
            var zoom = Villages.Villages.Instance.GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = (UInt64)(100.0f * zoom);
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            foreach (var tribe in Tribes.Tribes.Instance.GetTopConquerTribes())
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

            Common.DrawTopImageHeader(img, world, "Top Nobling Tribes Map (" + duration + "days)");
            string outputFile = configInfo?.outputDir + "/" + world + "/topConqTribes.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopLossTribes(string world, uint duration)
        {
            Image img = new Image<Rgba32>(1250, 1030);
            DrawTribesMap(img, Tribes.Tribes.Instance.GetTopLossTribes(), Common.alphaCharcoalColor, Common.alpha);

            var configInfo = Config.Instance.GetConfigInfo();
            var zoom = Villages.Villages.Instance.GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = (UInt64)(100.0f * zoom);
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            foreach (var tribe in Tribes.Tribes.Instance.GetTopLossTribes())
            {
                var configColor = configInfo?.colors[(Int32)tribe.Key - 1];
                Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2]);
                foreach (var player in tribe.Value.players)
                {
                    Common.DrawPlayerVillages(img, player.conquers, zoom, 2, Common.charcoalColor);
                }
                foreach (var player in tribe.Value.players)
                {
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

            Common.DrawTopImageHeader(img, world, "Top Nobled Tribes Map (" + duration + "days)");
            string outputFile = configInfo?.outputDir + "/" + world + "/topLossTribes.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }

        public static void DrawTopKontinentTribes(string world)
        {
            Image img = new Image<Rgba32>(1000, 1030);
            var configInfo = Config.Instance.GetConfigInfo();
            var zoom = Villages.Villages.Instance.GetZoom();

            float worldLength = 10.0f / (float)zoom;
            UInt64 worldLengthFloor = (UInt64)Math.Floor(worldLength);
            UInt64 kLength = (UInt64)(100.0f * zoom);
            UInt64 wholeKCount = worldLengthFloor - (worldLengthFloor % 2);
            float partialK = (worldLength - (float)wholeKCount) / 2.0f;

            // fill in primary areas for drawing
            var headerRect = new Rectangle(0, 0, 1250, 30);
            var backgroundRect = new Rectangle(0, 30, 1000, 1000);
            img.Mutate(x => x.Fill(Common.blackColor, headerRect));
            img.Mutate(x => x.Fill(Common.backgroundColor, backgroundRect));

            List<UInt64> drawnTribeRanks = new List<UInt64>();

            var startKontinentNumber = (byte)(Math.Ceiling((10.0f - worldLength) / 2.0f));
            var endKontinentNumber = (byte)(10 - startKontinentNumber);
            var colorIndex = 0;

            for (var yKontinentNumber = startKontinentNumber; yKontinentNumber < endKontinentNumber; yKontinentNumber++)
            {
                for (var xKontinentNumber = startKontinentNumber; xKontinentNumber < endKontinentNumber; xKontinentNumber++)
                {
                    UInt64 kontinent = (UInt64)(yKontinentNumber * 10 + xKontinentNumber);
                    var kontinentTopTribes = Tribes.Tribes.Instance.GetTopKTribes(kontinent);
                    if (kontinentTopTribes.Count <= 0)
                    {
                        continue;
                    }

                    if (!drawnTribeRanks.Contains(kontinentTopTribes[1].rank))
                    {
                        var configColor = configInfo?.colors[colorIndex];
                        Rgba32 color = new Rgba32(configColor[0], configColor[1], configColor[2]);

                        foreach (var player in kontinentTopTribes[1].players)
                        {
                            Common.DrawPlayerVillages(img, player.villages, zoom, 1, Common.charcoalColor);
                        }
                        foreach (var player in kontinentTopTribes[1].players)
                        {
                            Common.DrawPlayerVillages(img, player.villages, zoom, 0, color);
                        }

                        colorIndex++;
                        drawnTribeRanks.Add(kontinentTopTribes[1].rank);
                    }
                }
            }

            Common.DrawKontinentDetails(img, worldLength, kLength, partialK);

            // draw each kontinent's top two tribe percentage information after drawing villages
            // this avoids the possibility that a tribe's villages might overlap the details
            var firstFont = Fonts.Instance.GetFont("Arial Unicode MS", 10 + zoom, FontStyle.Regular);
            var secondFont = Fonts.Instance.GetFont("Arial Unicode MS", 8 + zoom, FontStyle.Regular);
            for (var yKontinentNumber = startKontinentNumber; yKontinentNumber < endKontinentNumber; yKontinentNumber++)
            {
                for (var xKontinentNumber = startKontinentNumber; xKontinentNumber < endKontinentNumber; xKontinentNumber++)
                {
                    UInt64 kontinent = (UInt64)(yKontinentNumber * 10 + xKontinentNumber);
                    var kontinentTopTribes = Tribes.Tribes.Instance.GetTopKTribes(kontinent);
                    var kontinentTotalPoints = Villages.Villages.Instance.GetKontinentTotalPoints(kontinent);
                    if (kontinentTopTribes.Count <= 0)
                    {
                        continue;
                    }

                    var firstTribePercentage = (float)kontinentTopTribes[1].kontinentTotalPoints[kontinent] * 100.0f / (float)kontinentTotalPoints;
                    var HalfOffscreenKontinents = (10.0f - worldLength) / 2.0f;
                    Common.DrawImageTextWithOutline(
                        img,
                        $"{kontinentTopTribes[1].tag}\n{firstTribePercentage.ToString("0.00")}%",
                        firstFont,
                        new PointF(
                            ((xKontinentNumber - HalfOffscreenKontinents + 0.5f) * kLength),
                            ((yKontinentNumber - HalfOffscreenKontinents + 0.125f) * kLength + 30)
                        ),
                        HorizontalAlignment.Center,
                        TextAlignment.Center,
                        Common.blackColor,
                        Common.blackColor,
                        4.0f
                    );
                    Common.DrawImageTextWithOutline(
                        img,
                        $"{kontinentTopTribes[1].tag}\n{firstTribePercentage.ToString("0.00")}%",
                        firstFont,
                        new PointF(
                            ((xKontinentNumber - HalfOffscreenKontinents + 0.5f) * kLength),
                            ((yKontinentNumber - HalfOffscreenKontinents + 0.125f) * kLength + 30)
                        ),
                        HorizontalAlignment.Center,
                        TextAlignment.Center,
                        Common.whiteColor,
                        Common.whiteColor
                    );

                    // there may not be a second tribe in the kontinent, verify first
                    if (!kontinentTopTribes.ContainsKey(2))
                    {
                        continue;
                    }
                    float secondTribePercentage = (float)kontinentTopTribes[2].kontinentTotalPoints[kontinent] * 100.0f / (float)kontinentTotalPoints;
                    Common.DrawImageTextWithOutline(
                        img,
                        $"{kontinentTopTribes[2].tag}\n{secondTribePercentage.ToString("0.00")}%",
                        secondFont,
                        new PointF(
                            ((xKontinentNumber - HalfOffscreenKontinents + 0.5f) * kLength),
                            ((yKontinentNumber - HalfOffscreenKontinents + 0.125f) * kLength + (50 * zoom) + 30)
                        ),
                        HorizontalAlignment.Center,
                        TextAlignment.Center,
                        Common.blackColor,
                        Common.blackColor,
                        4.0f
                    );
                    Common.DrawImageTextWithOutline(
                        img,
                        $"{kontinentTopTribes[2].tag}\n{secondTribePercentage.ToString("0.00")}%",
                        secondFont,
                        new PointF(
                            ((xKontinentNumber - HalfOffscreenKontinents + 0.5f) * kLength),
                            ((yKontinentNumber - HalfOffscreenKontinents + 0.125f) * kLength + (50 * zoom) + 30)
                        ),
                        HorizontalAlignment.Center,
                        TextAlignment.Center,
                        Common.whiteColor,
                        Common.whiteColor
                    );
                }
            }

            Common.DrawNoSidebarImageHeader(img, world, "Kontinent Dominance Tribes Map");
            string outputFile = configInfo?.outputDir + "/" + world + "/topDominanceTribes.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }
    }
}