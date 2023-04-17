namespace TWAUMM.Utility
{
    public class Downloader
    {
        private static HttpClient client = new HttpClient();

        public static async Task DownloadFile(string uri, string filepath)
        {
            var response = await client.GetStreamAsync(uri);
            using var fstream = new FileStream(filepath, FileMode.Create);
            response.CopyTo(fstream);
        }
    }
}