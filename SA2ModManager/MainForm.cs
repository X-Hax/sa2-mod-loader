using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Security.Cryptography;
using System.Windows.Forms;
using IniFile;
using System.ComponentModel;
using ModManagerCommon;
using System.Diagnostics;
using System.Reflection;
using ModManagerCommon.Forms;
using System.Drawing;
using System.Threading;
using System.Net;
using System.Threading.Tasks;
using System.Xml;
using SA2ModManager.Properties;

namespace SA2ModManager
{
	public partial class MainForm : Form
	{
		public MainForm()
		{
			this.Font = SystemFonts.MessageBoxFont;
			InitializeComponent();

			// WORKAROUND: Windows 7's system fonts don't have
			// U+2912 or U+2913. Use Cambria instead.
			// TODO: Check the actual font to see if it has the glyphs.
			Font boldFont = null;
			OperatingSystem os = Environment.OSVersion;
			if ((os.Platform == PlatformID.Win32NT || os.Platform == PlatformID.Win32Windows) &&
				(os.Version.Major < 6 || (os.Version.Major == 6 && os.Version.Minor < 2)))
			{
				// Windows 7 or earlier.
				// TODO: Make sure this font exists.
				// NOTE: U+2912 and U+2913 are missing in Bold, so use Regular.
				boldFont = new Font("Cambria", this.Font.Size * 1.25f, FontStyle.Regular);
			}
			else
			{
				// Newer than Windows 7, or not Windows.
				// Use the default font.
				boldFont = new Font(this.Font.FontFamily, this.Font.Size * 1.25f, FontStyle.Bold);
			}

			modTopButton.Font = boldFont;
			modUpButton.Font = boldFont;
			modDownButton.Font = boldFont;
			modBottomButton.Font = boldFont;
		}

		private bool checkedForUpdates;

		const string updatePath = "mods/.updates";
		const string datadllpath = @"resource\gd_PC\DLL\Win32\Data_DLL.dll";
		const string datadllorigpath = @"resource\gd_PC\DLL\Win32\Data_DLL_orig.dll";
		const string loaderinipath = @"mods\SA2ModLoader.ini";
		const string loaderdllpath = @"mods\SA2ModLoader.dll";
		SA2LoaderInfo loaderini;
		Dictionary<string, SA2ModInfo> mods;
		const string codelstpath = @"mods\Codes.lst";
		const string codexmlpath = @"mods\Codes.xml";
		const string codedatpath = @"mods\Codes.dat";
		const string patchdatpath = @"mods\Patches.dat";
		CodeList mainCodes;
		List<Code> codes;
		bool installed;
		bool suppressEvent;

		readonly ModUpdater modUpdater = new ModUpdater();
		BackgroundWorker updateChecker;
		private bool manualModUpdate;

		private Dictionary<int, string> TestSpawnCutsceneList;
		private Dictionary<int, string> TestSpawnLevelList;

		private static bool UpdateTimeElapsed(UpdateUnit unit, int amount, DateTime start)
		{
			if (unit == UpdateUnit.Always)
			{
				return true;
			}

			TimeSpan span = DateTime.UtcNow - start;

			switch (unit)
			{
				case UpdateUnit.Hours:
					return span.TotalHours >= amount;

				case UpdateUnit.Days:
					return span.TotalDays >= amount;

				case UpdateUnit.Weeks:
					return span.TotalDays / 7.0 >= amount;

				default:
					throw new ArgumentOutOfRangeException(nameof(unit), unit, null);
			}
		}

		private static void SetDoubleBuffered(Control control, bool enable)
		{
			PropertyInfo doubleBufferPropertyInfo = control.GetType().GetProperty("DoubleBuffered", BindingFlags.Instance | BindingFlags.NonPublic);
			doubleBufferPropertyInfo?.SetValue(control, enable, null);
		}

		private void MainForm_Load(object sender, EventArgs e)
		{
			Random rand = new Random();
			if (rand.Next(2) == 1)
				switch (rand.Next(10))
				{
					case 0:
						Icon = Properties.Resources.opt_theme_on_sn;
						break;
					case 1:
						Icon = Properties.Resources.opt_theme_on_tl;
						break;
					case 2:
						Icon = Properties.Resources.opt_theme_on_kn;
						break;
					case 3:
						Icon = Properties.Resources.opt_theme_on_sd;
						break;
					case 4:
						Icon = Properties.Resources.opt_theme_on_eg;
						break;
					case 5:
						Icon = Properties.Resources.opt_theme_on_rg;
						break;
					case 6:
						Icon = Properties.Resources.opt_theme_on_am;
						break;
					case 7:
						Icon = Properties.Resources.opt_theme_on_ch;
						break;
					case 8:
						Icon = Properties.Resources.opt_theme_on_ma;
						break;
					case 9:
						Icon = Properties.Resources.opt_theme_on_se;
						break;
				}

			// Try to use TLS 1.2
			try { ServicePointManager.SecurityProtocol = (SecurityProtocolType)3072; } catch { }
			if (!Debugger.IsAttached)
				Environment.CurrentDirectory = Application.StartupPath;
			SetDoubleBuffered(modListView, true);
			if (File.Exists(loaderinipath))
				loaderini = IniSerializer.Deserialize<SA2LoaderInfo>(loaderinipath);
			else
				loaderini = new SA2LoaderInfo();

			Load_WindowUserSettings();

			try
			{
				if (File.Exists(codelstpath))
					mainCodes = CodeList.Load(codelstpath);
				else if (File.Exists(codexmlpath))
					mainCodes = CodeList.Load(codexmlpath);
				else
					mainCodes = new CodeList();
			}
			catch (Exception ex)
			{
				MessageBox.Show(this, $"Error loading code list: {ex.Message}", "SA2 Mod Loader", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				mainCodes = new CodeList();
			}

			for (int i = 0; i < Screen.AllScreens.Length; i++)
			{
				Screen s = Screen.AllScreens[i];
				screenNumComboBox.Items.Add($"{ i + 1 } { s.DeviceName } ({ s.Bounds.Location.X },{ s.Bounds.Y })"
					+ $" { s.Bounds.Width }x{ s.Bounds.Height } { s.BitsPerPixel } bpp { (s.Primary ? "Primary" : "") }");
			}

			InitTestSpawnLevelList();
			InitTestSpawnCutsceneList();
			LoadSettings();

			profileNameBox.BeginUpdate();
			foreach (var item in Directory.EnumerateFiles("mods", "*.ini"))
				if (!item.EndsWith("SA2ModLoader.ini", StringComparison.OrdinalIgnoreCase) && !item.EndsWith("desktop.ini", StringComparison.OrdinalIgnoreCase))
					profileNameBox.Items.Add(Path.GetFileNameWithoutExtension(item));
			profileNameBox.EndUpdate();
		}

		private void SetXMLConfig()
		{
			XmlDocument xdoc = new XmlDocument();
			string configPath = Directory.GetCurrentDirectory() + "\\Config\\UserConfig.cfg";

			if (File.Exists(configPath))
			{
				xdoc.Load(configPath);
				XmlNode Configs = xdoc.FirstChild.NextSibling;
				if (Configs.Attributes != null)
				{
					Configs.Attributes["Width"].Value = loaderini.HorizontalResolution.ToString();
					Configs.Attributes["Height"].Value = loaderini.VerticalResolution.ToString();
					Configs.Attributes["FullScreen"].Value = loaderini.FullScreen && !loaderini.BorderlessWindow ? "1" : "0";
					int screenID = screenNumComboBox.SelectedIndex;
					if (screenID > 0)
						screenID -= 1;

					if (screenID > -1)
						Configs.Attributes["Display"].Value = screenID.ToString();

					Configs.Attributes["Language"].Value = loaderini.TextLanguage.ToString();
					xdoc.Save(configPath);
				}
			}
		}

		private void Load_WindowUserSettings()
		{
			if (Settings.Default.Maximised)
			{
				Location = Settings.Default.Location;
				WindowState = FormWindowState.Maximized;
				Size = Settings.Default.Size;
			}
			else if (Settings.Default.Minimised)
			{
				Location = Settings.Default.Location;
				WindowState = FormWindowState.Minimized;
				Size = Settings.Default.Size;
			}
			else
			{
				Location = Settings.Default.Location;
				Size = Settings.Default.Size;
			}
		}

		private void Save_WindowUserSettings()
		{
			if (WindowState == FormWindowState.Maximized)
			{
				Settings.Default.Location = RestoreBounds.Location;
				Settings.Default.Size = RestoreBounds.Size;
				Settings.Default.Maximised = true;
				Settings.Default.Minimised = false;
			}
			else if (WindowState == FormWindowState.Normal)
			{
				Settings.Default.Location = Location;
				Settings.Default.Size = Size;
				Settings.Default.Maximised = false;
				Settings.Default.Minimised = false;
			}
			else
			{
				Settings.Default.Location = RestoreBounds.Location;
				Settings.Default.Size = RestoreBounds.Size;
				Settings.Default.Maximised = false;
				Settings.Default.Minimised = true;
			}
			Settings.Default.Save();
		}

		private void MainForm_FormClosing(object sender, FormClosingEventArgs e)
		{
			Save_WindowUserSettings();
		}


		private void LoadSettings()
		{
			LoadModList();

			consoleCheckBox.Checked = loaderini.DebugConsole;
			screenCheckBox.Checked = loaderini.DebugScreen;
			fileCheckBox.Checked = loaderini.DebugFile;
			crashLogCheckBox.Checked = loaderini.DebugCrashLog;
			pauseWhenInactiveCheckBox.Checked = loaderini.PauseWhenInactive;
			disableExitPromptCheckBox.Checked = loaderini.DisableExitPrompt;
			horizontalResolution.Value = Math.Max(horizontalResolution.Minimum, Math.Min(horizontalResolution.Maximum, loaderini.HorizontalResolution));
			verticalResolution.Value = Math.Max(verticalResolution.Minimum, Math.Min(verticalResolution.Maximum, loaderini.VerticalResolution));
			skipIntrocheckBox.Checked = loaderini.SkipIntro;
			disableLoadAnimCheckBox.Checked = loaderini.SyncLoad;
			checkUpdateStartup.Checked = loaderini.UpdateCheck;
			checkUpdateModsStartup.Checked = loaderini.ModUpdateCheck;
			comboUpdateFrequency.SelectedIndex = (int)loaderini.UpdateUnit;
			numericUpdateFrequency.Value = loaderini.UpdateFrequency;
			comboVoiceLanguage.SelectedIndex = (int)loaderini.VoiceLanguage;
			comboTextLanguage.SelectedIndex = loaderini.TextLanguage;
			radioFullscreen.Checked = loaderini.FullScreen;
			radioWindowMode.Checked = loaderini.FullScreen == false;
			radioBorderlessWindowMode.Checked = loaderini.BorderlessWindow;
			
			int screenNum = Math.Min(Screen.AllScreens.Length, loaderini.ScreenNum);

			screenNumComboBox.SelectedIndex = screenNum;
			customWindowSizeCheckBox.Checked = windowHeight.Enabled = loaderini.CustomWindowSize;
			windowWidth.Enabled = loaderini.CustomWindowSize;

			Rectangle rect = Screen.PrimaryScreen.Bounds;

			foreach (Screen screen in Screen.AllScreens)
				rect = Rectangle.Union(rect, screen.Bounds);

			windowWidth.Maximum = rect.Width;
			windowWidth.Value = Math.Max(windowWidth.Minimum, Math.Min(rect.Width, loaderini.WindowWidth));
			windowHeight.Maximum = rect.Height;
			windowHeight.Value = Math.Max(windowHeight.Minimum, Math.Min(rect.Height, loaderini.WindowHeight));

			checkWindowResize.Checked = loaderini.ResizableWindow;
			aspectRatioCheckBox.Checked = loaderini.MaintainAspectRatio;
			checkFrameLimiter.Checked = loaderini.FramerateLimiter;

			checkBoxTestSpawnLevel.Checked = loaderini.TestSpawnLevel != -1;
			comboBoxTestSpawnLevel.SelectedIndex = loaderini.TestSpawnLevel;
			checkBoxTestSpawnCharacter.Checked = loaderini.TestSpawnCharacter != -1;
			comboBoxTestSpawnCharacter.SelectedIndex = loaderini.TestSpawnCharacter;
			checkBoxTestSpawnPlayer2.Checked = loaderini.TestSpawnPlayer2 != -1;
			comboBoxTestSpawnPlayer2.SelectedIndex = loaderini.TestSpawnPlayer2;
			checkBoxTestSpawnPosition.Checked = loaderini.TestSpawnPositionEnabled;
			numericUpDownTestSpawnX.Value = loaderini.TestSpawnX;
			numericUpDownTestSpawnY.Value = loaderini.TestSpawnY;
			numericUpDownTestSpawnZ.Value = loaderini.TestSpawnZ;
			checkBoxTestSpawnEvent.Checked = loaderini.TestSpawnEvent != -1;
			comboBoxTestSpawnEvent.SelectedIndex = loaderini.TestSpawnEvent;
			checkBoxTestSpawnSave.Checked = loaderini.TestSpawnSaveID != -1;
			numericUpDownTestSpawnSaveID.Value = Math.Max(1, loaderini.TestSpawnSaveID);
		}

		private void HandleUri(string uri)
		{
			if (WindowState == FormWindowState.Minimized)
			{
				WindowState = FormWindowState.Normal;
			}

			Activate();

			Uri url;
			string name;
			string author;
			string[] split = uri.Substring("sa2mm:".Length).Split(',');
			url = new Uri(split[0]);
			Dictionary<string, string> fields = new Dictionary<string, string>(split.Length - 1);
			for (int i = 1; i < split.Length; i++)
			{
				int ind = split[i].IndexOf(':');
				fields.Add(split[i].Substring(0, ind).ToLowerInvariant(), split[i].Substring(ind + 1));
			}
			if (fields.ContainsKey("gb_itemtype") && fields.ContainsKey("gb_itemid"))
			{
				string itemType;
				long itemId;

				try
				{
					itemType = fields["gb_itemtype"];
					itemId = long.Parse(fields["gb_itemid"]);
				}
				catch (Exception ex)
				{
					MessageBox.Show(this,
									$"Malformed One-Click Install URI \"{uri}\" caused parse failure:\n{ex.Message}",
									"URI Parse Failure",
									MessageBoxButtons.OK,
									MessageBoxIcon.Error);

					return;
				}

				GameBananaItem gbi;

				try
				{
					gbi = GameBananaItem.Load(itemType, itemId);

					if (gbi is null)
					{
						throw new Exception("GameBananaItem was unexpectedly null");
					}
				}
				catch (Exception ex)
				{
					MessageBox.Show(this,
									$"GameBanana API query failed:\n{ex.Message}",
									"GameBanana API Failure",
									MessageBoxButtons.OK,
									MessageBoxIcon.Error);

					return;
				}
				name = gbi.Name;
				author = gbi.OwnerName;
			}
			else if (fields.ContainsKey("name") && fields.ContainsKey("author"))
			{
				name = Uri.UnescapeDataString(fields["name"]);
				author = Uri.UnescapeDataString(fields["author"]);
			}
			else
			{
				MessageBox.Show(this,
								$"One-Click Install URI \"{uri}\" did not contain required fields.",
								"URI Parse Failure",
								MessageBoxButtons.OK,
								MessageBoxIcon.Error);

				return;
			}

			var dummyInfo = new ModInfo
			{
				Name = name,
				Author = author
			};

			DialogResult result = MessageBox.Show(this, $"Do you want to install mod \"{dummyInfo.Name}\" by {dummyInfo.Author} from {url.DnsSafeHost}?", "Mod Download", MessageBoxButtons.YesNo, MessageBoxIcon.Question);

			if (result != DialogResult.Yes)
			{
				return;
			}

			#region create update folder
			do
			{
				try
				{
					result = DialogResult.Cancel;
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);
			#endregion

			string dummyPath = dummyInfo.Name;

			foreach (char c in Path.GetInvalidFileNameChars())
			{
				dummyPath = dummyPath.Replace(c, '_');
			}

			dummyPath = Path.Combine("mods", dummyPath);

			var updates = new List<ModDownload>
			{
				new ModDownload(dummyInfo, dummyPath, url.AbsoluteUri, null, 0)
			};

			using (var progress = new ModDownloadDialog(updates, updatePath))
			{
				progress.ShowDialog(this);
			}

			do
			{
				try
				{
					result = DialogResult.Cancel;
					Directory.Delete(updatePath, true);
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to remove temporary update directory:\n" + ex.Message
												   + "\n\nWould you like to retry? You can remove the directory manually later.",
						"Directory Deletion Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			LoadModList();
		}

		private void MainForm_Shown(object sender, EventArgs e)
		{
			if (CheckForUpdates())
				return;

			if (!File.Exists(datadllpath))
			{
				MessageBox.Show(this, "Data_DLL.dll could not be found.\n\nCannot determine state of installation.\n\nMake sure that the mod loader files are in the game folder.", Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
				installButton.Hide();
			}
			else if (File.Exists(datadllorigpath))
			{
				installed = true;
				installButton.Text = "Uninstall loader";
				using (MD5 md5 = MD5.Create())
				{
					byte[] hash1 = md5.ComputeHash(File.ReadAllBytes(loaderdllpath));
					byte[] hash2 = md5.ComputeHash(File.ReadAllBytes(datadllpath));
					if (!hash1.SequenceEqual(hash2))
						if (MessageBox.Show(this, "Installed loader DLL differs from copy in mods folder.\n\nDo you want to overwrite the installed copy?", Text, MessageBoxButtons.YesNo, MessageBoxIcon.Warning) == DialogResult.Yes)
							File.Copy(loaderdllpath, datadllpath, true);
				}
			}

			List<string> uris = Program.UriQueue.GetUris();

			foreach (string str in uris)
			{
				HandleUri(str);
			}

			Program.UriQueue.UriEnqueued += UriQueueOnUriEnqueued;

			CheckForModUpdates();

			// If we've checked for updates, save the modified
			// last update times without requiring the user to
			// click the save button.
			if (checkedForUpdates)
				IniSerializer.Serialize(loaderini, loaderinipath);
		}

		private void UriQueueOnUriEnqueued(object sender, OnUriEnqueuedArgs args)
		{
			args.Handled = true;

			if (InvokeRequired)
			{
				Invoke((Action<object, OnUriEnqueuedArgs>)UriQueueOnUriEnqueued, sender, args);
				return;
			}

			HandleUri(args.Uri);
		}

		private void LoadModList()
		{
			modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
			modDescription.Text = "Description: No mod selected.";
			modListView.Items.Clear();
			mods = new Dictionary<string, SA2ModInfo>();
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");

			if (File.Exists(Path.Combine(modDir, "mod.ini")))
			{
				MessageBox.Show(this, "There is a mod.ini in the mods folder."
							+ "\n\nEach mod must be placed in a subfolder in the mods folder. Do not extract mods directly to the mods folder." +
							"\n\nMove or delete mod.ini in the mods folder and run the Mod Manager again.", "SA2 Mod Manager Error", MessageBoxButtons.OK, MessageBoxIcon.Error);
				Close();
				return;
			}

			foreach (string filename in SA2ModInfo.GetModFiles(new DirectoryInfo(modDir)))
				mods.Add((Path.GetDirectoryName(filename) ?? string.Empty).Substring(modDir.Length + 1), IniSerializer.Deserialize<SA2ModInfo>(filename));

			modListView.BeginUpdate();

			foreach (string mod in new List<string>(loaderini.Mods))
			{
				if (mods.ContainsKey(mod))
				{
					SA2ModInfo inf = mods[mod];
					suppressEvent = true;
					modListView.Items.Add(new ListViewItem(new[] { inf.Name, inf.Author, inf.Version, inf.Category }) { Checked = true, Tag = mod });
					suppressEvent = false;
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
				else
				{
					MessageBox.Show(this, "Mod \"" + mod + "\" could not be found.\n\nThis mod will be removed from the list.", Text, MessageBoxButtons.OK, MessageBoxIcon.Warning);
					loaderini.Mods.Remove(mod);
				}
			}

			foreach (KeyValuePair<string, SA2ModInfo> inf in mods.OrderBy(x => x.Value.Name))
				if (!loaderini.Mods.Contains(inf.Key))
					modListView.Items.Add(new ListViewItem(new[] { inf.Value.Name, inf.Value.Author, inf.Value.Version, inf.Value.Category }) { Tag = inf.Key });

			modListView.EndUpdate();

			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);

			codesCheckedListBox.BeginUpdate();
			codesCheckedListBox.Items.Clear();

			foreach (Code item in codes)
				codesCheckedListBox.Items.Add(item.Name, loaderini.EnabledCodes.Contains(item.Name));

			codesCheckedListBox.EndUpdate();
		}

		private bool CheckForUpdates(bool force = false)
		{
			if (!force && !loaderini.UpdateCheck)
			{
				return false;
			}

			if (!force && !UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.UpdateTime)))
			{
				return false;
			}

			checkedForUpdates = true;
			loaderini.UpdateTime = DateTime.UtcNow.ToFileTimeUtc();

			if (!File.Exists("sa2mlver.txt"))
			{
				return false;
			}

			using (var wc = new WebClient())
			{
				try
				{
					string msg = wc.DownloadString("http://mm.reimuhakurei.net/toolchangelog.php?tool=sa2ml&rev=" + File.ReadAllText("sa2mlver.txt"));

					if (msg.Length > 0)
					{
						using (var dlg = new UpdateMessageDialog("SA2", msg.Replace("\n", "\r\n")))
						{
							if (dlg.ShowDialog(this) == DialogResult.Yes)
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
										if (result == DialogResult.Cancel) return false;
									}
								} while (result == DialogResult.Retry);

								using (var dlg2 = new LoaderDownloadDialog("http://mm.reimuhakurei.net/sa2mods/SA2ModLoader.7z", updatePath))
									if (dlg2.ShowDialog(this) == DialogResult.OK)
									{
										Close();
										return true;
									}
							}
						}
					}
				}
				catch
				{
					MessageBox.Show(this, "Unable to retrieve update information.", "SA2 Mod Manager");
				}
			}

			return false;
		}

		private void InitializeWorker()
		{
			if (updateChecker != null)
			{
				return;
			}

			updateChecker = new BackgroundWorker { WorkerSupportsCancellation = true };
			updateChecker.DoWork += UpdateChecker_DoWork;
			updateChecker.RunWorkerCompleted += UpdateChecker_RunWorkerCompleted;
			updateChecker.RunWorkerCompleted += UpdateChecker_EnableControls;
		}

		private void UpdateChecker_EnableControls(object sender, RunWorkerCompletedEventArgs runWorkerCompletedEventArgs)
		{
			buttonCheckForUpdates.Enabled = true;
			checkForUpdatesToolStripMenuItem.Enabled = true;
			verifyToolStripMenuItem.Enabled = true;
			forceUpdateToolStripMenuItem.Enabled = true;
			uninstallToolStripMenuItem.Enabled = true;
			developerToolStripMenuItem.Enabled = true;
		}

		private void CheckForModUpdates(bool force = false)
		{
			if (!force && !loaderini.ModUpdateCheck)
			{
				return;
			}

			InitializeWorker();

			if (!force && !UpdateTimeElapsed(loaderini.UpdateUnit, loaderini.UpdateFrequency, DateTime.FromFileTimeUtc(loaderini.ModUpdateTime)))
			{
				return;
			}

			checkedForUpdates = true;
			loaderini.ModUpdateTime = DateTime.UtcNow.ToFileTimeUtc();
			updateChecker.RunWorkerAsync(mods.Select(x => new KeyValuePair<string, ModInfo>(x.Key, x.Value)).ToList());
			buttonCheckForUpdates.Enabled = false;
		}

		private void UpdateChecker_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
		{
			if (modUpdater.ForceUpdate)
			{
				updateChecker?.Dispose();
				updateChecker = null;
				modUpdater.ForceUpdate = false;
				modUpdater.Clear();
			}

			if (e.Cancelled)
			{
				return;
			}

			if (!(e.Result is Tuple<List<ModDownload>, List<string>> data))
			{
				return;
			}

			List<string> errors = data.Item2;
			if (errors.Count != 0)
			{
				MessageBox.Show(this, "The following errors occurred while checking for updates:\n\n" + string.Join("\n", errors),
					"Update Errors", MessageBoxButtons.OK, MessageBoxIcon.Warning);
			}

			bool manual = manualModUpdate;
			manualModUpdate = false;

			List<ModDownload> updates = data.Item1;
			if (updates.Count == 0)
			{
				if (manual)
				{
					MessageBox.Show(this, "Mods are up to date.",
						"No Updates", MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				return;
			}

			using (var dialog = new ModUpdatesDialog(updates))
			{
				if (dialog.ShowDialog(this) != DialogResult.OK)
				{
					return;
				}

				updates = dialog.SelectedMods;
			}

			if (updates.Count == 0)
			{
				return;
			}

			DialogResult result;

			do
			{
				try
				{
					result = DialogResult.Cancel;
					if (!Directory.Exists(updatePath))
					{
						Directory.CreateDirectory(updatePath);
					}
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to create temporary update directory:\n" + ex.Message
						+ "\n\nWould you like to retry?", "Directory Creation Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			using (var progress = new ModDownloadDialog(updates, updatePath))
			{
				progress.ShowDialog(this);
			}

			do
			{
				try
				{
					result = DialogResult.Cancel;
					Directory.Delete(updatePath, true);
				}
				catch (Exception ex)
				{
					result = MessageBox.Show(this, "Failed to remove temporary update directory:\n" + ex.Message
						+ "\n\nWould you like to retry? You can remove the directory manually later.",
						"Directory Deletion Failed", MessageBoxButtons.RetryCancel);
				}
			} while (result == DialogResult.Retry);

			LoadModList();
		}

		private void UpdateChecker_DoWork(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			Invoke(new Action(() =>
			{
				buttonCheckForUpdates.Enabled = false;
				checkForUpdatesToolStripMenuItem.Enabled = false;
				verifyToolStripMenuItem.Enabled = false;
				forceUpdateToolStripMenuItem.Enabled = false;
				uninstallToolStripMenuItem.Enabled = false;
				developerToolStripMenuItem.Enabled = false;
			}));

			var updatableMods = e.Argument as List<KeyValuePair<string, ModInfo>>;
			List<ModDownload> updates = null;
			List<string> errors = null;

			var tokenSource = new CancellationTokenSource();
			CancellationToken token = tokenSource.Token;

			using (var task = new Task(() => modUpdater.GetModUpdates(updatableMods, out updates, out errors, token), token))
			{
				task.Start();

				while (!task.IsCompleted && !task.IsCanceled)
				{
					Application.DoEvents();

					if (worker.CancellationPending)
					{
						tokenSource.Cancel();
					}
				}

				task.Wait(token);
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

		// TODO: merge with ^
		private void UpdateChecker_DoWorkForced(object sender, DoWorkEventArgs e)
		{
			if (!(sender is BackgroundWorker worker))
			{
				throw new Exception("what");
			}

			if (!(e.Argument is List<Tuple<string, ModInfo, List<ModManifestDiff>>> updatableMods) || updatableMods.Count == 0)
			{
				return;
			}

			var updates = new List<ModDownload>();
			var errors = new List<string>();

			using (var client = new UpdaterWebClient())
			{
				foreach (Tuple<string, ModInfo, List<ModManifestDiff>> info in updatableMods)
				{
					if (worker.CancellationPending)
					{
						e.Cancel = true;
						break;
					}

					ModInfo mod = info.Item2;
					if (!string.IsNullOrEmpty(mod.GitHubRepo))
					{
						if (string.IsNullOrEmpty(mod.GitHubAsset))
						{
							errors.Add($"[{ mod.Name }] GitHubRepo specified, but GitHubAsset is missing.");
							continue;
						}

						ModDownload d = modUpdater.GetGitHubReleases(mod, info.Item1, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.GameBananaItemType) && mod.GameBananaItemId.HasValue)
					{
						ModDownload d = modUpdater.GetGameBananaReleases(mod, info.Item1, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
					else if (!string.IsNullOrEmpty(mod.UpdateUrl))
					{
						List<ModManifestEntry> localManifest = info.Item3
							.Where(x => x.State == ModManifestState.Unchanged)
							.Select(x => x.Current).ToList();

						ModDownload d = modUpdater.CheckModularVersion(mod, info.Item1, localManifest, client, errors);
						if (d != null)
						{
							updates.Add(d);
						}
					}
				}
			}

			e.Result = new Tuple<List<ModDownload>, List<string>>(updates, errors);
		}

		private void modListView_SelectedIndexChanged(object sender, EventArgs e)
		{
			int count = modListView.SelectedIndices.Count;
			if (count == 0)
			{
				modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = configureModButton.Enabled = false;
				modDescription.Text = "Description: No mod selected.";
			}
			else if (count == 1)
			{
				modDescription.Text = "Description: " + mods[(string)modListView.SelectedItems[0].Tag].Description;
				modTopButton.Enabled = modListView.SelectedIndices[0] != 0;
				modUpButton.Enabled = modListView.SelectedIndices[0] > 0;
				modDownButton.Enabled = modListView.SelectedIndices[0] < modListView.Items.Count - 1;
				modBottomButton.Enabled = modListView.SelectedIndices[0] != modListView.Items.Count - 1;
				configureModButton.Enabled = File.Exists(Path.Combine("mods", (string)modListView.SelectedItems[0].Tag, "configschema.xml"));
			}
			else if (count > 1)
			{
				modDescription.Text = "Description: Multiple mods selected.";
				modTopButton.Enabled = modUpButton.Enabled = modDownButton.Enabled = modBottomButton.Enabled = true;
				configureModButton.Enabled = false;
			}
		}

		private void modTopButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = 0; i < modListView.SelectedItems.Count; i++)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index > 0)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(i, item);
				}
			}

			modListView.SelectedItems[0].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modUpButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = 0; i < modListView.SelectedItems.Count; i++)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index-- > 0 && !modListView.Items[index].Selected)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(index, item);
				}
			}

			modListView.SelectedItems[0].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modDownButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = modListView.SelectedItems.Count - 1; i >= 0; i--)
			{
				int index = modListView.SelectedItems[i].Index + 1;

				if (index != modListView.Items.Count && !modListView.Items[index].Selected)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(index, item);
				}
			}

			modListView.SelectedItems[modListView.SelectedItems.Count - 1].EnsureVisible();
			modListView.EndUpdate();
		}

		private void modBottomButton_Click(object sender, EventArgs e)
		{
			if (modListView.SelectedItems.Count < 1)
				return;

			modListView.BeginUpdate();

			for (int i = modListView.SelectedItems.Count - 1; i >= 0; i--)
			{
				int index = modListView.SelectedItems[i].Index;

				if (index != modListView.Items.Count - 1)
				{
					ListViewItem item = modListView.SelectedItems[i];
					modListView.Items.Remove(item);
					modListView.Items.Insert(modListView.Items.Count, item);
				}
			}

			modListView.SelectedItems[modListView.SelectedItems.Count - 1].EnsureVisible();
			modListView.EndUpdate();
		}

		static readonly string moddropname = "Mod" + Process.GetCurrentProcess().Id;
		private void modListView_ItemDrag(object sender, ItemDragEventArgs e)
		{
			modListView.DoDragDrop(new DataObject(moddropname, modListView.SelectedItems.Cast<ListViewItem>().ToArray()), DragDropEffects.Move | DragDropEffects.Scroll);
		}

		private void modListView_DragEnter(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
				e.Effect = DragDropEffects.Move | DragDropEffects.Scroll;
			else
				e.Effect = DragDropEffects.None;
		}

		private void modListView_DragOver(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
				e.Effect = DragDropEffects.Move | DragDropEffects.Scroll;
			else
				e.Effect = DragDropEffects.None;
		}

		private void modListView_DragDrop(object sender, DragEventArgs e)
		{
			if (e.Data.GetDataPresent(moddropname))
			{
				Point clientPoint = modListView.PointToClient(new Point(e.X, e.Y));
				ListViewItem[] items = (ListViewItem[])e.Data.GetData(moddropname);
				int ind = modListView.GetItemAt(clientPoint.X, clientPoint.Y).Index;
				foreach (ListViewItem item in items)
					if (ind > item.Index)
						ind++;
				modListView.BeginUpdate();
				foreach (ListViewItem item in items)
					modListView.Items.Insert(ind++, (ListViewItem)item.Clone());
				foreach (ListViewItem item in items)
					modListView.Items.Remove(item);
				modListView.EndUpdate();
			}
		}

		private void Save()
		{
			loaderini.Mods.Clear();

			foreach (ListViewItem item in modListView.CheckedItems)
			{
				loaderini.Mods.Add((string)item.Tag);
			}

			loaderini.DebugConsole = consoleCheckBox.Checked;
			loaderini.DebugScreen = screenCheckBox.Checked;
			loaderini.DebugFile = fileCheckBox.Checked;
			loaderini.DebugCrashLog = crashLogCheckBox.Checked;
			loaderini.PauseWhenInactive = pauseWhenInactiveCheckBox.Checked;
			loaderini.DisableExitPrompt = disableExitPromptCheckBox.Checked;
			loaderini.BorderlessWindow = radioBorderlessWindowMode.Checked;
			loaderini.HorizontalResolution = (int)horizontalResolution.Value;
			loaderini.VerticalResolution = (int)verticalResolution.Value;
			loaderini.FullScreen = radioFullscreen.Checked;
			loaderini.ScreenNum = screenNumComboBox.SelectedIndex;
			loaderini.CustomWindowSize = customWindowSizeCheckBox.Checked;
			loaderini.WindowWidth = (int)windowWidth.Value;
			loaderini.WindowHeight = (int)windowHeight.Value;
			loaderini.ResizableWindow = checkWindowResize.Checked;
			loaderini.MaintainAspectRatio = aspectRatioCheckBox.Checked;
			loaderini.FramerateLimiter = checkFrameLimiter.Checked;
			loaderini.SkipIntro = skipIntrocheckBox.Checked;
			loaderini.SyncLoad = disableLoadAnimCheckBox.Checked;
			loaderini.UpdateCheck = checkUpdateStartup.Checked;
			loaderini.ModUpdateCheck = checkUpdateModsStartup.Checked;
			loaderini.UpdateUnit = (UpdateUnit)comboUpdateFrequency.SelectedIndex;
			loaderini.UpdateFrequency = (int)numericUpdateFrequency.Value;
			loaderini.VoiceLanguage = (int)comboVoiceLanguage.SelectedIndex;
			loaderini.TextLanguage = (int)comboTextLanguage.SelectedIndex;

			loaderini.TestSpawnLevel = checkBoxTestSpawnLevel.Checked ? comboBoxTestSpawnLevel.SelectedIndex : -1;
			loaderini.TestSpawnCharacter = checkBoxTestSpawnCharacter.Checked ? comboBoxTestSpawnCharacter.SelectedIndex : -1;
			loaderini.TestSpawnPlayer2 = checkBoxTestSpawnPlayer2.Checked ? comboBoxTestSpawnPlayer2.SelectedIndex : -1;
			loaderini.TestSpawnPositionEnabled = checkBoxTestSpawnPosition.Checked;
			loaderini.TestSpawnX = (int)numericUpDownTestSpawnX.Value;
			loaderini.TestSpawnY = (int)numericUpDownTestSpawnY.Value;
			loaderini.TestSpawnZ = (int)numericUpDownTestSpawnZ.Value;
			loaderini.TestSpawnEvent = checkBoxTestSpawnEvent.Checked ? comboBoxTestSpawnEvent.SelectedIndex : -1;
			loaderini.TestSpawnSaveID = checkBoxTestSpawnSave.Checked ? (int)numericUpDownTestSpawnSaveID.Value : -1;

			IniSerializer.Serialize(loaderini, loaderinipath);
			SetXMLConfig();

			List<Code> selectedCodes = new List<Code>();
			List<Code> selectedPatches = new List<Code>();

			foreach (Code item in codesCheckedListBox.CheckedIndices.OfType<int>().Select(a => this.codes[a]))
				if (item.Patch)
					selectedPatches.Add(item);
				else
					selectedCodes.Add(item);

			CodeList.WriteDatFile(patchdatpath, selectedPatches);
			CodeList.WriteDatFile(codedatpath, selectedCodes);
		}

		private Dictionary<string, string> GetModReference()
		{
			Dictionary<string, string> activeMods = new Dictionary<string, string>();

			foreach (string mod in mods.Keys)
			{
				SA2ModInfo modinfo = mods[mod];
				string id = mod;
				if (modinfo.ModID != null)
					id = modinfo.ModID;
				if (!activeMods.ContainsKey(id))
					activeMods.Add(id, mod);
				else
				{
					MessageBox.Show(this,
						"Mods with duplicate ID '" + id + "' have been detected.\n" +
						"Mod name: '" + modinfo.Name + "'.\n" +
						"Remove duplicate mods or edit the value 'ModID' in mod.ini to fix this error.\n",
						"SA2 Mod Manager Error",
						MessageBoxButtons.OK,
						MessageBoxIcon.Error);
				}
			}

			return activeMods;
		}

		private bool CheckDependencies(string lMod, Dictionary<string, string> cMods)
		{
			bool check = false;
			SA2ModInfo mod = mods[lMod];

			if (mod.Dependencies.Count > 0)
			{
				int mID = loaderini.Mods.IndexOf(lMod);
				foreach (string sDependency in mod.Dependencies)
				{
					ModDependency dependency = new ModDependency(sDependency);
					if (dependency.ID == "" && dependency.Folder == "")
						return false;

					string depName = dependency.GetDependencyName();

					bool modExists = false;
					if (cMods.ContainsKey(dependency.ID))
						modExists = true;
					else if (cMods.ContainsValue(dependency.Folder))
						modExists = true;
					else if (cMods.ContainsValue(dependency.Name))
					{
						dependency.Folder = dependency.Name;
						modExists = true;
					}

					if (modExists)
					{
						// If Dependency Mod Exists, check if mod is active.
						if (loaderini.Mods.Contains(dependency.Folder))
						{
							int cID = loaderini.Mods.IndexOf(dependency.Folder);
							if (mID < cID)
							{
								MessageBox.Show(depName + " needs to be placed above " + mod.Name, "Mod Order", MessageBoxButtons.OKCancel);
								check = true;
							}
						}
						else
						{
							MessageBox.Show(depName + " is not enabled. Please enable this mod and place it above " + mod.Name, "Mod Dependency", MessageBoxButtons.OKCancel);
							check = true;
						}
					}
					else
					{
						string msg = "Dependency " + depName + " is not installed. Please install " + depName + " and place it above " + mod.Name;
						if (dependency.Link == "")
						{
							MessageBox.Show(msg, "Missing Mod", MessageBoxButtons.OKCancel);
							check = true;
						}
						else
						{
							DialogResult dg = MessageBox.Show(msg + "\n\nWould you like to download this mod now?", "Missing Mod", MessageBoxButtons.YesNo);
							if (dg == DialogResult.Yes)
							{
								Process.Start(dependency.Link);
							}
							check = true;
						}
					}
				}
			}
			return check;
		}

		private void saveAndPlayButton_Click(object sender, EventArgs e)
		{
			if (updateChecker?.IsBusy == true)
			{
				var result = MessageBox.Show(this, "Mods are still being checked for updates. Continue anyway?",
					"Busy", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

				if (result == DialogResult.No)
				{
					return;
				}

				Enabled = false;

				updateChecker.CancelAsync();
				while (updateChecker.IsBusy)
				{
					Application.DoEvents();
				}

				Enabled = true;
			}

			Save();
			if (!installed)
				switch (MessageBox.Show(this, "Looks like you're starting the game without the mod loader installed. Without the mod loader, the mods and codes you've selected won't be used, and some settings may not work.\n\nDo you want to install the mod loader now?", "SA2 Mod Manager", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Warning, MessageBoxDefaultButton.Button1))
				{
					case DialogResult.Cancel:
						return;
					case DialogResult.Yes:
						File.Move(datadllpath, datadllorigpath);
						File.Copy(loaderdllpath, datadllpath);
						break;
				}

			Dictionary<string, string> cMods = GetModReference();
			foreach (string mod in loaderini.Mods)
			{
				if (CheckDependencies(mod, cMods))
					return;
			}

			Process process = Process.Start(loaderini.Mods.Select((item) => mods[item].EXEFile)
												.FirstOrDefault((item) => !string.IsNullOrEmpty(item)) ?? "sonic2app.exe");
			process?.WaitForInputIdle(10000);
			Close();
		}

		private void saveButton_Click(object sender, EventArgs e)
		{
			Save();
			LoadModList();
		}

		private void installButton_Click(object sender, EventArgs e)
		{
			if (installed)
			{
				File.Delete(datadllpath);
				File.Move(datadllorigpath, datadllpath);
				installButton.Text = "Install loader";
			}
			else
			{
				File.Move(datadllpath, datadllorigpath);
				File.Copy(loaderdllpath, datadllpath);
				installButton.Text = "Uninstall loader";
			}
			installed = !installed;
		}

		private void buttonRefreshModList_Click(object sender, EventArgs e)
		{
			LoadModList();
		}

		private void buttonNewMod_Click(object sender, EventArgs e)
		{
			using (var ModDialog = new NewModDialog())
			{
				if (ModDialog.ShowDialog() == DialogResult.OK)
					LoadModList();
			}
		}

		private void codesCheckedListBox_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			Code code = codes[e.Index];
			if (code.Required)
				e.NewValue = CheckState.Checked;
			if (e.NewValue == CheckState.Unchecked)
			{
				if (loaderini.EnabledCodes.Contains(code.Name))
					loaderini.EnabledCodes.Remove(code.Name);
			}
			else
			{
				if (!loaderini.EnabledCodes.Contains(code.Name))
					loaderini.EnabledCodes.Add(code.Name);
			}
		}

		private void modListView_ItemCheck(object sender, ItemCheckEventArgs e)
		{
			if (suppressEvent) return;
			codes = new List<Code>(mainCodes.Codes);
			string modDir = Path.Combine(Environment.CurrentDirectory, "mods");
			List<string> modlist = new List<string>();
			foreach (ListViewItem item in modListView.CheckedItems)
				modlist.Add((string)item.Tag);
			if (e.NewValue == CheckState.Unchecked)
				modlist.Remove((string)modListView.Items[e.Index].Tag);
			else
				modlist.Add((string)modListView.Items[e.Index].Tag);
			foreach (string mod in modlist)
				if (mods.ContainsKey(mod))
				{
					SA2ModInfo inf = mods[mod];
					if (!string.IsNullOrEmpty(inf.Codes))
						codes.AddRange(CodeList.Load(Path.Combine(Path.Combine(modDir, mod), inf.Codes)).Codes);
				}
			loaderini.EnabledCodes = new List<string>(loaderini.EnabledCodes.Where(a => codes.Any(c => c.Name == a)));
			foreach (Code item in codes.Where(a => a.Required && !loaderini.EnabledCodes.Contains(a.Name)))
				loaderini.EnabledCodes.Add(item.Name);
			codesCheckedListBox.BeginUpdate();
			codesCheckedListBox.Items.Clear();
			foreach (Code item in codes)
				codesCheckedListBox.Items.Add(item.Name, loaderini.EnabledCodes.Contains(item.Name));
			codesCheckedListBox.EndUpdate();
		}


		private void modListView_MouseClick(object sender, MouseEventArgs e)
		{
			if (e.Button != MouseButtons.Right)
			{
				return;
			}

			if (modListView.FocusedItem.Bounds.Contains(e.Location))
			{
				modContextMenu.Show(Cursor.Position);
			}
		}

		private void openFolderToolStripMenuItem_Click(object sender, EventArgs e)
		{
			foreach (ListViewItem item in modListView.SelectedItems)
			{
				Process.Start(Path.Combine("mods", (string)item.Tag));
			}
		}

		private void uninstallToolStripMenuItem_Click(object sender, EventArgs e)
		{
			DialogResult result = MessageBox.Show(this, "This will uninstall all selected mods."
				+ "\n\nAre you sure you wish to continue?", "Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

			if (result != DialogResult.Yes)
			{
				return;
			}

			result = MessageBox.Show(this, "Would you like to keep mod user data where possible? (Save files, config files, etc)",
				"User Data", MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

			if (result == DialogResult.Cancel)
			{
				return;
			}

			foreach (ListViewItem item in modListView.SelectedItems)
			{
				var dir = (string)item.Tag;
				var modDir = Path.Combine("mods", dir);
				var manpath = Path.Combine(modDir, "mod.manifest");

				try
				{
					if (result == DialogResult.Yes && File.Exists(manpath))
					{
						List<ModManifestEntry> manifest = ModManifest.FromFile(manpath);
						foreach (var entry in manifest)
						{
							var path = Path.Combine(modDir, entry.FilePath);
							if (File.Exists(path))
							{
								File.Delete(path);
							}
						}

						File.Delete(manpath);
						var version = Path.Combine(modDir, "mod.version");
						if (File.Exists(version))
						{
							File.Delete(version);
						}
					}
					else
					{
						if (result == DialogResult.Yes)
						{
							var retain = MessageBox.Show(this, $"The mod \"{ mods[dir].Name }\" (\"mods\\{ dir }\") does not have a manifest, so mod user data cannot be retained."
								+ " Do you want to uninstall it anyway?", "Cannot Retain User Data", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

							if (retain == DialogResult.No)
							{
								continue;
							}
						}

						Directory.Delete(modDir, true);
					}
				}
				catch (Exception ex)
				{
					MessageBox.Show(this, $"Failed to uninstall mod \"{ mods[dir].Name }\" from \"{ dir }\": { ex.Message }", "Failed",
						MessageBoxButtons.OK, MessageBoxIcon.Error);
				}
			}

			LoadModList();
		}

		private bool displayedManifestWarning;

		private void generateManifestToolStripMenuItem_Click(object sender, EventArgs e)
		{
			if (!displayedManifestWarning)
			{
				DialogResult result = MessageBox.Show(this, Properties.Resources.GenerateManifestWarning,
					"Warning", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

				if (result != DialogResult.Yes)
				{
					return;
				}

				displayedManifestWarning = true;
			}

			foreach (ListViewItem item in modListView.SelectedItems)
			{
				var modPath = Path.Combine("mods", (string)item.Tag);
				var manifestPath = Path.Combine(modPath, "mod.manifest");

				List<ModManifestEntry> manifest;
				List<ModManifestDiff> diff;

				using (var progress = new ManifestDialog(modPath, $"Generating manifest: {(string)item.Tag}", true))
				{
					progress.SetTask("Generating file index...");
					if (progress.ShowDialog(this) == DialogResult.Cancel)
					{
						continue;
					}

					diff = progress.Diff;
				}

				if (diff == null)
				{
					continue;
				}

				if (diff.Count(x => x.State != ModManifestState.Unchanged) <= 0)
				{
					continue;
				}

				using (var dialog = new ManifestDiffDialog(diff))
				{
					if (dialog.ShowDialog(this) == DialogResult.Cancel)
					{
						continue;
					}

					manifest = dialog.MakeNewManifest();
				}

				ModManifest.ToFile(manifest, manifestPath);
			}
		}

		private void UpdateSelectedMods()
		{
			InitializeWorker();
			manualModUpdate = true;
			updateChecker?.RunWorkerAsync(modListView.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Select(x => new KeyValuePair<string, ModInfo>(x, mods[x]))
				.ToList());
		}

		private void checkForUpdatesToolStripMenuItem_Click(object sender, EventArgs e)
		{
			UpdateSelectedMods();
		}

		private void forceUpdateToolStripMenuItem_Click(object sender, EventArgs e)
		{
			var result = MessageBox.Show(this, "This will force all selected mods to be completely re-downloaded."
				+ " Are you sure you want to continue?",
				"Force Update", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

			if (result != DialogResult.Yes)
			{
				return;
			}

			modUpdater.ForceUpdate = true;
			UpdateSelectedMods();
		}

		private void verifyToolStripMenuItem_Click(object sender, EventArgs e)
		{
			List<Tuple<string, ModInfo>> items = modListView.SelectedItems.Cast<ListViewItem>()
				.Select(x => (string)x.Tag)
				.Where(x => File.Exists(Path.Combine("mods", x, "mod.manifest")))
				.Select(x => new Tuple<string, ModInfo>(x, mods[x]))
				.ToList();

			if (items.Count < 1)
			{
				MessageBox.Show(this, "None of the selected mods have manifests, so they cannot be verified.",
					"Missing mod.manifest", MessageBoxButtons.OK, MessageBoxIcon.Warning);
				return;
			}

			using (var progress = new VerifyModDialog(items))
			{
				var result = progress.ShowDialog(this);
				if (result == DialogResult.Cancel)
				{
					return;
				}

				List<Tuple<string, ModInfo, List<ModManifestDiff>>> failed = progress.Failed;
				if (failed.Count < 1)
				{
					MessageBox.Show(this, "All selected mods passed verification.", "Integrity Pass",
						MessageBoxButtons.OK, MessageBoxIcon.Information);
				}
				else
				{
					result = MessageBox.Show(this, "The following mods failed verification:\n"
						+ string.Join("\n", failed.Select(x => $"{x.Item2.Name}: {x.Item3.Count(y => y.State != ModManifestState.Unchanged)} file(s)"))
						+ "\n\nWould you like to attempt repairs?",
						"Integrity Fail", MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

					if (result != DialogResult.Yes)
					{
						return;
					}

					InitializeWorker();

					updateChecker.DoWork -= UpdateChecker_DoWork;
					updateChecker.DoWork += UpdateChecker_DoWorkForced;

					updateChecker.RunWorkerAsync(failed);

					modUpdater.ForceUpdate = true;
					buttonCheckForUpdates.Enabled = false;
				}
			}
		}

		private void comboUpdateFrequency_SelectedIndexChanged(object sender, EventArgs e)
		{
			numericUpdateFrequency.Enabled = comboUpdateFrequency.SelectedIndex > 0;
		}

		private void buttonCheckForUpdates_Click(object sender, EventArgs e)
		{
			buttonCheckForUpdates.Enabled = false;

			if (CheckForUpdates(true))
			{
				return;
			}

			manualModUpdate = true;
			CheckForModUpdates(true);
		}

		private void installURLHandlerButton_Click(object sender, EventArgs e)
		{
			Process.Start(new ProcessStartInfo(Application.ExecutablePath, "urlhandler") { UseShellExecute = true, Verb = "runas" }).WaitForExit();
			MessageBox.Show(this, "URL handler installed!", Text);
		}

		private void configureModButton_Click(object sender, EventArgs e)
		{
			using (ModConfigDialog dlg = new ModConfigDialog(Path.Combine("mods", (string)modListView.SelectedItems[0].Tag), modListView.SelectedItems[0].Text))
				dlg.ShowDialog(this);
		}

		private void checkBoxTestSpawnPosition_CheckedChanged(object sender, EventArgs e)
		{
			labelTestSpawnX.Enabled = labelTestSpawnY.Enabled = labelTestSpawnZ.Enabled =
				numericUpDownTestSpawnX.Enabled = numericUpDownTestSpawnY.Enabled = numericUpDownTestSpawnZ.Enabled = checkBoxTestSpawnPosition.Checked;
		}

		private void checkBoxTestSpawnLevel_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnLevel.Enabled = checkBoxTestSpawnLevel.Checked;
			if (comboBoxTestSpawnLevel.SelectedIndex == -1)
				comboBoxTestSpawnLevel.SelectedIndex = 0;
		}

		private void checkBoxTestSpawnCharacter_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnCharacter.Enabled = checkBoxTestSpawnCharacter.Checked;
			if (comboBoxTestSpawnCharacter.SelectedIndex == -1)
				comboBoxTestSpawnCharacter.SelectedIndex = 0;
		}

		private void checkBoxMission_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxMission.Enabled = checkBoxMission.Checked;
			if (comboBoxMission.SelectedIndex == -1)
				comboBoxMission.SelectedIndex = 0;
		}

		private void checkBoxTestSpawnEvent_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnEvent.Enabled = checkBoxTestSpawnEvent.Checked;
			if (comboBoxTestSpawnEvent.SelectedIndex == -1)
				comboBoxTestSpawnEvent.SelectedIndex = 0;
		}

		private void checkBoxTestSpawnSave_CheckStateChanged(object sender, EventArgs e)
		{
			numericUpDownTestSpawnSaveID.Enabled = checkBoxTestSpawnSave.Checked;
		}

		private void InitTestSpawnCutsceneList()
		{
			TestSpawnCutsceneList = new Dictionary<int, string>();
			TestSpawnCutsceneList.Add(0, "Hero Story Intro");
			TestSpawnCutsceneList.Add(2, "Sonic Encounters Shadow");
			TestSpawnCutsceneList.Add(3, "Knuckles Meets Rouge");
			TestSpawnCutsceneList.Add(4, "Tails Flies to Prison Island");
			TestSpawnCutsceneList.Add(5, "Tails vs. Eggman Aftermath");
			TestSpawnCutsceneList.Add(6, "Sonic Escapes from his Cell");
			TestSpawnCutsceneList.Add(11, "Sonic and Friends Witness the ARK's Power");
			TestSpawnCutsceneList.Add(14, "Where's Eggman?");
			TestSpawnCutsceneList.Add(15, "Sonic And Tails intercept Eggman's Video.");
			TestSpawnCutsceneList.Add(16, "Locate Eggman's Pyramid Base");
			TestSpawnCutsceneList.Add(17, "Knuckles Begins Looking for the Gate Keys");
			TestSpawnCutsceneList.Add(19, "Eggman Ambushes Sonic inside the Base.");
			TestSpawnCutsceneList.Add(21, "Tails Explains His Plan to Destroy the Eclipse Cannon");
			TestSpawnCutsceneList.Add(22, "Sonic Receives a Call for Ransom from Eggman");
			TestSpawnCutsceneList.Add(24, "Knuckles vs. Rouge Aftermath");
			TestSpawnCutsceneList.Add(25, "Sonic and Eggman Face Off");
			TestSpawnCutsceneList.Add(26, "Sonic Saves Himself");
			TestSpawnCutsceneList.Add(27, "Sonic and Shadow Final Battle");
			TestSpawnCutsceneList.Add(28, "Hero Story Ending");

			TestSpawnCutsceneList.Add(100, "Dark Story Intro");
			TestSpawnCutsceneList.Add(101, "Eggman Awakens Shadow");
			TestSpawnCutsceneList.Add(102, "Shadow Instructs Eggman");
			TestSpawnCutsceneList.Add(103, "Rouge Meets Knuckles");
			TestSpawnCutsceneList.Add(105, "Eggman Hears of Shadow's Highway Robbery");
			TestSpawnCutsceneList.Add(106, "Shadow's Pathos");
			TestSpawnCutsceneList.Add(107, "Shadow Encounters Sonic");
			TestSpawnCutsceneList.Add(109, "Rouge Follows Eggman");
			TestSpawnCutsceneList.Add(111, "Gathering of the Dark");
			TestSpawnCutsceneList.Add(112, "Arrive on Prison Island");
			TestSpawnCutsceneList.Add(113, "Eggman's Plan is Complicated by Tails and Amy");
			TestSpawnCutsceneList.Add(116, "Shadow's Conscience Begins to Surface");
			TestSpawnCutsceneList.Add(118, "Sonic vs. Shadow 1 Aftermath");
			TestSpawnCutsceneList.Add(119, "Shadow's Past Life with Maria");
			TestSpawnCutsceneList.Add(120, "Eggman Reworks His Plan");
			TestSpawnCutsceneList.Add(122, "Shadow Spots the Heroes approaching the Pyramid Base");
			TestSpawnCutsceneList.Add(123, "Eggman Fails to Ambush Sonic inside the base.");
			TestSpawnCutsceneList.Add(124, "Rouge Learns the Truth About Project Shadow.");
			TestSpawnCutsceneList.Add(126, "Knuckles vs. Rouge Aftermath");
			TestSpawnCutsceneList.Add(127, "Eggman's Final Plan to Crush the Heroes");
			TestSpawnCutsceneList.Add(128, "Eggman Sets a Diabolical Trap for Sonic");
			TestSpawnCutsceneList.Add(129, "Mystery of Shadow the Hedgehog");
			TestSpawnCutsceneList.Add(130, "Shadow and Sonic Final Battle");
			TestSpawnCutsceneList.Add(131, "Dark Story Ending");

			TestSpawnCutsceneList.Add(200, "Last Episode Intro");
			TestSpawnCutsceneList.Add(201, "The ARK Falls");
			TestSpawnCutsceneList.Add(203, "Gerald's Diary and Descent into Madness");
			TestSpawnCutsceneList.Add(204, "Shadow and Amy.");
			TestSpawnCutsceneList.Add(205, "Gerald's Final Trump Card");
			TestSpawnCutsceneList.Add(206, "Knuckles Stops the Chaos Emeralds");
			TestSpawnCutsceneList.Add(207, "Sonic and Shadow Transform");
			TestSpawnCutsceneList.Add(208, "Shadow gives his life to save the world");
			TestSpawnCutsceneList.Add(210, "Last Episode Ending/Credits");
			TestSpawnCutsceneList.Add(211, "Epilogue");


			TestSpawnCutsceneList.Add(360, "SA2B Intro");
			TestSpawnCutsceneList.Add(401, "SA2 Intro (FMV)");
			TestSpawnCutsceneList.Add(409, "Prison Island Explodes");
			TestSpawnCutsceneList.Add(411, "Eggman's Threat to Earth");
			TestSpawnCutsceneList.Add(420, "Sonic and Friends go to Space (Hero)");
			TestSpawnCutsceneList.Add(428, "The Eclipse Cannon Malfunctions");
			TestSpawnCutsceneList.Add(429, "Dark Story Trailer");
			TestSpawnCutsceneList.Add(430, "Last Story Trailer");
			TestSpawnCutsceneList.Add(524, "Sonic and Friends go to Space (Dark)");
			TestSpawnCutsceneList.Add(532, "Hero Story Trailer");
			TestSpawnCutsceneList.Add(602, "Gerald's Revenge");
			TestSpawnCutsceneList.Add(609, "The ARK is Neutralized");

			comboBoxTestSpawnEvent.Items.Clear();
			foreach (var item in TestSpawnCutsceneList)
			{
				comboBoxTestSpawnEvent.Items.Add("EV" + item.Key.ToString() + ": " + item.Value);
			}
		}


		private void InitTestSpawnLevelList()
		{
			TestSpawnLevelList = new Dictionary<int, string>();
			TestSpawnLevelList.Add(0, "Basic Test");
			TestSpawnLevelList.Add(3, "Green Forest");
			TestSpawnLevelList.Add(4, "White Jungle");
			TestSpawnLevelList.Add(5, "Pumpkin Hill");
			TestSpawnLevelList.Add(6, "Sky Rail");
			TestSpawnLevelList.Add(7, "Aquatic Mine");
			TestSpawnLevelList.Add(8, "Security Hall");
			TestSpawnLevelList.Add(9, "Prison Lane");
			TestSpawnLevelList.Add(10, "Metal Harbor");
			TestSpawnLevelList.Add(11, "Iron Gate");
			TestSpawnLevelList.Add(12, "Weapons Bed");
			TestSpawnLevelList.Add(13, "City Escape");
			TestSpawnLevelList.Add(14, "Radical Highway");
			TestSpawnLevelList.Add(15, "Weapons Bed 2P");
			TestSpawnLevelList.Add(16, "Wild Canyon");
			TestSpawnLevelList.Add(17, "Mission Street");
			TestSpawnLevelList.Add(18, "Dry Lagoon");
			TestSpawnLevelList.Add(19, "Sonic Vs. Shadow 1");
			TestSpawnLevelList.Add(20, "Tails Vs. Eggman 1");
			TestSpawnLevelList.Add(21, "Sand Ocean");
			TestSpawnLevelList.Add(22, "Crazy Gadget");
			TestSpawnLevelList.Add(23, "Hidden Base");
			TestSpawnLevelList.Add(24, "Eternal Engine");
			TestSpawnLevelList.Add(25, "Death Chamber");
			TestSpawnLevelList.Add(26, "Egg Quarters");
			TestSpawnLevelList.Add(27, "Lost Colony");
			TestSpawnLevelList.Add(28, "Pyramid Cave");
			TestSpawnLevelList.Add(29, "Tails Vs. Eggman 2");
			TestSpawnLevelList.Add(30, "Final Rush");
			TestSpawnLevelList.Add(31, "Green Hill");
			TestSpawnLevelList.Add(32, "Meteor Herd");
			TestSpawnLevelList.Add(33, "Knuckles Vs. Rouge");
			TestSpawnLevelList.Add(34, "Cannon's Core (Sonic)");
			TestSpawnLevelList.Add(35, "Cannon's Core (Eggman)");
			TestSpawnLevelList.Add(36, "Cannon's Core (Tails)");
			TestSpawnLevelList.Add(37, "Cannon's Core (Rouge)");
			TestSpawnLevelList.Add(38, "Cannon's Core (Knuckles)");
			TestSpawnLevelList.Add(39, "Mission Street 2P");
			TestSpawnLevelList.Add(40, "Final Chase");
			TestSpawnLevelList.Add(41, "Wild Canyon 2P");
			TestSpawnLevelList.Add(42, "Sonic Vs. Shadow 2");
			TestSpawnLevelList.Add(43, "Cosmic Wall");
			TestSpawnLevelList.Add(44, "Mad Space");
			TestSpawnLevelList.Add(45, "Sand Ocean 2P");
			TestSpawnLevelList.Add(46, "Dry Lagoon 2P");
			TestSpawnLevelList.Add(47, "Pyramid Race");
			TestSpawnLevelList.Add(48, "Hidden Base 2P");
			TestSpawnLevelList.Add(49, "Pool Quest");
			TestSpawnLevelList.Add(50, "Planet Quest");
			TestSpawnLevelList.Add(51, "Deck Race");
			TestSpawnLevelList.Add(52, "Downtown Race");
			TestSpawnLevelList.Add(53, "Cosmic Wall 2P");
			TestSpawnLevelList.Add(54, "Grind Race");
			TestSpawnLevelList.Add(55, "Lost Colony 2P");
			TestSpawnLevelList.Add(56, "Eternal Engine 2P");
			TestSpawnLevelList.Add(57, "Metal Harbor 2P");
			TestSpawnLevelList.Add(58, "Iron Gate 2P");
			TestSpawnLevelList.Add(59, "Death Chamber 2P");
			TestSpawnLevelList.Add(60, "Big Foot");
			TestSpawnLevelList.Add(61, "Hot Shot");
			TestSpawnLevelList.Add(62, "Flying Dog");
			TestSpawnLevelList.Add(63, "King Boom Boo");
			TestSpawnLevelList.Add(64, "Egg Golem (Sonic)");
			TestSpawnLevelList.Add(65, "Biolizard");
			TestSpawnLevelList.Add(66, "FinalHazard");
			TestSpawnLevelList.Add(67, "Egg Golem (Eggman)");
			TestSpawnLevelList.Add(70, "Route 101/Route 280");
			TestSpawnLevelList.Add(71, "Kart Race");
			TestSpawnLevelList.Add(90, "Chao World");

			comboBoxTestSpawnLevel.Items.Clear();
			foreach (var item in TestSpawnLevelList)
			{
				comboBoxTestSpawnLevel.Items.Add(item.Key.ToString() + ": " + item.Value);
			}
		}

		private string GetTestSpawnCommandLine()
		{
			List<string> cmdline = new List<string>();
			if (checkBoxTestSpawnLevel.Checked)
			{
				int lvl = 0;
				int lvl_result = 0;
				foreach (var item in TestSpawnLevelList)
				{
					if (lvl == comboBoxTestSpawnLevel.SelectedIndex)
					{
						lvl_result = item.Key;
						break;
					}
					lvl++;
				}
				cmdline.Add("-l " + lvl_result.ToString());
			}


			if (checkBoxTestSpawnCharacter.Checked)
				cmdline.Add("-c " + comboBoxTestSpawnCharacter.SelectedIndex.ToString());

			if (checkBoxTestSpawnPlayer2.Checked)
				cmdline.Add("-p2 " + comboBoxTestSpawnPlayer2.SelectedIndex.ToString());

			if (checkBoxTestSpawnPosition.Checked)
				cmdline.Add("-p " + numericUpDownTestSpawnX.Value.ToString() + " " +
					numericUpDownTestSpawnY.Value.ToString() + " " +
					numericUpDownTestSpawnZ.Value.ToString());

			if (checkBoxTestSpawnEvent.Checked)
			{
				int ev = 0;
				int ev_result = 0;
				foreach (var item in TestSpawnCutsceneList)
				{
					if (ev == comboBoxTestSpawnEvent.SelectedIndex)
					{
						ev_result = item.Key;
						break;
					}
					ev++;
				}
				cmdline.Add("-e " + ev_result.ToString());
			}

			if (checkBoxMission.Checked)
				cmdline.Add("-m " + comboBoxMission.SelectedIndex.ToString());

			if (checkBoxTestSpawnSave.Checked)
				cmdline.Add("-s " + numericUpDownTestSpawnSaveID.Value.ToString());



			return string.Join(" ", cmdline);
		}

		private void buttonTestSpawnPlay_Click(object sender, EventArgs e)
		{
			Process process = Process.Start(loaderini.Mods.Select((item) => mods[item].EXEFile)
												.FirstOrDefault((item) => !string.IsNullOrEmpty(item)) ?? "sonic2app.exe", GetTestSpawnCommandLine());
		}


		private void profileNameBox_TextChanged(object sender, EventArgs e)
		{
			if (string.IsNullOrEmpty(profileNameBox.Text) || profileNameBox.Text.Equals("SA2ModLoader", StringComparison.OrdinalIgnoreCase)
				|| profileNameBox.Text.Equals("mod", StringComparison.OrdinalIgnoreCase) || profileNameBox.Text.Equals("desktop", StringComparison.OrdinalIgnoreCase)
				|| profileNameBox.Text.IndexOfAny(Path.GetInvalidFileNameChars()) != -1)
			{
				buttonSaveProfile.Enabled = false;
				buttonLoadProfile.Enabled = false;
			}
			else
			{
				buttonSaveProfile.Enabled = true;
				buttonLoadProfile.Enabled = File.Exists($"mods/{profileNameBox.Text}.ini");
			}
		}

		private void buttonLoadProfile_Click(object sender, EventArgs e)
		{
			loaderini = IniSerializer.Deserialize<SA2LoaderInfo>($"mods/{profileNameBox.Text}.ini");
			LoadSettings();
		}

		private void buttonSaveProfile_Click(object sender, EventArgs e)
		{
			Save();
			File.Copy(loaderinipath, $"mods/{profileNameBox.Text}.ini", true);
			if (profileNameBox.FindStringExact(profileNameBox.Text) == -1)
				profileNameBox.Items.Add(profileNameBox.Text);
			buttonLoadProfile.Enabled = true;
		}

		private void checkBoxTestSpawnPlayer2_CheckedChanged(object sender, EventArgs e)
		{
			comboBoxTestSpawnPlayer2.Enabled = checkBoxTestSpawnPlayer2.Checked;

			if (comboBoxTestSpawnPlayer2.SelectedIndex == -1)
				comboBoxTestSpawnPlayer2.SelectedIndex = 0;
		}


		private void horizontalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (!suppressEvent)
				comboResolutionPreset.SelectedIndex = -1;
		}

		private void verticalResolution_ValueChanged(object sender, EventArgs e)
		{
			if (!suppressEvent)
				comboResolutionPreset.SelectedIndex = -1;
		}

		static readonly Size[] resolutionPresets =
		{
			new Size(640, 480), // 640x480
			new Size(800, 600), // 800x600
			new Size(1024, 768), // 1024x768
			new Size(1152, 864), // 1152x864
			new Size(1280, 960), // 1280x960
			new Size(1280, 1024), // 1280x1024
			new Size(), // Native
			new Size(), // 1/2x Native
			new Size(), // 2x Native
			new Size(1280, 720), // 720p
			new Size(1920, 1080), // 1080p
			new Size(3840, 2160), // 4K
		};

		private void comboResolutionPreset_SelectedIndexChanged(object sender, EventArgs e)
		{
			if (comboResolutionPreset.SelectedIndex == -1)
				return;

			suppressEvent = true;
			verticalResolution.Value = resolutionPresets[comboResolutionPreset.SelectedIndex].Height;
			horizontalResolution.Value = resolutionPresets[comboResolutionPreset.SelectedIndex].Width;
			suppressEvent = false;
		}

		private void screenNumComboBox_SelectedIndexChanged(object sender, EventArgs e)
		{
			Size oldsize = resolutionPresets[6];
			Rectangle rect = Screen.PrimaryScreen.Bounds;

			if (screenNumComboBox.SelectedIndex > 0)
				rect = Screen.AllScreens[screenNumComboBox.SelectedIndex - 1].Bounds;
			else
				foreach (Screen screen in Screen.AllScreens)
					rect = Rectangle.Union(rect, screen.Bounds);

			resolutionPresets[6] = rect.Size;
			resolutionPresets[7] = new Size(rect.Width / 2, rect.Height / 2);
			resolutionPresets[8] = new Size(rect.Width * 2, rect.Height * 2);

			if (comboResolutionPreset.SelectedIndex > 4 && comboResolutionPreset.SelectedIndex < 8 && rect.Size != oldsize)
				comboResolutionPreset.SelectedIndex = -1;

		}

		private void customWindowSizeCheckBox_CheckedChanged(object sender, EventArgs e)
		{
			var check = ((CheckBox)sender).Checked;
			windowHeight.Enabled = check;
			windowWidth.Enabled = check;
			checkWindowResize.Enabled = !check;
		}

		private void radioFullscreen_CheckedChanged(object sender, EventArgs e)
		{
			groupBox_WindowMode.Enabled = !((RadioButton)sender).Checked;
		}
	}
}