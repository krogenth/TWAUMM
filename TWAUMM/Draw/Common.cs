﻿using SixLabors.Fonts;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Drawing.Processing;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using TWAUMM.Utility;
using TWAUMM.Villages;

namespace TWAUMM.Draw
{
    public class Common
    {
        public static Rgba32 backgroundColor = new Rgba32(0, 120, 0);
        public static Rgba32 whiteColor = new Rgba32(255, 255, 255);
        public static Rgba32 blackColor = new Rgba32(0, 0, 0);
        public static Rgba32 charcoalColor = new Rgba32(51, 51, 51);
        public static Rgba32 greyColor = new Rgba32(150, 150, 150);
        public static Rgba32 alphaCharcoalColor = new Rgba32(51, 51, 51, 80);
        public static Rgba32 alphaBlackColor = new Rgba32(255, 255, 255, 50);
        public static float noAlpha = 1.0f;
        public static float alpha = 80.0f / 255.0f;

        public static void DrawVillage(Image img, (UInt64, UInt64) coords, float zoom, UInt64 offset, Rgba32 color)
        {
            var x = coords.Item1;
            var y = coords.Item2;

            if (x < 500) { x = (UInt64)Math.Floor(500.0f - ((float)(500 - x) * zoom)); }
            else { x = (UInt64)Math.Floor(500.0f + ((float)(x - 500) * zoom)); }

            if (y < 500) { y = (UInt64)Math.Floor(500.0f - ((float)(500 - y) * zoom)); }
            else { y = (UInt64)Math.Floor(500.0f + ((float)(y - 500) * zoom)); }

            var rect = new Rectangle(
                (int)(x - offset),
                (int)(y - offset + 30),
                (int)(Math.Floor(zoom) + (offset * 2)),
                (int)(Math.Floor(zoom) + (offset * 2))
            );

            img.Mutate(x => x.Fill(color, rect));
        }

        public static void DrawPlayerVillages(Image img, List<Village> villages, float zoom, UInt64 offset, Rgba32 color)
        {
            foreach (var village in villages)
            {
                DrawVillage(img, village.coords, zoom, offset, color);
            }
        }

        private static void DrawImageText(Image img, string text, Font font, PointF origin, HorizontalAlignment alignment, Rgba32 color)
        {
            img.Mutate(x =>
                x.DrawText(
                    new TextOptions(font)
                    {
                        Origin = origin,
                        TabWidth = 4,
                        HorizontalAlignment = alignment
                    },
                    text,
                    Brushes.Solid(color),
                    Pens.Solid(color, 1.0f)
                )
            );
        }

        public static void DrawImageTextWithOutline(Image img, string text, Font font, PointF origin, HorizontalAlignment hAlignment, TextAlignment textAlignment, Rgba32 color, Rgba32 outline, float outlineThickness = 1.0f)
        {
            img.Mutate(x =>
                x.DrawText(
                    new TextOptions(font)
                    {
                        Origin = origin,
                        TabWidth = 4,
                        HorizontalAlignment = hAlignment,
                        VerticalAlignment = VerticalAlignment.Top,
                        TextAlignment = textAlignment
                    },
                    text,
                    Brushes.Solid(color),
                    Pens.Solid(outline, outlineThickness)
                )
            );
        }

        public static void DrawTopImageHeader(Image img, string world, string mapname)
        {
            var font = Fonts.Instance.GetFont("Arial Unicode MS", 14.0f);

            var dateTime = DateTime.UtcNow.ToString("f");

            DrawImageText(img, world, font, new PointF(15, 5), HorizontalAlignment.Left, whiteColor);
            DrawImageText(img, mapname, font, new PointF(200, 5), HorizontalAlignment.Left, whiteColor);
            DrawImageText(img, dateTime, font, new PointF(1000, 5), HorizontalAlignment.Left, whiteColor);
        }

        public static void DrawNoSidebarImageHeader(Image img, string world, string mapname)
        {
            var font = Fonts.Instance.GetFont("Arial Unicode MS", 14.0f);

            var dateTime = DateTime.UtcNow.ToString("f");

            DrawImageText(img, world, font, new PointF(15, 5), HorizontalAlignment.Left, whiteColor);
            DrawImageText(img, mapname, font, new PointF(200, 5), HorizontalAlignment.Left, whiteColor);
            DrawImageText(img, dateTime, font, new PointF(750, 5), HorizontalAlignment.Left, whiteColor);
        }

        public static void DrawImageTopInformation<T>(Image img, string name, T obj, Func<T, string> topTextFunc, Func<T, string> bottomTextFunc, UInt64 index, Rgba32 color)
        {
            var font = Fonts.Instance.GetFont("Arial Unicode MS", 10.0f);

            var colorBorderRect = new Rectangle(
                1014,
                (int)(54 + (index * 60)),
                62,
                32
            );

            var colorRect = new Rectangle(
                1015,
                (int)(55 + (index * 60)),
                60,
                30
            );

            img.Mutate(x => x.Fill(charcoalColor, colorBorderRect));
            img.Mutate(x => x.Fill(color, colorRect));

            DrawImageText(img, name, font, new PointF(1090, 47 + (index * 60)), HorizontalAlignment.Left, blackColor);
            DrawImageText(img, topTextFunc(obj), font, new PointF(1090, 47 + 15 + (index * 60)), HorizontalAlignment.Left, blackColor);
            DrawImageText(img, bottomTextFunc(obj), font, new PointF(1090, 47 + 30 + (index * 60)), HorizontalAlignment.Left, blackColor);
        }

        public static void DrawKontinentDetails(Image img, float worldLength, UInt64 kLength, float partialK)
        {
            var font = Fonts.Instance.GetFont("Arial Unicode MS", 10.0f);

            for (UInt64 index = 0; (float)index < (float)kLength * worldLength; index += kLength)
            {
                img.Mutate(x =>
                    x.DrawLines(
                        new Pen(alphaBlackColor, 1),
                        new PointF[2]{
                            new PointF(x: ((int)((float)kLength * partialK) + (int)index), y: 30),
                            new PointF(x: ((int)((float)kLength * partialK) + (int)index), y: 1030)
                        }
                    )
                );
                img.Mutate(x =>
                    x.DrawLines(
                        new Pen(alphaBlackColor, 1),
                        new PointF[2] {
                            new PointF(x: 0, y: ((int)((float)kLength * partialK) + (int)index + 30)),
                            new PointF(x: 1000, y: ((int)((float)kLength * partialK) + (int)index + 30))
                        }
                    )
                );
            }

            var startKontinentNumber = (byte)Math.Max((Math.Ceiling((10.0f - worldLength) / 2.0f) - 1.0f), 0);
            var endKontinentNumber = (byte)(10 - Math.Ceiling((10.0f - worldLength) / 2.0f));
            var HalfWholeOffscreenKontinents = (byte)Math.Floor((10.0f - worldLength) / 2.0f);

            for (byte yKontinentNumber = startKontinentNumber; yKontinentNumber < endKontinentNumber; yKontinentNumber++)
            {
                for (byte xKontinentNumber = startKontinentNumber; xKontinentNumber < endKontinentNumber; xKontinentNumber++)
                {
                    string kNumber = yKontinentNumber.ToString() + xKontinentNumber.ToString();
                    UInt64 xPos = 87 + ((UInt64)(xKontinentNumber - HalfWholeOffscreenKontinents) * kLength);
                    UInt64 yPos = 115 + ((UInt64)(yKontinentNumber - HalfWholeOffscreenKontinents) * kLength);
                    DrawImageText(img, kNumber, font, new PointF(xPos, yPos), HorizontalAlignment.Left, alphaBlackColor);
                }
            }
        }
    }
}