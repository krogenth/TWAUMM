using SixLabors.Fonts;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Drawing.Processing;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using TWAUMM.Villages;
using TWAUMM.Utility;

namespace TWAUMM.Draw
{
    public class DrawVillages
    {
        public static void DrawBarbarians(string world)
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

            var villages = Villages.Villages.Instance.GetBarbarianVillages();

            Common.DrawPlayerVillages(img, villages, zoom, 1, Common.charcoalColor);
            Common.DrawPlayerVillages(img, villages, zoom, 0, Common.greyColor);

            Common.DrawKontinentDetails(img, worldLength, kLength, partialK);
            Common.DrawNoSidebarImageHeader(img, world, "Barbarian Villages");

            string outputFile = configInfo?.outputDir + "/" + world + "/barbarians.png";
            img.SaveAsPng(outputFile);
            img.Dispose();
        }
    }
}