namespace SA2ModManager
{
	partial class MainForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{   
			this.components = new System.ComponentModel.Container();
            System.Windows.Forms.GroupBox groupBox2;
            System.Windows.Forms.Label label1;
            System.Windows.Forms.Label label5;
            System.Windows.Forms.Label label3;
            System.Windows.Forms.Label label2;
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
            this.crashLogCheckBox = new System.Windows.Forms.CheckBox();
            this.screenCheckBox = new System.Windows.Forms.CheckBox();
            this.fileCheckBox = new System.Windows.Forms.CheckBox();
            this.consoleCheckBox = new System.Windows.Forms.CheckBox();
            this.modDescription = new System.Windows.Forms.Label();
            this.modDownButton = new System.Windows.Forms.Button();
            this.modUpButton = new System.Windows.Forms.Button();
            this.modListView = new System.Windows.Forms.ListView();
            this.columnHeaderName = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderAuthor = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
            this.columnHeaderVersion = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.columnHeaderCategory = ((System.Windows.Forms.ColumnHeader)(new System.Windows.Forms.ColumnHeader()));
			this.saveButton = new System.Windows.Forms.Button();
            this.saveAndPlayButton = new System.Windows.Forms.Button();
            this.installButton = new System.Windows.Forms.Button();
            this.tabControl1 = new System.Windows.Forms.TabControl();
            this.tabPageMods = new System.Windows.Forms.TabPage();
            this.configureModButton = new System.Windows.Forms.Button();
            this.modBottomButton = new System.Windows.Forms.Button();
            this.modTopButton = new System.Windows.Forms.Button();
            this.buttonNewMod = new System.Windows.Forms.Button();
            this.buttonRefreshModList = new System.Windows.Forms.Button();
            this.tabPageCodes = new System.Windows.Forms.TabPage();
            this.codesCheckedListBox = new System.Windows.Forms.CheckedListBox();
            this.tabPageGraphics = new System.Windows.Forms.TabPage();
            this.groupBox_Visuals = new System.Windows.Forms.GroupBox();
            this.checkFrameLimiter = new System.Windows.Forms.CheckBox();
            this.groupBox_WindowMode = new System.Windows.Forms.GroupBox();
            this.aspectRatioCheckBox = new System.Windows.Forms.CheckBox();
            this.checkWindowResize = new System.Windows.Forms.CheckBox();
            this.customWindowSizeCheckBox = new System.Windows.Forms.CheckBox();
            this.windowHeight = new System.Windows.Forms.NumericUpDown();
            this.windowWidth = new System.Windows.Forms.NumericUpDown();
            this.groupBox_Display = new System.Windows.Forms.GroupBox();
            this.radioBorderlessWindowMode = new System.Windows.Forms.RadioButton();
            this.screenNumComboBox = new System.Windows.Forms.ComboBox();
            this.comboResolutionPreset = new System.Windows.Forms.ComboBox();
            this.radioWindowMode = new System.Windows.Forms.RadioButton();
            this.radioFullscreen = new System.Windows.Forms.RadioButton();
            this.horizontalResolution = new System.Windows.Forms.NumericUpDown();
            this.verticalResolution = new System.Windows.Forms.NumericUpDown();
            this.tabPageOptions = new System.Windows.Forms.TabPage();
            this.groupBox3 = new System.Windows.Forms.GroupBox();
            this.buttonSaveProfile = new System.Windows.Forms.Button();
            this.buttonLoadProfile = new System.Windows.Forms.Button();
            this.profileNameBox = new System.Windows.Forms.ComboBox();
            this.installURLHandlerButton = new System.Windows.Forms.Button();
            this.groupBox4 = new System.Windows.Forms.GroupBox();
            this.buttonCheckForUpdates = new System.Windows.Forms.Button();
            this.label4 = new System.Windows.Forms.Label();
            this.checkUpdateModsStartup = new System.Windows.Forms.CheckBox();
            this.numericUpdateFrequency = new System.Windows.Forms.NumericUpDown();
            this.comboUpdateFrequency = new System.Windows.Forms.ComboBox();
            this.checkUpdateStartup = new System.Windows.Forms.CheckBox();
            this.groupBox1 = new System.Windows.Forms.GroupBox();
            this.disableExitPromptCheckBox = new System.Windows.Forms.CheckBox();
            this.comboTextLanguage = new System.Windows.Forms.ComboBox();
            this.comboVoiceLanguage = new System.Windows.Forms.ComboBox();
            this.label9 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.skipIntrocheckBox = new System.Windows.Forms.CheckBox();
            this.pauseWhenInactiveCheckBox = new System.Windows.Forms.CheckBox();
            this.tabPageDebug = new System.Windows.Forms.TabPage();
            this.groupBox12 = new System.Windows.Forms.GroupBox();
            this.labelTestSpawnX = new System.Windows.Forms.Label();
            this.checkBoxTestSpawnPlayer2 = new System.Windows.Forms.CheckBox();
            this.comboBoxTestSpawnPlayer2 = new System.Windows.Forms.ComboBox();
            this.checkBoxMission = new System.Windows.Forms.CheckBox();
            this.comboBoxMission = new System.Windows.Forms.ComboBox();
            this.checkBoxTestSpawnPosition = new System.Windows.Forms.CheckBox();
            this.buttonTestSpawnPlay = new System.Windows.Forms.Button();
            this.comboBoxTestSpawnEvent = new System.Windows.Forms.ComboBox();
            this.checkBoxTestSpawnEvent = new System.Windows.Forms.CheckBox();
            this.checkBoxTestSpawnCharacter = new System.Windows.Forms.CheckBox();
            this.checkBoxTestSpawnLevel = new System.Windows.Forms.CheckBox();
            this.comboBoxTestSpawnCharacter = new System.Windows.Forms.ComboBox();
            this.labelTestSpawnY = new System.Windows.Forms.Label();
            this.numericUpDownTestSpawnSaveID = new System.Windows.Forms.NumericUpDown();
            this.checkBoxTestSpawnSave = new System.Windows.Forms.CheckBox();
            this.labelTestSpawnZ = new System.Windows.Forms.Label();
            this.numericUpDownTestSpawnZ = new System.Windows.Forms.NumericUpDown();
            this.numericUpDownTestSpawnY = new System.Windows.Forms.NumericUpDown();
            this.comboBoxTestSpawnLevel = new System.Windows.Forms.ComboBox();
            this.numericUpDownTestSpawnX = new System.Windows.Forms.NumericUpDown();
            this.toolTip = new System.Windows.Forms.ToolTip(this.components);
            this.modContextMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
            this.openFolderToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripSeparator();
            this.checkForUpdatesToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.verifyToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.forceUpdateToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.uninstallToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem3 = new System.Windows.Forms.ToolStripSeparator();
            this.developerToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.generateManifestToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            groupBox2 = new System.Windows.Forms.GroupBox();
            label1 = new System.Windows.Forms.Label();
            label5 = new System.Windows.Forms.Label();
            label3 = new System.Windows.Forms.Label();
            label2 = new System.Windows.Forms.Label();
            groupBox2.SuspendLayout();
            this.tabControl1.SuspendLayout();
            this.tabPageMods.SuspendLayout();
            this.tabPageCodes.SuspendLayout();
            this.tabPageGraphics.SuspendLayout();
            this.groupBox_Visuals.SuspendLayout();
            this.groupBox_WindowMode.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.windowHeight)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.windowWidth)).BeginInit();
            this.groupBox_Display.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.horizontalResolution)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.verticalResolution)).BeginInit();
            this.tabPageOptions.SuspendLayout();
            this.groupBox3.SuspendLayout();
            this.groupBox4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpdateFrequency)).BeginInit();
            this.groupBox1.SuspendLayout();
            this.tabPageDebug.SuspendLayout();
            this.groupBox12.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnSaveID)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnZ)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnX)).BeginInit();
            this.modContextMenu.SuspendLayout();
            this.SuspendLayout();
            // 
            // groupBox2
            // 
            groupBox2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            groupBox2.Controls.Add(this.crashLogCheckBox);
            groupBox2.Controls.Add(this.screenCheckBox);
            groupBox2.Controls.Add(this.fileCheckBox);
            groupBox2.Controls.Add(this.consoleCheckBox);
            groupBox2.Location = new System.Drawing.Point(6, 6);
            groupBox2.Name = "groupBox2";
            groupBox2.Size = new System.Drawing.Size(390, 42);
            groupBox2.TabIndex = 0;
            groupBox2.TabStop = false;
            groupBox2.Text = "Debug Messages";
            // 
            // crashLogCheckBox
            // 
            this.crashLogCheckBox.AutoSize = true;
            this.crashLogCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.crashLogCheckBox.Location = new System.Drawing.Point(208, 19);
            this.crashLogCheckBox.Name = "crashLogCheckBox";
            this.crashLogCheckBox.Size = new System.Drawing.Size(99, 18);
            this.crashLogCheckBox.TabIndex = 3;
            this.crashLogCheckBox.Text = "Crash Handler";
            this.toolTip.SetToolTip(this.crashLogCheckBox, "Displays the crash address and generates a crash dump when the game crashes.");
            this.crashLogCheckBox.UseVisualStyleBackColor = true;
            // 
            // screenCheckBox
            // 
            this.screenCheckBox.AutoSize = true;
            this.screenCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.screenCheckBox.Location = new System.Drawing.Point(82, 19);
            this.screenCheckBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.screenCheckBox.Name = "screenCheckBox";
            this.screenCheckBox.Size = new System.Drawing.Size(66, 18);
            this.screenCheckBox.TabIndex = 1;
            this.screenCheckBox.Text = "Screen";
            this.screenCheckBox.UseVisualStyleBackColor = true;
            // 
            // fileCheckBox
            // 
            this.fileCheckBox.AutoSize = true;
            this.fileCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.fileCheckBox.Location = new System.Drawing.Point(154, 19);
            this.fileCheckBox.Name = "fileCheckBox";
            this.fileCheckBox.Size = new System.Drawing.Size(48, 18);
            this.fileCheckBox.TabIndex = 2;
            this.fileCheckBox.Text = "File";
            this.fileCheckBox.UseVisualStyleBackColor = true;
            // 
            // consoleCheckBox
            // 
            this.consoleCheckBox.AutoSize = true;
            this.consoleCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.consoleCheckBox.Location = new System.Drawing.Point(6, 19);
            this.consoleCheckBox.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.consoleCheckBox.Name = "consoleCheckBox";
            this.consoleCheckBox.Size = new System.Drawing.Size(70, 18);
            this.consoleCheckBox.TabIndex = 0;
            this.consoleCheckBox.Text = "Console";
            this.toolTip.SetToolTip(this.consoleCheckBox, "Shows a console and displays debug messages.");
            this.consoleCheckBox.UseVisualStyleBackColor = true;
            // 
            // label1
            // 
            label1.AutoSize = true;
            label1.Location = new System.Drawing.Point(140, 50);
            label1.Name = "label1";
            label1.Size = new System.Drawing.Size(12, 13);
            label1.TabIndex = 4;
            label1.Text = "x";
            // 
            // label5
            // 
            label5.AutoSize = true;
            label5.Location = new System.Drawing.Point(6, 49);
            label5.Name = "label5";
            label5.Size = new System.Drawing.Size(60, 13);
            label5.TabIndex = 2;
            label5.Text = "Resolution:";
            // 
            // label3
            // 
            label3.AutoSize = true;
            label3.Location = new System.Drawing.Point(6, 22);
            label3.Name = "label3";
            label3.Size = new System.Drawing.Size(44, 13);
            label3.TabIndex = 0;
            label3.Text = "Screen:";
            // 
            // label2
            // 
            label2.AutoSize = true;
            label2.Location = new System.Drawing.Point(219, 22);
            label2.Name = "label2";
            label2.Size = new System.Drawing.Size(12, 13);
            label2.TabIndex = 2;
            label2.Text = "x";
            // 
            // modDescription
            // 
            this.modDescription.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.modDescription.Location = new System.Drawing.Point(6, 316);
            this.modDescription.Name = "modDescription";
            this.modDescription.Size = new System.Drawing.Size(390, 60);
            this.modDescription.TabIndex = 8;
            this.modDescription.Text = "Description: No mod selected.";
            // 
            // modDownButton
            // 
            this.modDownButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modDownButton.AutoSize = true;
            this.modDownButton.Enabled = false;
            this.modDownButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modDownButton.Location = new System.Drawing.Point(369, 78);
            this.modDownButton.Name = "modDownButton";
            this.modDownButton.Size = new System.Drawing.Size(31, 29);
            this.modDownButton.TabIndex = 5;
            this.modDownButton.Text = "↓";
            this.modDownButton.UseVisualStyleBackColor = true;
            this.modDownButton.Click += new System.EventHandler(this.modDownButton_Click);
            // 
            // modUpButton
            // 
            this.modUpButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modUpButton.AutoSize = true;
            this.modUpButton.Enabled = false;
            this.modUpButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modUpButton.Location = new System.Drawing.Point(369, 42);
            this.modUpButton.Name = "modUpButton";
            this.modUpButton.Size = new System.Drawing.Size(31, 29);
            this.modUpButton.TabIndex = 4;
            this.modUpButton.Text = "↑";
            this.modUpButton.UseVisualStyleBackColor = true;
            this.modUpButton.Click += new System.EventHandler(this.modUpButton_Click);
            // 
            // modListView
            // 
            this.modListView.AllowDrop = true;
            this.modListView.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.modListView.CheckBoxes = true;
            this.modListView.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeaderName,
            this.columnHeaderAuthor,
            this.columnHeaderVersion,
			this.columnHeaderCategory});
			this.modListView.FullRowSelect = true;
            this.modListView.HideSelection = false;
            this.modListView.Location = new System.Drawing.Point(6, 6);
            this.modListView.Name = "modListView";
            this.modListView.Size = new System.Drawing.Size(361, 275);
            this.modListView.TabIndex = 0;
            this.modListView.UseCompatibleStateImageBehavior = false;
            this.modListView.View = System.Windows.Forms.View.Details;
            this.modListView.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.modListView_ItemCheck);
            this.modListView.ItemDrag += new System.Windows.Forms.ItemDragEventHandler(this.modListView_ItemDrag);
            this.modListView.SelectedIndexChanged += new System.EventHandler(this.modListView_SelectedIndexChanged);
            this.modListView.DragDrop += new System.Windows.Forms.DragEventHandler(this.modListView_DragDrop);
            this.modListView.DragEnter += new System.Windows.Forms.DragEventHandler(this.modListView_DragEnter);
            this.modListView.DragOver += new System.Windows.Forms.DragEventHandler(this.modListView_DragOver);
            this.modListView.MouseClick += new System.Windows.Forms.MouseEventHandler(this.modListView_MouseClick);
            // 
            // columnHeaderName
            // 
            this.columnHeaderName.Text = "Name";
            this.columnHeaderName.Width = 152;
            // 
            // columnHeaderAuthor
            // 
            this.columnHeaderAuthor.Text = "Author";
            this.columnHeaderAuthor.Width = 130;
            // 
            // columnHeaderVersion
            // 
            this.columnHeaderVersion.Text = "Version";
            this.columnHeaderVersion.Width = 55;
			// 
			// columnHeaderCategory
			// 
			this.columnHeaderCategory.Text = "Category";
			this.columnHeaderCategory.Width = 94;
			// 
			// saveButton
			// 
			this.saveButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.saveButton.AutoSize = true;
            this.saveButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.saveButton.Location = new System.Drawing.Point(88, 411);
            this.saveButton.Name = "saveButton";
            this.saveButton.Size = new System.Drawing.Size(75, 22);
            this.saveButton.TabIndex = 2;
            this.saveButton.Text = "&Save";
            this.saveButton.UseVisualStyleBackColor = true;
            this.saveButton.Click += new System.EventHandler(this.saveButton_Click);
            // 
            // saveAndPlayButton
            // 
            this.saveAndPlayButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.saveAndPlayButton.AutoSize = true;
            this.saveAndPlayButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.saveAndPlayButton.Location = new System.Drawing.Point(4, 411);
            this.saveAndPlayButton.Name = "saveAndPlayButton";
            this.saveAndPlayButton.Size = new System.Drawing.Size(78, 22);
            this.saveAndPlayButton.TabIndex = 1;
            this.saveAndPlayButton.Text = "Save && &Play";
            this.saveAndPlayButton.UseVisualStyleBackColor = true;
            this.saveAndPlayButton.Click += new System.EventHandler(this.saveAndPlayButton_Click);
            // 
            // installButton
            // 
            this.installButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.installButton.AutoSize = true;
            this.installButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.installButton.Location = new System.Drawing.Point(168, 411);
            this.installButton.Name = "installButton";
            this.installButton.Size = new System.Drawing.Size(80, 22);
            this.installButton.TabIndex = 3;
            this.installButton.Text = "Install loader";
            this.installButton.UseVisualStyleBackColor = true;
            this.installButton.Click += new System.EventHandler(this.installButton_Click);
            // 
            // tabControl1
            // 
            this.tabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.tabControl1.Controls.Add(this.tabPageMods);
            this.tabControl1.Controls.Add(this.tabPageCodes);
            this.tabControl1.Controls.Add(this.tabPageGraphics);
            this.tabControl1.Controls.Add(this.tabPageOptions);
            this.tabControl1.Controls.Add(this.tabPageDebug);
            this.tabControl1.Location = new System.Drawing.Point(0, 0);
            this.tabControl1.Margin = new System.Windows.Forms.Padding(0, 0, 0, 3);
            this.tabControl1.Name = "tabControl1";
            this.tabControl1.SelectedIndex = 0;
            this.tabControl1.Size = new System.Drawing.Size(410, 405);
            this.tabControl1.TabIndex = 0;
            // 
            // tabPageMods
            // 
            this.tabPageMods.Controls.Add(this.configureModButton);
            this.tabPageMods.Controls.Add(this.modBottomButton);
            this.tabPageMods.Controls.Add(this.modTopButton);
            this.tabPageMods.Controls.Add(this.buttonNewMod);
            this.tabPageMods.Controls.Add(this.buttonRefreshModList);
            this.tabPageMods.Controls.Add(this.modDescription);
            this.tabPageMods.Controls.Add(this.modListView);
            this.tabPageMods.Controls.Add(this.modDownButton);
            this.tabPageMods.Controls.Add(this.modUpButton);
            this.tabPageMods.Location = new System.Drawing.Point(4, 22);
            this.tabPageMods.Name = "tabPageMods";
            this.tabPageMods.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageMods.Size = new System.Drawing.Size(402, 379);
            this.tabPageMods.TabIndex = 0;
            this.tabPageMods.Text = "Mods";
            this.tabPageMods.UseVisualStyleBackColor = true;
            // 
            // configureModButton
            // 
            this.configureModButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.configureModButton.Enabled = false;
            this.configureModButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.configureModButton.Location = new System.Drawing.Point(87, 287);
            this.configureModButton.Name = "configureModButton";
            this.configureModButton.Size = new System.Drawing.Size(75, 23);
            this.configureModButton.TabIndex = 2;
            this.configureModButton.Text = "Configure...";
            this.configureModButton.UseVisualStyleBackColor = true;
            this.configureModButton.Click += new System.EventHandler(this.configureModButton_Click);
            // 
            // modBottomButton
            // 
            this.modBottomButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modBottomButton.AutoSize = true;
            this.modBottomButton.Enabled = false;
            this.modBottomButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modBottomButton.Location = new System.Drawing.Point(369, 114);
            this.modBottomButton.Name = "modBottomButton";
            this.modBottomButton.Size = new System.Drawing.Size(31, 29);
            this.modBottomButton.TabIndex = 6;
            this.modBottomButton.Text = "⤓";
            this.modBottomButton.UseVisualStyleBackColor = true;
            this.modBottomButton.Click += new System.EventHandler(this.modBottomButton_Click);
            // 
            // modTopButton
            // 
            this.modTopButton.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
            this.modTopButton.AutoSize = true;
            this.modTopButton.Enabled = false;
            this.modTopButton.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.modTopButton.Location = new System.Drawing.Point(369, 6);
            this.modTopButton.Name = "modTopButton";
            this.modTopButton.Size = new System.Drawing.Size(31, 29);
            this.modTopButton.TabIndex = 3;
            this.modTopButton.Text = "⤒";
            this.modTopButton.UseVisualStyleBackColor = true;
            this.modTopButton.Click += new System.EventHandler(this.modTopButton_Click);
            // 
            // buttonNewMod
            // 
            this.buttonNewMod.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
            this.buttonNewMod.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.buttonNewMod.Location = new System.Drawing.Point(292, 287);
            this.buttonNewMod.Name = "buttonNewMod";
            this.buttonNewMod.Size = new System.Drawing.Size(75, 23);
            this.buttonNewMod.TabIndex = 7;
            this.buttonNewMod.Text = "&New Mod";
            this.buttonNewMod.UseVisualStyleBackColor = true;
            this.buttonNewMod.Click += new System.EventHandler(this.buttonNewMod_Click);
            // 
            // buttonRefreshModList
            // 
            this.buttonRefreshModList.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
            this.buttonRefreshModList.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.buttonRefreshModList.Location = new System.Drawing.Point(6, 287);
            this.buttonRefreshModList.Name = "buttonRefreshModList";
            this.buttonRefreshModList.Size = new System.Drawing.Size(75, 23);
            this.buttonRefreshModList.TabIndex = 1;
            this.buttonRefreshModList.Text = "&Refresh";
            this.buttonRefreshModList.UseVisualStyleBackColor = true;
            this.buttonRefreshModList.Click += new System.EventHandler(this.buttonRefreshModList_Click);
            // 
            // tabPageCodes
            // 
            this.tabPageCodes.Controls.Add(this.codesCheckedListBox);
            this.tabPageCodes.Location = new System.Drawing.Point(4, 22);
            this.tabPageCodes.Name = "tabPageCodes";
            this.tabPageCodes.Size = new System.Drawing.Size(402, 379);
            this.tabPageCodes.TabIndex = 1;
            this.tabPageCodes.Text = "Codes";
            this.tabPageCodes.UseVisualStyleBackColor = true;
            // 
            // codesCheckedListBox
            // 
            this.codesCheckedListBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.codesCheckedListBox.FormattingEnabled = true;
            this.codesCheckedListBox.Location = new System.Drawing.Point(0, 0);
            this.codesCheckedListBox.Name = "codesCheckedListBox";
            this.codesCheckedListBox.Size = new System.Drawing.Size(402, 379);
            this.codesCheckedListBox.TabIndex = 0;
            this.codesCheckedListBox.ItemCheck += new System.Windows.Forms.ItemCheckEventHandler(this.codesCheckedListBox_ItemCheck);
            // 
            // tabPageGraphics
            // 
            this.tabPageGraphics.Controls.Add(this.groupBox_Visuals);
            this.tabPageGraphics.Controls.Add(this.groupBox_WindowMode);
            this.tabPageGraphics.Controls.Add(this.groupBox_Display);
            this.tabPageGraphics.Location = new System.Drawing.Point(4, 22);
            this.tabPageGraphics.Name = "tabPageGraphics";
            this.tabPageGraphics.Padding = new System.Windows.Forms.Padding(3, 3, 3, 3);
            this.tabPageGraphics.Size = new System.Drawing.Size(402, 379);
            this.tabPageGraphics.TabIndex = 2;
            this.tabPageGraphics.Text = "Graphics";
            this.tabPageGraphics.UseVisualStyleBackColor = true;
            // 
            // groupBox_Visuals
            // 
            this.groupBox_Visuals.Controls.Add(this.checkFrameLimiter);
            this.groupBox_Visuals.Location = new System.Drawing.Point(6, 195);
            this.groupBox_Visuals.Margin = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox_Visuals.Name = "groupBox_Visuals";
            this.groupBox_Visuals.Padding = new System.Windows.Forms.Padding(2, 2, 2, 2);
            this.groupBox_Visuals.Size = new System.Drawing.Size(390, 45);
            this.groupBox_Visuals.TabIndex = 2;
            this.groupBox_Visuals.TabStop = false;
            this.groupBox_Visuals.Text = "Visuals";
            // 
            // checkFrameLimiter
            // 
            this.checkFrameLimiter.AutoSize = true;
            this.checkFrameLimiter.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkFrameLimiter.Location = new System.Drawing.Point(6, 20);
            this.checkFrameLimiter.Name = "checkFrameLimiter";
            this.checkFrameLimiter.Size = new System.Drawing.Size(103, 18);
            this.checkFrameLimiter.TabIndex = 6;
            this.checkFrameLimiter.Text = "Lock framerate";
            this.toolTip.SetToolTip(this.checkFrameLimiter, "Prevents the game from going over 60 FPS, useful for high refresh rate monitors.");
            this.checkFrameLimiter.UseVisualStyleBackColor = true;
            // 
            // groupBox_WindowMode
            // 
            this.groupBox_WindowMode.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_WindowMode.Controls.Add(this.aspectRatioCheckBox);
            this.groupBox_WindowMode.Controls.Add(this.checkWindowResize);
            this.groupBox_WindowMode.Controls.Add(this.customWindowSizeCheckBox);
            this.groupBox_WindowMode.Controls.Add(this.windowHeight);
            this.groupBox_WindowMode.Controls.Add(this.windowWidth);
            this.groupBox_WindowMode.Controls.Add(label2);
            this.groupBox_WindowMode.Location = new System.Drawing.Point(6, 113);
            this.groupBox_WindowMode.Name = "groupBox_WindowMode";
            this.groupBox_WindowMode.Size = new System.Drawing.Size(390, 75);
            this.groupBox_WindowMode.TabIndex = 1;
            this.groupBox_WindowMode.TabStop = false;
            this.groupBox_WindowMode.Text = "Window mode";
            // 
            // aspectRatioCheckBox
            // 
            this.aspectRatioCheckBox.AutoSize = true;
            this.aspectRatioCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.aspectRatioCheckBox.Location = new System.Drawing.Point(151, 48);
            this.aspectRatioCheckBox.Name = "aspectRatioCheckBox";
            this.aspectRatioCheckBox.Size = new System.Drawing.Size(136, 18);
            this.aspectRatioCheckBox.TabIndex = 5;
            this.aspectRatioCheckBox.Text = "Maintain Aspect Ratio";
            this.toolTip.SetToolTip(this.aspectRatioCheckBox, "Maintain the aspect ratio of the select resolution, will draw a stylized border a" +
        "round the screen.");
            this.aspectRatioCheckBox.UseVisualStyleBackColor = true;
            // 
            // checkWindowResize
            // 
            this.checkWindowResize.AutoSize = true;
            this.checkWindowResize.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkWindowResize.Location = new System.Drawing.Point(6, 48);
            this.checkWindowResize.Name = "checkWindowResize";
            this.checkWindowResize.Size = new System.Drawing.Size(120, 18);
            this.checkWindowResize.TabIndex = 4;
            this.checkWindowResize.Text = "Resizable Window";
            this.toolTip.SetToolTip(this.checkWindowResize, "Allows the window to be resized and dynamically adjusts resolution to match.");
            this.checkWindowResize.UseVisualStyleBackColor = true;
            // 
            // customWindowSizeCheckBox
            // 
            this.customWindowSizeCheckBox.AutoSize = true;
            this.customWindowSizeCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.customWindowSizeCheckBox.Location = new System.Drawing.Point(6, 20);
            this.customWindowSizeCheckBox.Name = "customWindowSizeCheckBox";
            this.customWindowSizeCheckBox.Size = new System.Drawing.Size(135, 18);
            this.customWindowSizeCheckBox.TabIndex = 0;
            this.customWindowSizeCheckBox.Text = "Custom Window Size:";
            this.toolTip.SetToolTip(this.customWindowSizeCheckBox, "Allows you to specify a fixed size for the window in windowed mode.");
            this.customWindowSizeCheckBox.UseVisualStyleBackColor = true;
            this.customWindowSizeCheckBox.CheckedChanged += new System.EventHandler(this.customWindowSizeCheckBox_CheckedChanged);
            // 
            // windowHeight
            // 
            this.windowHeight.Location = new System.Drawing.Point(237, 19);
            this.windowHeight.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.windowHeight.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.windowHeight.Name = "windowHeight";
            this.windowHeight.Size = new System.Drawing.Size(62, 20);
            this.windowHeight.TabIndex = 3;
            this.windowHeight.Value = new decimal(new int[] {
            480,
            0,
            0,
            0});
            // 
            // windowWidth
            // 
            this.windowWidth.Location = new System.Drawing.Point(151, 19);
            this.windowWidth.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.windowWidth.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.windowWidth.Name = "windowWidth";
            this.windowWidth.Size = new System.Drawing.Size(62, 20);
            this.windowWidth.TabIndex = 1;
            this.windowWidth.Value = new decimal(new int[] {
            640,
            0,
            0,
            0});
            // 
            // groupBox_Display
            // 
            this.groupBox_Display.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox_Display.Controls.Add(this.radioBorderlessWindowMode);
            this.groupBox_Display.Controls.Add(label3);
            this.groupBox_Display.Controls.Add(this.screenNumComboBox);
            this.groupBox_Display.Controls.Add(this.comboResolutionPreset);
            this.groupBox_Display.Controls.Add(this.radioWindowMode);
            this.groupBox_Display.Controls.Add(label5);
            this.groupBox_Display.Controls.Add(this.radioFullscreen);
            this.groupBox_Display.Controls.Add(this.horizontalResolution);
            this.groupBox_Display.Controls.Add(label1);
            this.groupBox_Display.Controls.Add(this.verticalResolution);
            this.groupBox_Display.Location = new System.Drawing.Point(6, 6);
            this.groupBox_Display.Name = "groupBox_Display";
            this.groupBox_Display.Size = new System.Drawing.Size(390, 101);
            this.groupBox_Display.TabIndex = 0;
            this.groupBox_Display.TabStop = false;
            this.groupBox_Display.Text = "Display";
            // 
            // radioBorderlessWindowMode
            // 
            this.radioBorderlessWindowMode.AutoSize = true;
            this.radioBorderlessWindowMode.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioBorderlessWindowMode.Location = new System.Drawing.Point(95, 73);
            this.radioBorderlessWindowMode.Name = "radioBorderlessWindowMode";
            this.radioBorderlessWindowMode.Size = new System.Drawing.Size(130, 18);
            this.radioBorderlessWindowMode.TabIndex = 8;
            this.radioBorderlessWindowMode.Text = "Windowed fullscreen";
            this.toolTip.SetToolTip(this.radioBorderlessWindowMode, "Creates a borderless window adjusted to the size of the screen.");
            this.radioBorderlessWindowMode.UseVisualStyleBackColor = true;
            // 
            // screenNumComboBox
            // 
            this.screenNumComboBox.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.screenNumComboBox.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.screenNumComboBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.screenNumComboBox.FormattingEnabled = true;
            this.screenNumComboBox.Items.AddRange(new object[] {
            "All Screens"});
            this.screenNumComboBox.Location = new System.Drawing.Point(56, 19);
            this.screenNumComboBox.Name = "screenNumComboBox";
            this.screenNumComboBox.Size = new System.Drawing.Size(315, 21);
            this.screenNumComboBox.TabIndex = 1;
            this.screenNumComboBox.SelectedIndexChanged += new System.EventHandler(this.screenNumComboBox_SelectedIndexChanged);
            // 
            // comboResolutionPreset
            // 
            this.comboResolutionPreset.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboResolutionPreset.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboResolutionPreset.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboResolutionPreset.FormattingEnabled = true;
            this.comboResolutionPreset.Items.AddRange(new object[] {
            "640x480",
            "800x600",
            "1024x768",
            "1152x864",
            "1280x960",
            "1280x1024",
            "Native",
            "1/2x Native",
            "2x Native",
            "720p",
            "1080p",
            "4K"});
            this.comboResolutionPreset.Location = new System.Drawing.Point(226, 46);
            this.comboResolutionPreset.Name = "comboResolutionPreset";
            this.comboResolutionPreset.Size = new System.Drawing.Size(145, 21);
            this.comboResolutionPreset.TabIndex = 6;
            this.comboResolutionPreset.SelectedIndexChanged += new System.EventHandler(this.comboResolutionPreset_SelectedIndexChanged);
            // 
            // radioWindowMode
            // 
            this.radioWindowMode.AutoSize = true;
            this.radioWindowMode.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioWindowMode.Location = new System.Drawing.Point(233, 73);
            this.radioWindowMode.Name = "radioWindowMode";
            this.radioWindowMode.Size = new System.Drawing.Size(82, 18);
            this.radioWindowMode.TabIndex = 9;
            this.radioWindowMode.Text = "Windowed";
            this.toolTip.SetToolTip(this.radioWindowMode, "Classic windowed mode.");
            this.radioWindowMode.UseVisualStyleBackColor = true;
            // 
            // radioFullscreen
            // 
            this.radioFullscreen.AutoSize = true;
            this.radioFullscreen.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.radioFullscreen.Location = new System.Drawing.Point(9, 73);
            this.radioFullscreen.Name = "radioFullscreen";
            this.radioFullscreen.Size = new System.Drawing.Size(79, 18);
            this.radioFullscreen.TabIndex = 7;
            this.radioFullscreen.Text = "Fullscreen";
            this.toolTip.SetToolTip(this.radioFullscreen, "Native fullscreen mode.");
            this.radioFullscreen.UseVisualStyleBackColor = true;
            this.radioFullscreen.CheckedChanged += new System.EventHandler(this.radioFullscreen_CheckedChanged);
            // 
            // horizontalResolution
            // 
            this.horizontalResolution.Location = new System.Drawing.Point(72, 47);
            this.horizontalResolution.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.horizontalResolution.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.horizontalResolution.Name = "horizontalResolution";
            this.horizontalResolution.Size = new System.Drawing.Size(62, 20);
            this.horizontalResolution.TabIndex = 3;
            this.horizontalResolution.Value = new decimal(new int[] {
            640,
            0,
            0,
            0});
            this.horizontalResolution.ValueChanged += new System.EventHandler(this.horizontalResolution_ValueChanged);
            // 
            // verticalResolution
            // 
            this.verticalResolution.Location = new System.Drawing.Point(157, 47);
            this.verticalResolution.Maximum = new decimal(new int[] {
            2147483647,
            0,
            0,
            0});
            this.verticalResolution.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.verticalResolution.Name = "verticalResolution";
            this.verticalResolution.Size = new System.Drawing.Size(62, 20);
            this.verticalResolution.TabIndex = 5;
            this.verticalResolution.Value = new decimal(new int[] {
            480,
            0,
            0,
            0});
            this.verticalResolution.ValueChanged += new System.EventHandler(this.verticalResolution_ValueChanged);
            // 
            // tabPageOptions
            // 
            this.tabPageOptions.Controls.Add(this.groupBox3);
            this.tabPageOptions.Controls.Add(this.installURLHandlerButton);
            this.tabPageOptions.Controls.Add(this.groupBox4);
            this.tabPageOptions.Controls.Add(this.groupBox1);
            this.tabPageOptions.Location = new System.Drawing.Point(4, 22);
            this.tabPageOptions.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabPageOptions.Name = "tabPageOptions";
            this.tabPageOptions.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabPageOptions.Size = new System.Drawing.Size(402, 379);
            this.tabPageOptions.TabIndex = 3;
            this.tabPageOptions.Text = "Options";
            this.tabPageOptions.UseVisualStyleBackColor = true;
            // 
            // groupBox3
            // 
            this.groupBox3.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox3.Controls.Add(this.buttonSaveProfile);
            this.groupBox3.Controls.Add(this.buttonLoadProfile);
            this.groupBox3.Controls.Add(this.profileNameBox);
            this.groupBox3.Location = new System.Drawing.Point(6, 196);
            this.groupBox3.Name = "groupBox3";
            this.groupBox3.Size = new System.Drawing.Size(390, 58);
            this.groupBox3.TabIndex = 2;
            this.groupBox3.TabStop = false;
            this.groupBox3.Text = "Profiles";
            // 
            // buttonSaveProfile
            // 
            this.buttonSaveProfile.Enabled = false;
            this.buttonSaveProfile.Location = new System.Drawing.Point(266, 19);
            this.buttonSaveProfile.Name = "buttonSaveProfile";
            this.buttonSaveProfile.Size = new System.Drawing.Size(75, 23);
            this.buttonSaveProfile.TabIndex = 2;
            this.buttonSaveProfile.Text = "Save";
            this.buttonSaveProfile.UseVisualStyleBackColor = true;
            this.buttonSaveProfile.Click += new System.EventHandler(this.buttonSaveProfile_Click);
            // 
            // buttonLoadProfile
            // 
            this.buttonLoadProfile.Enabled = false;
            this.buttonLoadProfile.Location = new System.Drawing.Point(185, 19);
            this.buttonLoadProfile.Name = "buttonLoadProfile";
            this.buttonLoadProfile.Size = new System.Drawing.Size(75, 23);
            this.buttonLoadProfile.TabIndex = 1;
            this.buttonLoadProfile.Text = "Load";
            this.buttonLoadProfile.UseVisualStyleBackColor = true;
            this.buttonLoadProfile.Click += new System.EventHandler(this.buttonLoadProfile_Click);
            // 
            // profileNameBox
            // 
            this.profileNameBox.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Suggest;
            this.profileNameBox.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.ListItems;
            this.profileNameBox.FormattingEnabled = true;
            this.profileNameBox.Location = new System.Drawing.Point(6, 21);
            this.profileNameBox.Name = "profileNameBox";
            this.profileNameBox.Size = new System.Drawing.Size(169, 21);
            this.profileNameBox.TabIndex = 0;
            this.profileNameBox.TextChanged += new System.EventHandler(this.profileNameBox_TextChanged);
            // 
            // installURLHandlerButton
            // 
            this.installURLHandlerButton.AutoSize = true;
            this.installURLHandlerButton.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.installURLHandlerButton.Location = new System.Drawing.Point(6, 260);
            this.installURLHandlerButton.Name = "installURLHandlerButton";
            this.installURLHandlerButton.Size = new System.Drawing.Size(115, 23);
            this.installURLHandlerButton.TabIndex = 3;
            this.installURLHandlerButton.Text = "Enable 1-Click Install";
            this.installURLHandlerButton.UseVisualStyleBackColor = true;
            this.installURLHandlerButton.Click += new System.EventHandler(this.installURLHandlerButton_Click);
            // 
            // groupBox4
            // 
            this.groupBox4.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox4.Controls.Add(this.buttonCheckForUpdates);
            this.groupBox4.Controls.Add(this.label4);
            this.groupBox4.Controls.Add(this.checkUpdateModsStartup);
            this.groupBox4.Controls.Add(this.numericUpdateFrequency);
            this.groupBox4.Controls.Add(this.comboUpdateFrequency);
            this.groupBox4.Controls.Add(this.checkUpdateStartup);
            this.groupBox4.Location = new System.Drawing.Point(6, 100);
            this.groupBox4.Name = "groupBox4";
            this.groupBox4.Size = new System.Drawing.Size(390, 90);
            this.groupBox4.TabIndex = 1;
            this.groupBox4.TabStop = false;
            this.groupBox4.Text = "Updates";
            // 
            // buttonCheckForUpdates
            // 
            this.buttonCheckForUpdates.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.buttonCheckForUpdates.Location = new System.Drawing.Point(259, 55);
            this.buttonCheckForUpdates.Name = "buttonCheckForUpdates";
            this.buttonCheckForUpdates.Size = new System.Drawing.Size(75, 23);
            this.buttonCheckForUpdates.TabIndex = 5;
            this.buttonCheckForUpdates.Text = "Check Now";
            this.buttonCheckForUpdates.UseVisualStyleBackColor = true;
            this.buttonCheckForUpdates.Click += new System.EventHandler(this.buttonCheckForUpdates_Click);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(6, 40);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(60, 13);
            this.label4.TabIndex = 2;
            this.label4.Text = "Frequency:";
            // 
            // checkUpdateModsStartup
            // 
            this.checkUpdateModsStartup.AutoSize = true;
            this.checkUpdateModsStartup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkUpdateModsStartup.Location = new System.Drawing.Point(133, 19);
            this.checkUpdateModsStartup.Name = "checkUpdateModsStartup";
            this.checkUpdateModsStartup.Size = new System.Drawing.Size(141, 18);
            this.checkUpdateModsStartup.TabIndex = 1;
            this.checkUpdateModsStartup.Text = "Check mods on startup";
            this.checkUpdateModsStartup.UseVisualStyleBackColor = true;
            // 
            // numericUpdateFrequency
            // 
            this.numericUpdateFrequency.Location = new System.Drawing.Point(133, 57);
            this.numericUpdateFrequency.Margin = new System.Windows.Forms.Padding(6, 8, 6, 8);
            this.numericUpdateFrequency.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.numericUpdateFrequency.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpdateFrequency.Name = "numericUpdateFrequency";
            this.numericUpdateFrequency.Size = new System.Drawing.Size(120, 20);
            this.numericUpdateFrequency.TabIndex = 4;
            this.numericUpdateFrequency.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // comboUpdateFrequency
            // 
            this.comboUpdateFrequency.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboUpdateFrequency.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboUpdateFrequency.FormattingEnabled = true;
            this.comboUpdateFrequency.Items.AddRange(new object[] {
            "Always",
            "Hours",
            "Days",
            "Weeks"});
            this.comboUpdateFrequency.Location = new System.Drawing.Point(6, 56);
            this.comboUpdateFrequency.Name = "comboUpdateFrequency";
            this.comboUpdateFrequency.Size = new System.Drawing.Size(121, 21);
            this.comboUpdateFrequency.TabIndex = 3;
            this.comboUpdateFrequency.SelectedIndexChanged += new System.EventHandler(this.comboUpdateFrequency_SelectedIndexChanged);
            // 
            // checkUpdateStartup
            // 
            this.checkUpdateStartup.AutoSize = true;
            this.checkUpdateStartup.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkUpdateStartup.Location = new System.Drawing.Point(6, 19);
            this.checkUpdateStartup.Name = "checkUpdateStartup";
            this.checkUpdateStartup.Size = new System.Drawing.Size(113, 18);
            this.checkUpdateStartup.TabIndex = 0;
            this.checkUpdateStartup.Text = "Check on startup";
            this.checkUpdateStartup.UseVisualStyleBackColor = true;
            // 
            // groupBox1
            // 
            this.groupBox1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox1.Controls.Add(this.disableExitPromptCheckBox);
            this.groupBox1.Controls.Add(this.comboTextLanguage);
            this.groupBox1.Controls.Add(this.comboVoiceLanguage);
            this.groupBox1.Controls.Add(this.label9);
            this.groupBox1.Controls.Add(this.label8);
            this.groupBox1.Controls.Add(this.skipIntrocheckBox);
            this.groupBox1.Controls.Add(this.pauseWhenInactiveCheckBox);
            this.groupBox1.Location = new System.Drawing.Point(6, 6);
            this.groupBox1.Name = "groupBox1";
            this.groupBox1.Size = new System.Drawing.Size(390, 88);
            this.groupBox1.TabIndex = 0;
            this.groupBox1.TabStop = false;
            this.groupBox1.Text = "Misc.";
            // 
            // disableExitPromptCheckBox
            // 
            this.disableExitPromptCheckBox.AutoSize = true;
            this.disableExitPromptCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.disableExitPromptCheckBox.Location = new System.Drawing.Point(141, 19);
            this.disableExitPromptCheckBox.Name = "disableExitPromptCheckBox";
            this.disableExitPromptCheckBox.Size = new System.Drawing.Size(121, 18);
            this.disableExitPromptCheckBox.TabIndex = 7;
            this.disableExitPromptCheckBox.Text = "Disable exit prompt";
            this.toolTip.SetToolTip(this.disableExitPromptCheckBox, "Disables the prompt message when closing the window.");
            this.disableExitPromptCheckBox.UseVisualStyleBackColor = true;
            // 
            // comboTextLanguage
            // 
            this.comboTextLanguage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboTextLanguage.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboTextLanguage.FormattingEnabled = true;
            this.comboTextLanguage.Items.AddRange(new object[] {
            "English",
            "German",
            "Spanish",
            "French",
            "Italian",
            "Japanese"});
            this.comboTextLanguage.Location = new System.Drawing.Point(125, 57);
            this.comboTextLanguage.Name = "comboTextLanguage";
            this.comboTextLanguage.Size = new System.Drawing.Size(98, 21);
            this.comboTextLanguage.TabIndex = 6;
            // 
            // comboVoiceLanguage
            // 
            this.comboVoiceLanguage.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboVoiceLanguage.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.comboVoiceLanguage.FormattingEnabled = true;
            this.comboVoiceLanguage.Items.AddRange(new object[] {
            "Japanese",
            "English"});
            this.comboVoiceLanguage.Location = new System.Drawing.Point(11, 57);
            this.comboVoiceLanguage.Name = "comboVoiceLanguage";
            this.comboVoiceLanguage.Size = new System.Drawing.Size(98, 21);
            this.comboVoiceLanguage.TabIndex = 5;
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(123, 40);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(78, 13);
            this.label9.TabIndex = 4;
            this.label9.Text = "Text language:";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(9, 40);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(84, 13);
            this.label8.TabIndex = 3;
            this.label8.Text = "Voice language:";
            // 
            // skipIntrocheckBox
            // 
            this.skipIntrocheckBox.AutoSize = true;
            this.skipIntrocheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.skipIntrocheckBox.Location = new System.Drawing.Point(262, 19);
            this.skipIntrocheckBox.Name = "skipIntrocheckBox";
            this.skipIntrocheckBox.Size = new System.Drawing.Size(77, 18);
            this.skipIntrocheckBox.TabIndex = 1;
            this.skipIntrocheckBox.Text = "Skip Intro";
            this.skipIntrocheckBox.UseVisualStyleBackColor = true;
            // 
            // pauseWhenInactiveCheckBox
            // 
            this.pauseWhenInactiveCheckBox.AutoSize = true;
            this.pauseWhenInactiveCheckBox.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.pauseWhenInactiveCheckBox.Location = new System.Drawing.Point(6, 19);
            this.pauseWhenInactiveCheckBox.Name = "pauseWhenInactiveCheckBox";
            this.pauseWhenInactiveCheckBox.Size = new System.Drawing.Size(135, 18);
            this.pauseWhenInactiveCheckBox.TabIndex = 2;
            this.pauseWhenInactiveCheckBox.Text = "Pause When Inactive";
            this.pauseWhenInactiveCheckBox.UseVisualStyleBackColor = true;
            // 
            // tabPageDebug
            // 
            this.tabPageDebug.Controls.Add(groupBox2);
            this.tabPageDebug.Controls.Add(this.groupBox12);
            this.tabPageDebug.Location = new System.Drawing.Point(4, 22);
            this.tabPageDebug.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabPageDebug.Name = "tabPageDebug";
            this.tabPageDebug.Padding = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.tabPageDebug.Size = new System.Drawing.Size(402, 379);
            this.tabPageDebug.TabIndex = 4;
            this.tabPageDebug.Text = "Debug";
            this.tabPageDebug.UseVisualStyleBackColor = true;
            // 
            // groupBox12
            // 
            this.groupBox12.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.groupBox12.Controls.Add(this.labelTestSpawnX);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnPlayer2);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnPlayer2);
            this.groupBox12.Controls.Add(this.checkBoxMission);
            this.groupBox12.Controls.Add(this.comboBoxMission);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnPosition);
            this.groupBox12.Controls.Add(this.buttonTestSpawnPlay);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnEvent);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnEvent);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnCharacter);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnLevel);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnCharacter);
            this.groupBox12.Controls.Add(this.labelTestSpawnY);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnSaveID);
            this.groupBox12.Controls.Add(this.checkBoxTestSpawnSave);
            this.groupBox12.Controls.Add(this.labelTestSpawnZ);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnZ);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnY);
            this.groupBox12.Controls.Add(this.comboBoxTestSpawnLevel);
            this.groupBox12.Controls.Add(this.numericUpDownTestSpawnX);
            this.groupBox12.Location = new System.Drawing.Point(6, 55);
            this.groupBox12.Name = "groupBox12";
            this.groupBox12.Size = new System.Drawing.Size(390, 225);
            this.groupBox12.TabIndex = 1;
            this.groupBox12.TabStop = false;
            this.groupBox12.Text = "Test Spawn";
            // 
            // labelTestSpawnX
            // 
            this.labelTestSpawnX.AutoSize = true;
            this.labelTestSpawnX.Enabled = false;
            this.labelTestSpawnX.Location = new System.Drawing.Point(72, 98);
            this.labelTestSpawnX.Name = "labelTestSpawnX";
            this.labelTestSpawnX.Size = new System.Drawing.Size(17, 13);
            this.labelTestSpawnX.TabIndex = 7;
            this.labelTestSpawnX.Text = "X:";
            // 
            // checkBoxTestSpawnPlayer2
            // 
            this.checkBoxTestSpawnPlayer2.AutoSize = true;
            this.checkBoxTestSpawnPlayer2.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxTestSpawnPlayer2.Location = new System.Drawing.Point(6, 71);
            this.checkBoxTestSpawnPlayer2.Name = "checkBoxTestSpawnPlayer2";
            this.checkBoxTestSpawnPlayer2.Size = new System.Drawing.Size(73, 18);
            this.checkBoxTestSpawnPlayer2.TabIndex = 4;
            this.checkBoxTestSpawnPlayer2.Text = "Player 2:";
            this.checkBoxTestSpawnPlayer2.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnPlayer2.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnPlayer2_CheckedChanged);
            // 
            // comboBoxTestSpawnPlayer2
            // 
            this.comboBoxTestSpawnPlayer2.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnPlayer2.Enabled = false;
            this.comboBoxTestSpawnPlayer2.FormattingEnabled = true;
            this.comboBoxTestSpawnPlayer2.Items.AddRange(new object[] {
            "Sonic",
            "Shadow",
            "Tails",
            "Eggman",
            "Knuckles",
            "Rouge",
            "Mech Tails",
            "Mech Eggman"});
            this.comboBoxTestSpawnPlayer2.Location = new System.Drawing.Point(93, 69);
            this.comboBoxTestSpawnPlayer2.Name = "comboBoxTestSpawnPlayer2";
            this.comboBoxTestSpawnPlayer2.Size = new System.Drawing.Size(149, 21);
            this.comboBoxTestSpawnPlayer2.TabIndex = 5;
            // 
            // checkBoxMission
            // 
            this.checkBoxMission.AutoSize = true;
            this.checkBoxMission.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxMission.Location = new System.Drawing.Point(6, 121);
            this.checkBoxMission.Name = "checkBoxMission";
            this.checkBoxMission.Size = new System.Drawing.Size(70, 18);
            this.checkBoxMission.TabIndex = 13;
            this.checkBoxMission.Text = "Mission:";
            this.checkBoxMission.UseVisualStyleBackColor = true;
            this.checkBoxMission.CheckedChanged += new System.EventHandler(this.checkBoxMission_CheckedChanged);
            // 
            // comboBoxMission
            // 
            this.comboBoxMission.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxMission.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxMission.Enabled = false;
            this.comboBoxMission.FormattingEnabled = true;
            this.comboBoxMission.Items.AddRange(new object[] {
            "Mission 1",
            "Mission 2: 100 Rings",
            "Mission 3: Lost Chao",
            "Mission 4: Timer",
            "Mission 5: Hard Mode"});
            this.comboBoxMission.Location = new System.Drawing.Point(93, 119);
            this.comboBoxMission.Name = "comboBoxMission";
            this.comboBoxMission.Size = new System.Drawing.Size(291, 21);
            this.comboBoxMission.TabIndex = 14;
            // 
            // checkBoxTestSpawnPosition
            // 
            this.checkBoxTestSpawnPosition.AutoSize = true;
            this.checkBoxTestSpawnPosition.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxTestSpawnPosition.Location = new System.Drawing.Point(6, 96);
            this.checkBoxTestSpawnPosition.Name = "checkBoxTestSpawnPosition";
            this.checkBoxTestSpawnPosition.Size = new System.Drawing.Size(72, 18);
            this.checkBoxTestSpawnPosition.TabIndex = 6;
            this.checkBoxTestSpawnPosition.Text = "Position:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnPosition, "Select Player 2 character.");
            this.checkBoxTestSpawnPosition.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnPosition.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnPosition_CheckedChanged);
            // 
            // buttonTestSpawnPlay
            // 
            this.buttonTestSpawnPlay.Location = new System.Drawing.Point(5, 197);
            this.buttonTestSpawnPlay.Name = "buttonTestSpawnPlay";
            this.buttonTestSpawnPlay.Size = new System.Drawing.Size(76, 22);
            this.buttonTestSpawnPlay.TabIndex = 19;
            this.buttonTestSpawnPlay.Text = "Play";
            this.toolTip.SetToolTip(this.buttonTestSpawnPlay, "Run the game with the above Test Spawn settings.");
            this.buttonTestSpawnPlay.UseVisualStyleBackColor = true;
            this.buttonTestSpawnPlay.Click += new System.EventHandler(this.buttonTestSpawnPlay_Click);
            // 
            // comboBoxTestSpawnEvent
            // 
            this.comboBoxTestSpawnEvent.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.comboBoxTestSpawnEvent.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnEvent.Enabled = false;
            this.comboBoxTestSpawnEvent.FormattingEnabled = true;
            this.comboBoxTestSpawnEvent.Location = new System.Drawing.Point(93, 144);
            this.comboBoxTestSpawnEvent.Name = "comboBoxTestSpawnEvent";
            this.comboBoxTestSpawnEvent.Size = new System.Drawing.Size(291, 21);
            this.comboBoxTestSpawnEvent.TabIndex = 16;
            this.toolTip.SetToolTip(this.comboBoxTestSpawnEvent, "Start the game on a specific cutscene.");
            // 
            // checkBoxTestSpawnEvent
            // 
            this.checkBoxTestSpawnEvent.AutoSize = true;
            this.checkBoxTestSpawnEvent.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxTestSpawnEvent.Location = new System.Drawing.Point(6, 146);
            this.checkBoxTestSpawnEvent.Name = "checkBoxTestSpawnEvent";
            this.checkBoxTestSpawnEvent.Size = new System.Drawing.Size(63, 18);
            this.checkBoxTestSpawnEvent.TabIndex = 15;
            this.checkBoxTestSpawnEvent.Text = "Event:";
            this.checkBoxTestSpawnEvent.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnEvent.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnEvent_CheckedChanged);
            // 
            // checkBoxTestSpawnCharacter
            // 
            this.checkBoxTestSpawnCharacter.AutoSize = true;
            this.checkBoxTestSpawnCharacter.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxTestSpawnCharacter.Location = new System.Drawing.Point(6, 46);
            this.checkBoxTestSpawnCharacter.Name = "checkBoxTestSpawnCharacter";
            this.checkBoxTestSpawnCharacter.Size = new System.Drawing.Size(81, 18);
            this.checkBoxTestSpawnCharacter.TabIndex = 2;
            this.checkBoxTestSpawnCharacter.Text = "Character:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnCharacter, "Start the game with a specific character.");
            this.checkBoxTestSpawnCharacter.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnCharacter.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnCharacter_CheckedChanged);
            // 
            // checkBoxTestSpawnLevel
            // 
            this.checkBoxTestSpawnLevel.AutoSize = true;
            this.checkBoxTestSpawnLevel.FlatStyle = System.Windows.Forms.FlatStyle.System;
            this.checkBoxTestSpawnLevel.Location = new System.Drawing.Point(6, 21);
            this.checkBoxTestSpawnLevel.Name = "checkBoxTestSpawnLevel";
            this.checkBoxTestSpawnLevel.Size = new System.Drawing.Size(61, 18);
            this.checkBoxTestSpawnLevel.TabIndex = 0;
            this.checkBoxTestSpawnLevel.Text = "Level:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnLevel, "Start the game on a specific level.");
            this.checkBoxTestSpawnLevel.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnLevel.CheckedChanged += new System.EventHandler(this.checkBoxTestSpawnLevel_CheckedChanged);
            // 
            // comboBoxTestSpawnCharacter
            // 
            this.comboBoxTestSpawnCharacter.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnCharacter.Enabled = false;
            this.comboBoxTestSpawnCharacter.FormattingEnabled = true;
            this.comboBoxTestSpawnCharacter.Items.AddRange(new object[] {
            "Sonic",
            "Shadow",
            "Tails",
            "Eggman",
            "Knuckles",
            "Rouge",
            "Mech Tails",
            "Mech Eggman"});
            this.comboBoxTestSpawnCharacter.Location = new System.Drawing.Point(93, 44);
            this.comboBoxTestSpawnCharacter.Name = "comboBoxTestSpawnCharacter";
            this.comboBoxTestSpawnCharacter.Size = new System.Drawing.Size(149, 21);
            this.comboBoxTestSpawnCharacter.TabIndex = 3;
            this.toolTip.SetToolTip(this.comboBoxTestSpawnCharacter, "Start the game with a specific character.");
            // 
            // labelTestSpawnY
            // 
            this.labelTestSpawnY.AutoSize = true;
            this.labelTestSpawnY.Enabled = false;
            this.labelTestSpawnY.Location = new System.Drawing.Point(172, 98);
            this.labelTestSpawnY.Name = "labelTestSpawnY";
            this.labelTestSpawnY.Size = new System.Drawing.Size(17, 13);
            this.labelTestSpawnY.TabIndex = 9;
            this.labelTestSpawnY.Text = "Y:";
            // 
            // numericUpDownTestSpawnSaveID
            // 
            this.numericUpDownTestSpawnSaveID.Enabled = false;
            this.numericUpDownTestSpawnSaveID.Location = new System.Drawing.Point(127, 170);
            this.numericUpDownTestSpawnSaveID.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.numericUpDownTestSpawnSaveID.Maximum = new decimal(new int[] {
            99,
            0,
            0,
            0});
            this.numericUpDownTestSpawnSaveID.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.numericUpDownTestSpawnSaveID.Name = "numericUpDownTestSpawnSaveID";
            this.numericUpDownTestSpawnSaveID.Size = new System.Drawing.Size(76, 20);
            this.numericUpDownTestSpawnSaveID.TabIndex = 18;
            this.numericUpDownTestSpawnSaveID.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // checkBoxTestSpawnSave
            // 
            this.checkBoxTestSpawnSave.AutoSize = true;
            this.checkBoxTestSpawnSave.Location = new System.Drawing.Point(6, 171);
            this.checkBoxTestSpawnSave.Name = "checkBoxTestSpawnSave";
            this.checkBoxTestSpawnSave.Size = new System.Drawing.Size(111, 17);
            this.checkBoxTestSpawnSave.TabIndex = 17;
            this.checkBoxTestSpawnSave.Text = "Save Redirection:";
            this.toolTip.SetToolTip(this.checkBoxTestSpawnSave, "Force the game to load a specific save file.");
            this.checkBoxTestSpawnSave.UseVisualStyleBackColor = true;
            this.checkBoxTestSpawnSave.CheckStateChanged += new System.EventHandler(this.checkBoxTestSpawnSave_CheckStateChanged);
            // 
            // labelTestSpawnZ
            // 
            this.labelTestSpawnZ.AutoSize = true;
            this.labelTestSpawnZ.Enabled = false;
            this.labelTestSpawnZ.Location = new System.Drawing.Point(271, 98);
            this.labelTestSpawnZ.Name = "labelTestSpawnZ";
            this.labelTestSpawnZ.Size = new System.Drawing.Size(17, 13);
            this.labelTestSpawnZ.TabIndex = 11;
            this.labelTestSpawnZ.Text = "Z:";
            // 
            // numericUpDownTestSpawnZ
            // 
            this.numericUpDownTestSpawnZ.Enabled = false;
            this.numericUpDownTestSpawnZ.Location = new System.Drawing.Point(292, 95);
            this.numericUpDownTestSpawnZ.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
            this.numericUpDownTestSpawnZ.Minimum = new decimal(new int[] {
            999999,
            0,
            0,
            -2147483648});
            this.numericUpDownTestSpawnZ.Name = "numericUpDownTestSpawnZ";
            this.numericUpDownTestSpawnZ.Size = new System.Drawing.Size(70, 20);
            this.numericUpDownTestSpawnZ.TabIndex = 12;
            // 
            // numericUpDownTestSpawnY
            // 
            this.numericUpDownTestSpawnY.Enabled = false;
            this.numericUpDownTestSpawnY.Location = new System.Drawing.Point(192, 95);
            this.numericUpDownTestSpawnY.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
            this.numericUpDownTestSpawnY.Minimum = new decimal(new int[] {
            999999,
            0,
            0,
            -2147483648});
            this.numericUpDownTestSpawnY.Name = "numericUpDownTestSpawnY";
            this.numericUpDownTestSpawnY.Size = new System.Drawing.Size(70, 20);
            this.numericUpDownTestSpawnY.TabIndex = 10;
            // 
            // comboBoxTestSpawnLevel
            // 
            this.comboBoxTestSpawnLevel.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.comboBoxTestSpawnLevel.Enabled = false;
            this.comboBoxTestSpawnLevel.FormattingEnabled = true;
            this.comboBoxTestSpawnLevel.Location = new System.Drawing.Point(93, 19);
            this.comboBoxTestSpawnLevel.Name = "comboBoxTestSpawnLevel";
            this.comboBoxTestSpawnLevel.Size = new System.Drawing.Size(149, 21);
            this.comboBoxTestSpawnLevel.TabIndex = 1;
            // 
            // numericUpDownTestSpawnX
            // 
            this.numericUpDownTestSpawnX.Enabled = false;
            this.numericUpDownTestSpawnX.Location = new System.Drawing.Point(93, 95);
            this.numericUpDownTestSpawnX.Maximum = new decimal(new int[] {
            999999,
            0,
            0,
            0});
            this.numericUpDownTestSpawnX.Minimum = new decimal(new int[] {
            999999,
            0,
            0,
            -2147483648});
            this.numericUpDownTestSpawnX.Name = "numericUpDownTestSpawnX";
            this.numericUpDownTestSpawnX.Size = new System.Drawing.Size(70, 20);
            this.numericUpDownTestSpawnX.TabIndex = 8;
            // 
            // modContextMenu
            // 
            this.modContextMenu.ImageScalingSize = new System.Drawing.Size(24, 24);
            this.modContextMenu.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.openFolderToolStripMenuItem,
            this.toolStripMenuItem1,
            this.checkForUpdatesToolStripMenuItem,
            this.verifyToolStripMenuItem,
            this.forceUpdateToolStripMenuItem,
            this.uninstallToolStripMenuItem,
            this.toolStripMenuItem3,
            this.developerToolStripMenuItem});
            this.modContextMenu.Name = "modContextMenu";
            this.modContextMenu.Size = new System.Drawing.Size(171, 148);
            // 
            // openFolderToolStripMenuItem
            // 
            this.openFolderToolStripMenuItem.Name = "openFolderToolStripMenuItem";
            this.openFolderToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.openFolderToolStripMenuItem.Text = "Open folder";
            this.openFolderToolStripMenuItem.Click += new System.EventHandler(this.openFolderToolStripMenuItem_Click);
            // 
            // toolStripMenuItem1
            // 
            this.toolStripMenuItem1.Name = "toolStripMenuItem1";
            this.toolStripMenuItem1.Size = new System.Drawing.Size(167, 6);
            // 
            // checkForUpdatesToolStripMenuItem
            // 
            this.checkForUpdatesToolStripMenuItem.Name = "checkForUpdatesToolStripMenuItem";
            this.checkForUpdatesToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.checkForUpdatesToolStripMenuItem.Text = "Check for updates";
            this.checkForUpdatesToolStripMenuItem.Click += new System.EventHandler(this.checkForUpdatesToolStripMenuItem_Click);
            // 
            // verifyToolStripMenuItem
            // 
            this.verifyToolStripMenuItem.Name = "verifyToolStripMenuItem";
            this.verifyToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.verifyToolStripMenuItem.Text = "Verify integrity";
            this.verifyToolStripMenuItem.Click += new System.EventHandler(this.verifyToolStripMenuItem_Click);
            // 
            // forceUpdateToolStripMenuItem
            // 
            this.forceUpdateToolStripMenuItem.Name = "forceUpdateToolStripMenuItem";
            this.forceUpdateToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.forceUpdateToolStripMenuItem.Text = "Force update";
            this.forceUpdateToolStripMenuItem.Click += new System.EventHandler(this.forceUpdateToolStripMenuItem_Click);
            // 
            // uninstallToolStripMenuItem
            // 
            this.uninstallToolStripMenuItem.Name = "uninstallToolStripMenuItem";
            this.uninstallToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.uninstallToolStripMenuItem.Text = "Uninstall";
            this.uninstallToolStripMenuItem.Click += new System.EventHandler(this.uninstallToolStripMenuItem_Click);
            // 
            // toolStripMenuItem3
            // 
            this.toolStripMenuItem3.Name = "toolStripMenuItem3";
            this.toolStripMenuItem3.Size = new System.Drawing.Size(167, 6);
            // 
            // developerToolStripMenuItem
            // 
            this.developerToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.generateManifestToolStripMenuItem});
            this.developerToolStripMenuItem.Name = "developerToolStripMenuItem";
            this.developerToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.developerToolStripMenuItem.Text = "Developer";
            // 
            // generateManifestToolStripMenuItem
            // 
            this.generateManifestToolStripMenuItem.Name = "generateManifestToolStripMenuItem";
            this.generateManifestToolStripMenuItem.Size = new System.Drawing.Size(170, 22);
            this.generateManifestToolStripMenuItem.Text = "Generate manifest";
            this.generateManifestToolStripMenuItem.Click += new System.EventHandler(this.generateManifestToolStripMenuItem_Click);
            // 
            // MainForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(408, 461);
            this.Controls.Add(this.tabControl1);
            this.Controls.Add(this.installButton);
            this.Controls.Add(this.saveAndPlayButton);
            this.Controls.Add(this.saveButton);
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4, 5, 4, 5);
            this.MinimumSize = new System.Drawing.Size(420, 489);
            this.Name = "MainForm";
            this.Padding = new System.Windows.Forms.Padding(0, 0, 0, 5);
            this.SizeGripStyle = System.Windows.Forms.SizeGripStyle.Show;
            this.Text = "SA2 Mod Manager";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainForm_FormClosing);
			this.Load += new System.EventHandler(this.MainForm_Load);
            this.Shown += new System.EventHandler(this.MainForm_Shown);
            groupBox2.ResumeLayout(false);
            groupBox2.PerformLayout();
            this.tabControl1.ResumeLayout(false);
            this.tabPageMods.ResumeLayout(false);
            this.tabPageMods.PerformLayout();
            this.tabPageCodes.ResumeLayout(false);
            this.tabPageGraphics.ResumeLayout(false);
            this.groupBox_Visuals.ResumeLayout(false);
            this.groupBox_Visuals.PerformLayout();
            this.groupBox_WindowMode.ResumeLayout(false);
            this.groupBox_WindowMode.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.windowHeight)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.windowWidth)).EndInit();
            this.groupBox_Display.ResumeLayout(false);
            this.groupBox_Display.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.horizontalResolution)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.verticalResolution)).EndInit();
            this.tabPageOptions.ResumeLayout(false);
            this.tabPageOptions.PerformLayout();
            this.groupBox3.ResumeLayout(false);
            this.groupBox4.ResumeLayout(false);
            this.groupBox4.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpdateFrequency)).EndInit();
            this.groupBox1.ResumeLayout(false);
            this.groupBox1.PerformLayout();
            this.tabPageDebug.ResumeLayout(false);
            this.groupBox12.ResumeLayout(false);
            this.groupBox12.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnSaveID)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnZ)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.numericUpDownTestSpawnX)).EndInit();
            this.modContextMenu.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ListView modListView;
		private System.Windows.Forms.ColumnHeader columnHeaderName;
		private System.Windows.Forms.ColumnHeader columnHeaderAuthor;
		private System.Windows.Forms.Button modDownButton;
		private System.Windows.Forms.Button modUpButton;
		private System.Windows.Forms.Button saveButton;
		private System.Windows.Forms.Button saveAndPlayButton;
		private System.Windows.Forms.Button installButton;
		private System.Windows.Forms.Label modDescription;
		private System.Windows.Forms.TabControl tabControl1;
		private System.Windows.Forms.TabPage tabPageMods;
		private System.Windows.Forms.TabPage tabPageCodes;
		private System.Windows.Forms.CheckedListBox codesCheckedListBox;
		private System.Windows.Forms.Button buttonRefreshModList;
		private System.Windows.Forms.CheckBox fileCheckBox;
		private System.Windows.Forms.CheckBox consoleCheckBox;
		private System.Windows.Forms.Button buttonNewMod;
		private System.Windows.Forms.ColumnHeader columnHeaderVersion;
		private System.Windows.Forms.TabPage tabPageOptions;
		private System.Windows.Forms.GroupBox groupBox1;
		private System.Windows.Forms.CheckBox pauseWhenInactiveCheckBox;
		private System.Windows.Forms.ContextMenuStrip modContextMenu;
		private System.Windows.Forms.ToolStripMenuItem openFolderToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem1;
		private System.Windows.Forms.ToolStripMenuItem checkForUpdatesToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem verifyToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem forceUpdateToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem uninstallToolStripMenuItem;
		private System.Windows.Forms.ToolStripSeparator toolStripMenuItem3;
		private System.Windows.Forms.ToolStripMenuItem developerToolStripMenuItem;
		private System.Windows.Forms.ToolStripMenuItem generateManifestToolStripMenuItem;
		private System.Windows.Forms.ToolTip toolTip;
		private System.Windows.Forms.GroupBox groupBox4;
		private System.Windows.Forms.Button buttonCheckForUpdates;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.CheckBox checkUpdateModsStartup;
		private System.Windows.Forms.NumericUpDown numericUpdateFrequency;
		private System.Windows.Forms.ComboBox comboUpdateFrequency;
		private System.Windows.Forms.CheckBox checkUpdateStartup;
		private System.Windows.Forms.Button installURLHandlerButton;
		private System.Windows.Forms.Button modTopButton;
		private System.Windows.Forms.Button modBottomButton;
		private System.Windows.Forms.Button configureModButton;
		private System.Windows.Forms.CheckBox screenCheckBox;
		private System.Windows.Forms.TabPage tabPageDebug;
		private System.Windows.Forms.GroupBox groupBox12;
		private System.Windows.Forms.CheckBox checkBoxTestSpawnPosition;
		private System.Windows.Forms.Button buttonTestSpawnPlay;
		private System.Windows.Forms.ComboBox comboBoxTestSpawnEvent;
		private System.Windows.Forms.CheckBox checkBoxTestSpawnEvent;
		private System.Windows.Forms.CheckBox checkBoxTestSpawnCharacter;
		private System.Windows.Forms.CheckBox checkBoxTestSpawnLevel;
		private System.Windows.Forms.ComboBox comboBoxTestSpawnCharacter;
		private System.Windows.Forms.Label labelTestSpawnY;
		private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnSaveID;
		private System.Windows.Forms.CheckBox checkBoxTestSpawnSave;
		private System.Windows.Forms.Label labelTestSpawnX;
		private System.Windows.Forms.Label labelTestSpawnZ;
		private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnZ;
		private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnY;
		private System.Windows.Forms.ComboBox comboBoxTestSpawnLevel;
		private System.Windows.Forms.NumericUpDown numericUpDownTestSpawnX;
		private System.Windows.Forms.CheckBox skipIntrocheckBox;
		private System.Windows.Forms.GroupBox groupBox3;
		private System.Windows.Forms.ComboBox profileNameBox;
		private System.Windows.Forms.Button buttonLoadProfile;
		private System.Windows.Forms.Button buttonSaveProfile;
		private System.Windows.Forms.CheckBox checkBoxMission;
		private System.Windows.Forms.ComboBox comboBoxMission;
		private System.Windows.Forms.CheckBox checkBoxTestSpawnPlayer2;
		private System.Windows.Forms.ComboBox comboBoxTestSpawnPlayer2;
		private System.Windows.Forms.TabPage tabPageGraphics;
		private System.Windows.Forms.ComboBox comboTextLanguage;
		private System.Windows.Forms.ComboBox comboVoiceLanguage;
		private System.Windows.Forms.Label label9;
		private System.Windows.Forms.Label label8;
		private System.Windows.Forms.CheckBox crashLogCheckBox;
		private System.Windows.Forms.GroupBox groupBox_Display;
		private System.Windows.Forms.GroupBox groupBox_WindowMode;
		private System.Windows.Forms.CheckBox checkWindowResize;
		private System.Windows.Forms.CheckBox customWindowSizeCheckBox;
		private System.Windows.Forms.NumericUpDown windowHeight;
		private System.Windows.Forms.NumericUpDown windowWidth;
		private System.Windows.Forms.ComboBox screenNumComboBox;
		private System.Windows.Forms.ComboBox comboResolutionPreset;
		private System.Windows.Forms.RadioButton radioWindowMode;
		private System.Windows.Forms.RadioButton radioFullscreen;
		private System.Windows.Forms.NumericUpDown horizontalResolution;
		private System.Windows.Forms.NumericUpDown verticalResolution;
		private System.Windows.Forms.RadioButton radioBorderlessWindowMode;
		private System.Windows.Forms.CheckBox disableExitPromptCheckBox;
		private System.Windows.Forms.CheckBox aspectRatioCheckBox;
		private System.Windows.Forms.GroupBox groupBox_Visuals;
		private System.Windows.Forms.CheckBox checkFrameLimiter;
		private System.Windows.Forms.ColumnHeader columnHeaderCategory;
	}
}

