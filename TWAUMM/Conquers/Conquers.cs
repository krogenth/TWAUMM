using TWAUMM.Players;
using TWAUMM.Tribes;
using TWAUMM.Utility;
using TWAUMM.Villages;

namespace TWAUMM.Conquers
{
    public class Conquers
    {
        public static void ReadConquerData(string baseUrl)
        {
            var tribes = Tribes.Tribes.GetInstance().GetTribes();
            var players = Players.Players.GetInstance().GetPlayers();
            var villages = Villages.Villages.GetInstance().GetVillages();

            var now = DateTime.UtcNow;

            using Task<string> response = UrlRequest.GetWebResponse(baseUrl + "/map/conquer.txt");
            response.Wait();
            if (response.Result.Length <= 0)
            {
                return;
            }

            using (var reader = new StreamReader(StringToStream.GenerateStreamFromString(response.Result)))
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
                    if (now - datetime > TimeSpan.FromDays(1d))
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
