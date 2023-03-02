namespace TWAUMM.Draw
{
    public class Draws
    {
        public static void DrawMaps(string world, uint duration)
        {
            DrawPlayers.DrawTopPlayers(world);
            DrawPlayers.DrawTopODPlayers(world);
            DrawPlayers.DrawTopODAPlayers(world);
            DrawPlayers.DrawTopODDPlayers(world);
            DrawPlayers.DrawTopConqPlayers(world, duration);
            DrawPlayers.DrawTopLossPlayers(world, duration);

            DrawTribes.DrawTopTribes(world);
            DrawTribes.DrawTopODTribes(world);
            DrawTribes.DrawTopODATribes(world);
            DrawTribes.DrawTopODDTribes(world);
            DrawTribes.DrawTopConquerTribes(world, duration);
            DrawTribes.DrawTopLossTribes(world, duration);
        }
    }
}