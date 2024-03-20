using Microsoft.Win32;
using ModManagerCommon;
using ModManagerCommon.Forms;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.Windows.Forms;


namespace UpgradeTool
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			InitializeComponent();
		}

		public static readonly List<string> oneclickInstallList = new List<string>
		{
			"sadxmm",
			"sa2mm",
			"sonicrmm"
		};

		string updatePath = "mods/.updates";
		public string datadllorigpath = @"resource\gd_PC\DLL\Win32\Data_DLL_orig.dll";
		public string datadllpath = @"resource\gd_PC\DLL\Win32\Data_DLL.dll";
		public const string managerName = "SA2 Mod Manager";

		private void UninstallOldModLoader(string gameDirectory)
		{
			if (string.IsNullOrEmpty(gameDirectory) == false)
			{
				datadllpath = Path.GetFullPath(Path.Combine(gameDirectory, datadllpath));
				datadllorigpath = Path.GetFullPath(Path.Combine(gameDirectory, datadllorigpath));
			}

			if (File.Exists(datadllorigpath)) //remove the mod loader since we will use a new one.
			{
				File.Delete(datadllpath);
				File.Move(datadllorigpath, datadllpath);
			}
		}

		public static async Task<string> GetLatestReleaseNewManager(HttpClient httpClient)
		{
			try
			{

				httpClient.DefaultRequestHeaders.Add("User-Agent", Program.oldLoaderExeName);

				HttpResponseMessage response = await httpClient.GetAsync("https://api.github.com/repos/X-Hax/SA-Mod-manager/releases/latest");

				if (response.IsSuccessStatusCode)
				{
					string responseBody = await response.Content.ReadAsStringAsync();
					var release = JsonConvert.DeserializeObject<GitHubRelease>(responseBody);
					if (release != null && release.Assets != null)
					{
						var targetAsset = release.Assets.FirstOrDefault(asset => asset.Name.Contains(Environment.Is64BitOperatingSystem ? "x64" : "x86"));

						if (targetAsset != null)
						{
							return targetAsset.DownloadUrl;
						}
					}
				}
			}
			catch (Exception ex)
			{
				Console.WriteLine("Error fetching latest release: " + ex.Message);
			}

			return null;
		}

		private static string CleanPath(string path)
		{
			int exeIndex = path.IndexOf(".exe", StringComparison.OrdinalIgnoreCase);
			if (exeIndex != -1)
			{
				return path.Substring(0, exeIndex + 4);
			}
			else
			{
				return path;
			}
		}

		static string GetNewManagerPath()
		{
			try
			{
				foreach (var game in oneclickInstallList)
				{
					using (var hkcu = Registry.CurrentUser)
					using (var key = hkcu.OpenSubKey(@"Software\Classes\" + game + @"\shell\open\command"))
					{
						if (key != null)
						{
							string value = (string)key.GetValue(null);
							if (!string.IsNullOrEmpty(value))
							{
								// Trim leading and trailing double quotes
								value = value.Trim('"');
								return value;
							}
						}
					}
				}
			}
			catch { }

			return null;
		}

		private async void button1_Click(object sender, EventArgs e)
		{
			try
			{
				var NewManagerPath = CleanPath(GetNewManagerPath());

				if (File.Exists(NewManagerPath))
				{
					var msg = MessageBox.Show("It looks like you already have the new SA Mod Manager installed." +
	"\n\nDo you want to install the last update of it and continue the migration? (Recommended).", "SA Mod Manager found", MessageBoxButtons.YesNo);

					if (msg == DialogResult.Yes)
					{
						NewManagerPath = Path.GetFullPath(NewManagerPath); //cleanup
						string managerFolder = Path.GetDirectoryName(NewManagerPath);
						updatePath = managerFolder;

						try
						{
							File.Delete(NewManagerPath);
						}
						catch { }
					}
				}

				var wc = new HttpClient();
				var release = await GetLatestReleaseNewManager(wc) ?? (Environment.Is64BitOperatingSystem ? "https://github.com/X-Hax/SA-Mod-Manager/releases/latest/download/release_x64.zip" : "https://github.com/X-Hax/SA-Mod-Manager/releases/latest/download/release_x86.zip");
				DialogResult result = DialogResult.OK;
				do
				{
					try
					{
						if (!Directory.Exists(updatePath))
						{
							Directory.CreateDirectory(updatePath);
						}
					}
					catch (Exception ex)
					{
						result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
													   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
						if (result == DialogResult.Cancel)
							return;
					}
				} while (result == DialogResult.Retry);


				using (var dlg2 = new WPFDownloadDialog(release, updatePath))
					if (dlg2.ShowDialog(this) == DialogResult.OK)
					{
						string gameDirectory = Program.FindGameDirectory(AppDomain.CurrentDomain.BaseDirectory, Program.exeName);
						UninstallOldModLoader(gameDirectory);
						Close();
					}

			}
			catch
			{
				MessageBox.Show(this, "Unable to retrieve update information.", managerName);
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			DialogResult result = DialogResult.OK;
			do
			{
				try
				{
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);

					if (result == DialogResult.Cancel)
						return;
				}
			} while (result == DialogResult.Retry);

			using (var dlg2 = new LoaderDownloadDialog("http://mm.reimuhakurei.net/sa2mods/SA2ModLoaderLegacy.7z", updatePath))
				if (dlg2.ShowDialog(this) == DialogResult.OK)
				{
					Close();
				}

		}
	}
}
