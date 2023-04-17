using System.Text.Json;
using System.Text.Json.Serialization;

namespace TWAUMM.Utility
{
    public class WorldInfo
    {
        [JsonPropertyName("duration")]
        public uint duration { get; set; }
        [JsonPropertyName("world")]
        public string world { get; set; } = "";
    }

    public class JsonInfo
    {
        [JsonPropertyName("outputDir")]
        public string outputDir { get; set; } = "";
        [JsonPropertyName("colors")]
        public List<List<float>> colors { get; set; } = new List<List<float>>();
        [JsonPropertyName("worlds")]
        public List<WorldInfo> worlds { get; set; } = new List<WorldInfo>();
    }

    /// <summary>
    /// Handles the reading of json data to be used for determining worlds to generate images for
    /// </summary>
    public class Config
    {
        private static JsonInfo? info;
        private static readonly Config _singleton = new Config();
        private static string configFilename = "./config.json";

        private Config() { }

        /// <summary>
        /// Returns the singleton instance of the class
        /// </summary>
        /// <returns></returns>
        public static Config Instance { get { return _singleton; } }

        /// <summary>
        /// Re-reads the jsoninfo file to allow live-updating
        /// </summary>
        public void RefreshConfig()
        {
            string text = File.ReadAllText(configFilename);
            info = JsonSerializer.Deserialize<JsonInfo>(text);
        }

        /// <summary>
        /// Returns the read in jsoninfo if it exists
        /// </summary>
        /// <returns></returns>
        public JsonInfo? GetConfigInfo() { return info; }
    }
}