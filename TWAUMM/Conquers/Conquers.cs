using System.IO.Compression;
using TWAUMM.Utility;

namespace TWAUMM.Conquers
{
    public class Conquers
    {
        public static void ReadConquerData(string baseUrl, uint duration)
        {
            var task = Downloader.DownloadFile(baseUrl + "/map/conquer.txt.gz", "conquer.txt.gz");
            task.Wait();

            var tribes = Tribes.Tribes.Instance.GetTribes();
            var players = Players.Players.Instance.GetPlayers();
            var villages = Villages.Villages.Instance.GetVillages();
            var now = DateTime.UtcNow;

            using (var stream = File.OpenRead("conquer.txt.gz"))
            using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress))
            using (var reader = new StreamReader(gzipStream))
            {
                // $village_id, $unix_timestamp, $new_owner, $old_owner
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');
                    var villageId = UInt64.Parse(lineValues[0]);
                    var datetime = DateTimeOffset.FromUnixTimeSeconds(Int64.Parse(lineValues[1])).UtcDateTime;
                    var conquererId = UInt64.Parse(lineValues[2]);
                    var loserId = UInt64.Parse(lineValues[3]);

                    // check if time of conquer is within current timeframe
                    if (now - datetime > TimeSpan.FromDays(duration))
                    {
                        continue;
                    }

                    // we need to verify the references actually exist, it is possibly they don't
                    if (!villages.ContainsKey(villageId))
                    {
                        continue;
                    }

                    var village = villages[villageId];
                    if (players.ContainsKey(conquererId))
                    {
                        var conquerer = players[conquererId];
                        conquerer.conquers.Add(village);
                        conquerer.conquerPoints += village.points;

                        // check if the conquering player is associated with a tribe
                        if (conquerer.tribe != null)
                        {
                            conquerer.tribe.conquers.Add(village);
                            conquerer.tribe.conquerPoints += village.points;
                        }
                    }

                    if (players.ContainsKey(loserId))
                    {
                        var loser = players[loserId];
                        loser.losses.Add(village);
                        loser.lossPoints += village.points;

                        // check if the losing player is associated with a tribe
                        if (loser.tribe != null)
                        {
                            loser.tribe.losses.Add(village);
                            loser.tribe.lossPoints += village.points;
                        }
                    }
                }
            }
        }
    }
}
