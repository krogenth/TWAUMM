using System.Net;
using System.Linq;
using TWAUMM.Players;
using TWAUMM.Utility;
using TWAUMM.Villages;

namespace TWAUMM.Tribes
{
    using Id = UInt64;

    public class Tribe
    {
        public string name { get; set; } = "";
        public string tag { get; set; } = "";
        public UInt64 memberCount { get; set; }
        public UInt64 villageCount { get; set; }
        public UInt64 topPoints { get; set; }
        public UInt64 totalPoints { get; set; }
        public UInt64 rank { get; set; }
        public UInt64 od { get; set; }
        public UInt64 odRank { get; set; }
        public UInt64 oda { get; set; }
        public UInt64 odaRank { get; set; }
        public UInt64 odd { get; set; }
        public UInt64 oddRank { get; set; }
        public List<Player> players { get; set; } = new List<Player>();
        public UInt64 conquerPoints { get; set; }
        public List<Village> conquers { get; set; } = new List<Village>();
        public UInt64 lossPoints { get; set; }
        public List<Village> losses { get; set; } = new List<Village>();
    }

    public class Tribes
    {
        private SortedDictionary<Id, Tribe> _tribes = new SortedDictionary<Id, Tribe>();
        private static readonly Tribes _singleton = new Tribes();

        private Tribes() { }

        public static Tribes GetInstance() { return _singleton; }

        public SortedDictionary<Id, Tribe> GetTribes() { return _tribes; }

        public Dictionary<Id, Tribe> GetTopTribes()
        {
            return _tribes.Where(tribe => tribe.Value.rank <= 15 && tribe.Value.rank > 0)
                .ToDictionary(kvp => kvp.Value.rank, kvp => kvp.Value);
        }

        public Dictionary<Id, Tribe> GetTopODTribes()
        {
            return _tribes.Where(tribe => tribe.Value.odRank <= 15 && tribe.Value.odRank > 0)
                .ToDictionary(kvp => kvp.Value.odRank, kvp => kvp.Value);
        }

        public Dictionary<Id, Tribe> GetTopODATribes()
        {
            return _tribes.Where(tribe => tribe.Value.odaRank <= 15 && tribe.Value.odaRank > 0)
                .ToDictionary(kvp => kvp.Value.odaRank, kvp => kvp.Value);
        }

        public Dictionary<Id, Tribe> GetTopODDTribes()
        {
            return _tribes.Where(tribe => tribe.Value.oddRank <= 15 && tribe.Value.oddRank > 0)
                .ToDictionary(kvp => kvp.Value.oddRank, kvp => kvp.Value);
        }

        public Dictionary<Id, Tribe> GetTopConquerTribes()
        {
            return (from entry in _tribes where entry.Value.conquerPoints > 0 orderby entry.Value.conquerPoints descending select entry)
                .Take(15)
                .Select((element, index) => new { index, element.Value })
                .ToDictionary(kvp => (UInt64)kvp.index + 1, kvp => kvp.Value);
        }

        public Dictionary<Id, Tribe> GetTopLossTribes()
        {
            return (from entry in _tribes where entry.Value.lossPoints > 0 orderby entry.Value.lossPoints descending select entry)
                .Take(15)
                .Select((element, index) => new { index, element.Value })
                .ToDictionary(kvp => (UInt64)kvp.index + 1, kvp => kvp.Value);
        }

        /// <summary>
        /// Public method to allow retrieving all tribe data for a given base world url
        /// </summary>
        /// <param name="baseUrl"></param>
        public void ReadTribeData(string baseUrl)
        {
            _tribes.Clear();
            ReadTribeBaseData(baseUrl);
            ReadTribeODData(baseUrl);
            ReadTribeODAData(baseUrl);
            ReadTribeODDData(baseUrl);
        }

        /// <summary>
        /// Reads in the core tribe data
        /// </summary>
        /// <param name="url"></param>
        private void ReadTribeBaseData(string baseUrl)
        {
            using Task<string> response = UrlRequest.GetWebResponse(baseUrl + "/map/ally.txt");
            response.Wait();
            if (response.Result.Length <= 0)
            {
                return;
            }

            using (var reader = new StreamReader(StringToStream.GenerateStreamFromString(response.Result)))
            {
                // $tribe_id, $name, $tag, $members, $villages, $points, $all_points, $rank
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');
                    // we can use the tribe's id as the key
                    _tribes.Add(Id.Parse(lineValues[0]), new Tribe
                    {
                        name         = WebUtility.UrlDecode(lineValues[1]),
                        tag          = WebUtility.UrlDecode(lineValues[2]),
                        memberCount  = UInt64.Parse(lineValues[3]),
                        villageCount = UInt64.Parse(lineValues[4]),
                        topPoints    = UInt64.Parse(lineValues[5]),
                        totalPoints  = UInt64.Parse(lineValues[6]),
                        rank         = UInt64.Parse(lineValues[7]),
                    });
                }
            }
        }

        /// <summary>
        /// Reads in the OD tribe data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadTribeODData(string baseUrl)
        {
            using Task<string> response = UrlRequest.GetWebResponse(baseUrl + "/map/kill_all_tribe.txt");
            response.Wait();
            if (response.Result.Length <= 0)
            {
                return;
            }

            using (var reader = new StreamReader(StringToStream.GenerateStreamFromString(response.Result)))
            {
                // $rank, $id, $score
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');

                    // verify id exists in map of tribes before storing data
                    var tribeId = Id.Parse(lineValues[1]);
                    if (_tribes.ContainsKey(tribeId))
                    {
                        _tribes[tribeId].odRank = UInt64.Parse(lineValues[0]);
                        _tribes[tribeId].od     = UInt64.Parse(lineValues[2]);
                    }
                }
            }
        }

        /// <summary>
        /// Reads in the ODA tribe data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadTribeODAData(string baseUrl)
        {
            using Task<string> response = UrlRequest.GetWebResponse(baseUrl + "/map/kill_att_tribe.txt");
            response.Wait();
            if (response.Result.Length <= 0)
            {
                return;
            }

            using (var reader = new StreamReader(StringToStream.GenerateStreamFromString(response.Result)))
            {
                // $rank, $id, $score
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');

                    // verify id exists in map of tribes before storing data
                    var tribeId = Id.Parse(lineValues[1]);
                    if (_tribes.ContainsKey(tribeId))
                    {
                        _tribes[tribeId].odaRank = UInt64.Parse(lineValues[0]);
                        _tribes[tribeId].oda     = UInt64.Parse(lineValues[2]);
                    }
                }
            }
        }

        /// <summary>
        /// Reads in the ODD tribe data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadTribeODDData(string baseUrl)
        {
            using Task<string> response = UrlRequest.GetWebResponse(baseUrl + "/map/kill_def_tribe.txt");
            response.Wait();
            if (response.Result.Length <= 0)
            {
                return;
            }

            using (var reader = new StreamReader(StringToStream.GenerateStreamFromString(response.Result)))
            {
                // $rank, $id, $score
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');

                    // verify id exists in map of tribes before storing data
                    var tribeId = Id.Parse(lineValues[1]);
                    if (_tribes.ContainsKey(tribeId))
                    {
                        _tribes[tribeId].oddRank = UInt64.Parse(lineValues[0]);
                        _tribes[tribeId].odd     = UInt64.Parse(lineValues[2]);
                    }
                }
            }
        }
    }
}