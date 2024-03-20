using System;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Reflection;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;
using UpgradeTool;

namespace ModManagerCommon.Forms
{
	public class WPFDownloadDialog : ProgressDialog
	{
		private readonly string url;
		private readonly string updatePath;
		private readonly CancellationTokenSource tokenSource = new CancellationTokenSource();

		public WPFDownloadDialog(string url, string updatePath)
			: base("Download Progress", true)
		{
			this.url = url;
			this.updatePath = Path.GetFullPath(updatePath);

			Shown += OnShown;
			CancelEvent += OnCancelEvent;
		}

		private void OnCancelEvent(object sender, EventArgs eventArgs)
		{
			tokenSource.Cancel();
		}

		private void OnShown(object sender, EventArgs eventArgs)
		{
			DialogResult = DialogResult.OK;

			SetTaskCount(1);

			using (var client = new UpdaterWebClient())
			{
				CancellationToken token = tokenSource.Token;

				void OnDownloadProgress(object o, DownloadProgressEventArgs args)
				{
					SetProgress(args.BytesReceived / (double)args.TotalBytesToReceive);
					SetTaskAndStep($"Downloading file:",
						$"({SizeSuffix.GetSizeSuffix(args.BytesReceived)} / {SizeSuffix.GetSizeSuffix(args.TotalBytesToReceive)})");
					args.Cancel = token.IsCancellationRequested;
				}
				void OnDownloadCompleted(object o, CancelEventArgs args)
				{
					NextTask();
					args.Cancel = token.IsCancellationRequested;
				}

				DialogResult result;

				SetTaskAndStep("Starting download...");

				do
				{
					result = DialogResult.Cancel;

					try
					{
						// poor man's await Task.Run (not available in .net 4.0)
						using (var task = new Task(() =>
						{
							var cancelArgs = new CancelEventArgs(false);
							DownloadProgressEventArgs downloadArgs = null;

							void DownloadComplete(object _sender, AsyncCompletedEventArgs args)
							{
								lock (args.UserState)
								{
									Monitor.Pulse(args.UserState);
								}
							}

							void DownloadProgressChanged(object _sender, DownloadProgressChangedEventArgs args)
							{
								downloadArgs = new DownloadProgressEventArgs(args, 1, 1);
								OnDownloadProgress(this, downloadArgs);
								if (downloadArgs.Cancel)
								{
									client.CancelAsync();
								}
							}

							var uri = new Uri(url);
							string filePath = Path.GetFullPath(Path.Combine(updatePath, uri.Segments.Last()));

							var info = new FileInfo(filePath);
							client.DownloadFileCompleted += DownloadComplete;
							client.DownloadProgressChanged += DownloadProgressChanged;

							var sync = new object();
							lock (sync)
							{
								client.DownloadFileAsync(uri, filePath, sync);
								Monitor.Wait(sync);
							}

							client.DownloadProgressChanged -= DownloadProgressChanged;
							client.DownloadFileCompleted -= DownloadComplete;

							if (cancelArgs.Cancel || downloadArgs?.Cancel == true)
							{
								return;
							}

							OnDownloadCompleted(this, cancelArgs);
							if (cancelArgs.Cancel)
							{
								return;
							}

							string managerDLPath = updatePath;

							SetTaskAndStep("Extracting...");
							if (token.IsCancellationRequested)
							{
								return;
							}

							Process.Start(new ProcessStartInfo("7z.exe", $"x -aoa -o\"{managerDLPath}\" \"{filePath}\"") { UseShellExecute = false, CreateNoWindow = true }).WaitForExit();
							string NewManagerPath = Path.GetFullPath(Path.Combine(managerDLPath, "SAModManager.exe"));
							string dest = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "SAModManager.exe");

							string gameDirectory = Program.FindGameDirectory(AppDomain.CurrentDomain.BaseDirectory, Program.exeName);

							if (gameDirectory is null) //dirty failsafe but in theory shouldn't be needed
							{
								bool first = dest.ToLower().Contains("mods/.updates");
								bool second = dest.ToLower().Contains("mods\\.updates");

								if (first)
								{
									dest = dest.Replace("mods/.updates/" + Program.oldLoaderExeName,  "");
								}
								else if (second)
								{
									dest = dest.Replace("mods\\.updates\\" + Program.oldLoaderExeName, "");
								}
							}
							else
							{
								dest = Path.Combine(gameDirectory, "SAModManager.exe");
							}

							if (File.Exists(NewManagerPath))
							{
								if (File.Exists(dest)) //just in case
								{
									File.Delete(dest);
								}

								File.Move(NewManagerPath, dest); //we move the new manager to the game directory
							}

							if (File.Exists("SAModManager.exe"))
							{
								var startInfo = new ProcessStartInfo
								{
									FileName = "SAModManager.exe",
									Arguments = "vanillaUpdate",
									UseShellExecute = true,
									WorkingDirectory = Environment.CurrentDirectory,
								};
								Process.Start(startInfo);
							}
							else if (File.Exists(dest))
							{
								var startInfo = new ProcessStartInfo
								{
									FileName = dest,
									Arguments = "vanillaUpdate",
									UseShellExecute = true,
									WorkingDirectory = Path.GetDirectoryName(dest),
								};
								Process.Start(startInfo);
							}
							else
							{
								MessageBox.Show("Failed to open the new manager\nplease download the update manually.");
							}
						}, token))
						{
							task.Start();

							while (!task.IsCompleted && !task.IsCanceled)
							{
								Application.DoEvents();
							}

							task.Wait(token);
						}
					}
					catch (AggregateException ae)
					{
						ae.Handle(ex =>
						{
							result = MessageBox.Show(this, $"Failed to update:\r\n{ex.Message}",
								"Update Failed", MessageBoxButtons.RetryCancel, MessageBoxIcon.Error);
							return true;
						});
					}
				} while (result == DialogResult.Retry);

			}
		}

		protected override void Dispose(bool disposing)
		{
			tokenSource.Dispose();
			base.Dispose(disposing);
		}
	}
}
