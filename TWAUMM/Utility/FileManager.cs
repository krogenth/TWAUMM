namespace TWAUMM.Utility
{
    public class FileManager
    {
        public static void CopyExistingFilesToBackup(string worldPath)
        {
            if (!WorldExistsOnDisk(worldPath))
            {
                return;
            }

            var today = DateTime.Today.AddDays(-1).ToString("yyyy-MM-dd");
            var todayDirectory = worldPath + "/" + today;
            Directory.CreateDirectory(todayDirectory);

            foreach (var file in Directory.GetFiles(worldPath, "*.*"))
            {
                File.Copy(file, file.Replace(worldPath, todayDirectory), true);
            }
        }

        public static bool WorldExistsOnDisk(string worldPath)
        {
            return Directory.Exists(worldPath);
        }

        public static void CreateDirectory(string path)
        {
            Directory.CreateDirectory(path);
        }
    }
}