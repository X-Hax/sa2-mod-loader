using System;
using System.IO;
using System.Threading;
using System.Windows.Forms;

namespace UpgradeTool
{
	static class Program
	{
		private const string pipeName = "sa2-mod-manager";
		private static readonly Mutex mutex = new Mutex(true, pipeName);
		public const string exeName = "sonic2app.exe";
		public const string oldLoaderExeName = "SA2ModLoader";

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			try { mutex.WaitOne(); }
			catch (AbandonedMutexException) { }

			if (args.Length > 1 && args[0] == "doupdate")
				File.Delete(args[1] + ".7z");
			Application.EnableVisualStyles();
			Application.SetCompatibleTextRenderingDefault(false);
			Application.Run(new MainForm());
		}

		public static string FindGameDirectory(string filePath, string exeName)
		{
			string directory = Path.GetDirectoryName(filePath);
			while (directory != null)
			{
				if (Directory.GetFiles(directory, exeName).Length > 0)
				{
					return directory;
				}
				directory = Directory.GetParent(directory)?.FullName;
			}
			return null;
		}
	}
}
