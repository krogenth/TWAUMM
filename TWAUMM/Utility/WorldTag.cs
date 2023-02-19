namespace TWAUMM.Utility
{
    public class WorldTag
    {
        public static string GetWorldTag(string world)
        {
            return world.Substring(0, world.IndexOf("."));
        }
    }
}
