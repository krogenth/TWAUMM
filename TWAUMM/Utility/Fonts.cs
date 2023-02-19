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

        public static Fonts GetInstance() { return _singleton; }

        public Font GetFont(string fontName, UInt32 size = 12, FontStyle style = FontStyle.Regular)
        {
            return _fonts.Get(fontName).CreateFont(size, style);
        }
    }
}
