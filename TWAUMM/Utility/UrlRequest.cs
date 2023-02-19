using System;

namespace TWAUMM.Utility
{
    public class UrlRequest
    {
        static readonly HttpClient client = new HttpClient();
        public static async Task<string> GetWebResponse(string url)
        {
            try
            {
                string responseBody = await client.GetStringAsync(url);
                return responseBody;
            } catch(Exception ex)
            {
                Console.WriteLine(ex.Message);
                return "";
            }
        }
    }
}