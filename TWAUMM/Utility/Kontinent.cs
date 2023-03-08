using TWAUMM.Villages;

namespace TWAUMM.Utility
{
    public class Kontinent
    {
        public static UInt64 KontinentFromVillage(Village village)
        {
            UInt64 x = village.coords.Item1 / 100u;
            UInt64 y = village.coords.Item2 / 100u;
            return (y * 10) + x;
        }

        public static UInt64 KontinentCountToDraw(float worldLength)
        {
            return (UInt64)Math.Floor(worldLength) * (UInt64)Math.Floor(worldLength);
        }
    }
}