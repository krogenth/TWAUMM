﻿using System.IO.Compression;
using System.Net;
using TWAUMM.Tribes;
using TWAUMM.Utility;
using TWAUMM.Villages;

namespace TWAUMM.Players
{
    using Id = UInt64;

    public class Player
    {
        public string name { get; set; } = "";
        public UInt64 points { get; set; }
        public UInt64 rank { get; set; }
        public UInt64 od { get; set; }
        public UInt64 odRank { get; set; }
        public UInt64 oda { get; set; }
        public UInt64 odaRank { get; set; }
        public UInt64 odd { get; set; }
        public UInt64 oddRank { get; set; }
        public Tribe? tribe { get; set; }
        public List<Village> villages { get; set; } = new List<Village>();
        public UInt64 conquerPoints { get; set; }
        public List<Village> conquers { get; set; } = new List<Village>();
        public UInt64 lossPoints { get; set; }
        public List<Village> losses { get; set; } = new List<Village>();
        public UInt64[] kontinentTotalPoints { get; set; } = new UInt64[100];

        public void AddVillage(Village village)
        {
            villages.Add(village);
            kontinentTotalPoints[Kontinent.KontinentFromVillage(village)] += village.points;
            if (tribe != null)
            {
                tribe.AddVillage(village);
            }
        }
    }

    public class Players
    {
        private SortedDictionary<Id, Player> _players = new SortedDictionary<Id, Player>();
        private static readonly Players _singleton = new Players();

        private Players() { }

        public static Players Instance { get { return _singleton; } }

        public SortedDictionary<Id, Player> GetPlayers() { return _players; }

        public Dictionary<Id, Player> GetTopPlayers()
        {
            return _players.Where(player => player.Value.rank <= 15 && player.Value.rank > 0)
                .ToDictionary(kvp => kvp.Value.rank, kvp => kvp.Value);
        }

        public Dictionary<Id, Player> GetTopODPlayers()
        {
            return _players.Where(player => player.Value.odRank <= 15 && player.Value.odRank > 0)
                .ToDictionary(kvp => kvp.Value.odRank, kvp => kvp.Value);
        }

        public Dictionary<Id, Player> GetTopODAPlayers()
        {
            return _players.Where(player => player.Value.odaRank <= 15 && player.Value.odaRank > 0)
                .ToDictionary(kvp => kvp.Value.odaRank, kvp => kvp.Value);
        }

        public Dictionary<Id, Player> GetTopODDPlayers()
        {
            return _players.Where(player => player.Value.oddRank <= 15 && player.Value.oddRank > 0)
                .ToDictionary(kvp => kvp.Value.oddRank, kvp => kvp.Value);
        }

        public Dictionary<Id, Player> GetTopConqerPlayers()
        {
            return (from entry in _players where entry.Value.conquerPoints > 0 orderby entry.Value.conquerPoints descending select entry)
                .Take(15)
                .Select((element, index) => new { index, element.Value })
                .ToDictionary(kvp => (UInt64)kvp.index + 1, kvp => kvp.Value);
        }

        public Dictionary<Id, Player> GetTopLossPlayers()
        {
            return (from entry in _players where entry.Value.lossPoints > 0 orderby entry.Value.lossPoints descending select entry)
                .Take(15)
                .Select((element, index) => new { index, element.Value })
                .ToDictionary(kvp => (UInt64)kvp.index + 1, kvp => kvp.Value);
        }

        public Dictionary<Id, Player> GetTopKPlayers(UInt64 kontinent)
        {
            return (from entry in _players where entry.Value.kontinentTotalPoints[kontinent] > 0 orderby entry.Value.kontinentTotalPoints[kontinent] descending select entry)
                .Take(2)
                .Select((element, index) => new {index, element.Value, element.Key})
                .ToDictionary(kvp => (UInt64)kvp.index + 1, kvp => kvp.Value);
        }

        public void ClearPlayerData()
        {
            _players.Clear();
        }

        public void ReadPlayerData(string baseUrl)
        {
            ClearPlayerData();
            ReadPlayerBaseData(baseUrl);
            ReadPlayerODData(baseUrl);
            ReadPlayerODAData(baseUrl);
            ReadPlayerODDData(baseUrl);
        }

        /// <summary>
        /// Reads in the core player data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadPlayerBaseData(string baseUrl)
        {
            var task = Downloader.DownloadFile(baseUrl + "/map/tribe.txt.gz", "tribe.txt.gz");
            task.Wait();

            var tribes = Tribes.Tribes.Instance.GetTribes();

            using (var stream = File.OpenRead("tribe.txt.gz"))
            using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress))
            using (var reader = new StreamReader(gzipStream))
            {
                // $player_id, $name, $tribe_id, $villages, $points, $rank
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');
                    // we can use the player's id as the key
                    var playerId = Id.Parse(lineValues[0]);
                    _players.Add(playerId, new Player
                    {
                        name   = WebUtility.UrlDecode(lineValues[1]),
                        points = UInt64.Parse(lineValues[4]),
                        rank   = UInt64.Parse(lineValues[5]),
                    });

                    // link player and tribe together
                    var tribeId = UInt64.Parse(lineValues[2]);
                    if (tribeId > 0)
                    {
                        try
                        {
                            _players[playerId].tribe = tribes[tribeId];
                            tribes[tribeId].players.Add(_players[playerId]);
                        } catch(Exception ex)
                        {
                            Console.WriteLine(ex.Message);
                        }
                    }
                }
            }
        }

        /// <summary>
        /// Reads in the OD player data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadPlayerODData(string baseUrl)
        {
            var task = Downloader.DownloadFile(baseUrl + "/map/kill_all.txt.gz", "kill_all.txt.gz");
            task.Wait();

            using (var stream = File.OpenRead("kill_all.txt.gz"))
            using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress))
            using (var reader = new StreamReader(gzipStream))
            {
                // $rank, $id, $score
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');

                    var playerId = Id.Parse(lineValues[1]);
                    if (_players.ContainsKey(playerId))
                    {
                        _players[playerId].odRank = UInt64.Parse(lineValues[0]);
                        _players[playerId].od     = UInt64.Parse(lineValues[2]);
                    }
                }
            }
        }

        /// <summary>
        /// Reads in the ODA player data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadPlayerODAData(string baseUrl)
        {
            var task = Downloader.DownloadFile(baseUrl + "/map/kill_att.txt.gz", "kill_att.txt.gz");
            task.Wait();

            using (var stream = File.OpenRead("kill_att.txt.gz"))
            using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress))
            using (var reader = new StreamReader(gzipStream))
            {
                // $rank, $id, $score
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');

                    var playerId = Id.Parse(lineValues[1]);
                    if (_players.ContainsKey(playerId))
                    {
                        _players[playerId].odaRank = UInt64.Parse(lineValues[0]);
                        _players[playerId].oda     = UInt64.Parse(lineValues[2]);
                    }
                }
            }
        }

        /// <summary>
        /// Reads in the ODD player data
        /// </summary>
        /// <param name="baseUrl"></param>
        private void ReadPlayerODDData(string baseUrl)
        {
            var task = Downloader.DownloadFile(baseUrl + "/map/kill_def.txt.gz", "kill_def.txt.gz");
            task.Wait();

            using (var stream = File.OpenRead("kill_def.txt.gz"))
            using (var gzipStream = new GZipStream(stream, CompressionMode.Decompress))
            using (var reader = new StreamReader(gzipStream))
            {
                // $rank, $id, $score
                for (string? line = reader.ReadLine(); line != null && line.Length > 0; line = reader.ReadLine())
                {
                    var lineValues = line.Split(',');

                    var playerId = Id.Parse(lineValues[1]);
                    if (_players.ContainsKey(playerId))
                    {
                        _players[playerId].oddRank = UInt64.Parse(lineValues[0]);
                        _players[playerId].odd     = UInt64.Parse(lineValues[2]);
                    }
                }
            }
        }
    }
}
