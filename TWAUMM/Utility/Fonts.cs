using SixLabors.Fonts;

namespace TWAUMM.Utility
{
    public class Fonts
    {
        private FontCollection _fonts = new FontCollection();
        private static readonly Fonts _singleton = new Fonts();

        private Fonts()
        {
            _fonts.Add("arialuni.ttf");
        }

        public static Fonts Instance { get { return _singleton; } }

        public Font GetFont(string fontName, float size = 12.0f, FontStyle style = FontStyle.Bold)
        {
            return _fonts.Get(fontName).CreateFont(size, style);
        }
    }
}