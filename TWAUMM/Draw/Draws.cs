namespace TWAUMM.Draw
{
    public class Draws
    {
        public static void DrawMaps(string world)
        {
            DrawPlayers.DrawTopPlayers(world);
            DrawPlayers.DrawTopODPlayers(world);
            DrawPlayers.DrawTopODAPlayers(world);
            DrawPlayers.DrawTopODDPlayers(world);
            DrawPlayers.DrawTopConqPlayers(world);
            DrawPlayers.DrawTopLossPlayers(world);

            DrawTribes.DrawTopTribes(world);
            DrawTribes.DrawTopODTribes(world);
            DrawTribes.DrawTopODATribes(world);
            DrawTribes.DrawTopODDTribes(world);
            DrawTribes.DrawTopConquerTribes(world);
            DrawTribes.DrawTopLossTribes(world);
        }
    }
}
