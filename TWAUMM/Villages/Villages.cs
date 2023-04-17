using System.IO.Compression;
using TWAUMM.Players;
using TWAUMM.Utility;

namespace TWAUMM.Villages
{
    using Id = UInt64;

    public class Village
    {
        public (UInt64, UInt64) coords { get; set; }
        public UInt64 points { get; set; }
        public Player? player { get; set; }
    }

    public class Villages
    {
        private SortedDictionary<Id, Village> _villages = new SortedDictionary<Id, Village>();
        private static readonly Villages _singleton = new Villages();
        private float _zoom = 1.0f;
        private UInt64[] kontinentTotalPoints { get; set; } = new UInt64[100];

        private Villages() { }

        public static Villages Instance { get { return _singleton; } }

        public SortedDictionary<Id, Village> GetVillages() { return _villages; }

        public List<Village> GetBarbarianVillages()
        {
            return _villages.Where(village => village.Value.player == null)
                .Select(kvp => kvp.Value)
                .ToList();
        }

        public float GetZoom() { return _zoom; }

        public UInt64 GetKontinentTotalPoints(UInt64 kontinent) { return kontinentTotalPoints[kontinent]; }

        public void ClearVillageData()
        {
            _villages.Clear();
        }

        public void ReadVillageData(string baseUrl)
        {
            kontinentTotalPoints = new UInt64[100];
            ClearVillageData();
            ReadVillageBaseData(baseUrl);
        }

        /// <summary>
        /// Reads in the base village data
        /// </summary>
        /// <param name="baseUrl"></param>
        public void ReadVillageBaseData(string baseUrl)
        {
            var task = Downloader.DownloadFile(baseUrl + "/map/village.txt.gz", "village.txt.gz");
            task.Wait();

            var players = Players.Players.Instance.GetPlayers();
            UInt64 lowestXCoordVillage = 500;

            using (var stream = File.OpenRead("village.txt.gz"))
            using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress))
            using (var reader = new StreamReader(gzipStream))
            {
                // $village_id, $name, $x, $y, $player_id, $points, $special
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');
                    var villageId = Id.Parse(lineValues[0]);
                    _villages.Add(villageId, new Village
                    {
                        coords = (UInt64.Parse(lineValues[2]), UInt64.Parse(lineValues[3])),
                        points = UInt64.Parse(lineValues[5]),
                    });

                    // link player and village together
                    var playerId = Id.Parse(lineValues[4]);
                    if (playerId > 0)
                    {
                        try
                        {
                            _villages[villageId].player = players[playerId];
                            players[playerId].AddVillage(_villages[villageId]);
                            // we ignore all barbarian villages for this
                            kontinentTotalPoints[Kontinent.KontinentFromVillage(_villages[villageId])] += _villages[villageId].points;
                        } catch(Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }

                    if (_villages[villageId].coords.Item1 < lowestXCoordVillage)
                    {
                        lowestXCoordVillage = _villages[villageId].coords.Item1;
                    }
                }
            }

            if (lowestXCoordVillage > 399)
            {
                _zoom = 4.0f;
            }
            else if (lowestXCoordVillage > 299)
            {
                _zoom = 2.0f;
            }
            else
            {
                _zoom = 1.0f;
            }
        }
    }
}
