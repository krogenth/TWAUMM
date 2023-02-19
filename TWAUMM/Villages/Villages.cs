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
        private UInt64 _zoom = 500;

        private Villages() { }

        public static Villages GetInstance() { return _singleton; }

        public SortedDictionary<Id, Village> GetVillages() { return _villages; }

        public UInt64 GetZoom() { return _zoom; }

        public void ReadVillageData(string baseUrl)
        {
            _villages.Clear();
            _zoom = 500;
            ReadVillageBaseData(baseUrl);
        }

        /// <summary>
        /// Reads in the base village data
        /// </summary>
        /// <param name="baseUrl"></param>
        public void ReadVillageBaseData(string baseUrl)
        {
            var players = Players.Players.GetInstance().GetPlayers();

            using Task<string> response = UrlRequest.GetWebResponse(baseUrl + "/map/village.txt");
            response.Wait();
            if (response.Result.Length <= 0)
            {
                return;
            }

            using (var reader = new StreamReader(StringToStream.GenerateStreamFromString(response.Result)))
            {
                // $village_id, $name, $x, $y, $player_id, $points, $rank
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
                            players[playerId].villages.Add(_villages[villageId]);
                        } catch(Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }

                    if (_villages[villageId].coords.Item1 < _zoom)
                    {
                        _zoom = _villages[villageId].coords.Item1;
                    }
                }
            }

            if (_zoom > 399)
            {
                _zoom = 4;
            }
            else if (_zoom > 260)
            {
                _zoom = 2;
            }
            else
            {
                _zoom = 1;
            }
        }
    }
}
