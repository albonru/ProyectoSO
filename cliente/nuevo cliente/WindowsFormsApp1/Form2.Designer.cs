namespace WindowsFormsApp1
{
    partial class Form2
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form2));
            this.label3 = new System.Windows.Forms.Label();
            this.btnInstrucciones = new System.Windows.Forms.Button();
            this.panel1 = new System.Windows.Forms.Panel();
            this.piederecho = new System.Windows.Forms.PictureBox();
            this.pieizquierdo = new System.Windows.Forms.PictureBox();
            this.cuerpo = new System.Windows.Forms.PictureBox();
            this.cabeza = new System.Windows.Forms.PictureBox();
            this.manoizquierda = new System.Windows.Forms.PictureBox();
            this.manoderecha = new System.Windows.Forms.PictureBox();
            this.hazta = new System.Windows.Forms.PictureBox();
            this.label2 = new System.Windows.Forms.Label();
            this.NumLetrasBox = new System.Windows.Forms.TextBox();
            this.Box1 = new System.Windows.Forms.GroupBox();
            this.label4 = new System.Windows.Forms.Label();
            this.btnStart = new System.Windows.Forms.Button();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.lblValor = new System.Windows.Forms.Label();
            this.panel2 = new System.Windows.Forms.Panel();
            this.Boxjug = new System.Windows.Forms.TextBox();
            this.label6 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.lblMensaje = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.pictureBox2 = new System.Windows.Forms.PictureBox();
            this.panelTeclado = new System.Windows.Forms.FlowLayoutPanel();
            this.pictureBox26 = new System.Windows.Forms.PictureBox();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.pictureBox3 = new System.Windows.Forms.PictureBox();
            this.ChatBox = new System.Windows.Forms.ListBox();
            this.textChat = new System.Windows.Forms.TextBox();
            this.btnEnviarChat = new System.Windows.Forms.Button();
            this.panelChat = new System.Windows.Forms.Panel();
            this.panel1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.piederecho)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pieizquierdo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cuerpo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cabeza)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.manoizquierda)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.manoderecha)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.hazta)).BeginInit();
            this.panel2.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox26)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).BeginInit();
            this.panelChat.SuspendLayout();
            this.SuspendLayout();
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.label3.Font = new System.Drawing.Font("MS Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label3.Location = new System.Drawing.Point(100, 68);
            this.label3.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(62, 24);
            this.label3.TabIndex = 0;
            this.label3.Text = "CHAT";
            // 
            // btnInstrucciones
            // 
            this.btnInstrucciones.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.btnInstrucciones.Font = new System.Drawing.Font("MS Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnInstrucciones.Location = new System.Drawing.Point(1119, 12);
            this.btnInstrucciones.Margin = new System.Windows.Forms.Padding(4);
            this.btnInstrucciones.Name = "btnInstrucciones";
            this.btnInstrucciones.Size = new System.Drawing.Size(215, 48);
            this.btnInstrucciones.TabIndex = 10;
            this.btnInstrucciones.Tag = "btnNormas";
            this.btnInstrucciones.Text = "INSTRUCCIONES";
            this.btnInstrucciones.UseVisualStyleBackColor = false;
            this.btnInstrucciones.Click += new System.EventHandler(this.btnInstrucciones_Click);
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.LightGray;
            this.panel1.Controls.Add(this.piederecho);
            this.panel1.Controls.Add(this.pieizquierdo);
            this.panel1.Controls.Add(this.cuerpo);
            this.panel1.Controls.Add(this.cabeza);
            this.panel1.Controls.Add(this.manoizquierda);
            this.panel1.Controls.Add(this.manoderecha);
            this.panel1.Controls.Add(this.hazta);
            this.panel1.Location = new System.Drawing.Point(656, 193);
            this.panel1.Margin = new System.Windows.Forms.Padding(4);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(339, 432);
            this.panel1.TabIndex = 11;
            // 
            // piederecho
            // 
            this.piederecho.Image = ((System.Drawing.Image)(resources.GetObject("piederecho.Image")));
            this.piederecho.Location = new System.Drawing.Point(87, 357);
            this.piederecho.Margin = new System.Windows.Forms.Padding(4);
            this.piederecho.Name = "piederecho";
            this.piederecho.Size = new System.Drawing.Size(84, 75);
            this.piederecho.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.piederecho.TabIndex = 2;
            this.piederecho.TabStop = false;
            // 
            // pieizquierdo
            // 
            this.pieizquierdo.Image = ((System.Drawing.Image)(resources.GetObject("pieizquierdo.Image")));
            this.pieizquierdo.Location = new System.Drawing.Point(4, 361);
            this.pieizquierdo.Margin = new System.Windows.Forms.Padding(4);
            this.pieizquierdo.Name = "pieizquierdo";
            this.pieizquierdo.Size = new System.Drawing.Size(88, 71);
            this.pieizquierdo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.CenterImage;
            this.pieizquierdo.TabIndex = 3;
            this.pieizquierdo.TabStop = false;
            // 
            // cuerpo
            // 
            this.cuerpo.Image = ((System.Drawing.Image)(resources.GetObject("cuerpo.Image")));
            this.cuerpo.Location = new System.Drawing.Point(75, 257);
            this.cuerpo.Margin = new System.Windows.Forms.Padding(4);
            this.cuerpo.Name = "cuerpo";
            this.cuerpo.Size = new System.Drawing.Size(24, 108);
            this.cuerpo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.cuerpo.TabIndex = 7;
            this.cuerpo.TabStop = false;
            // 
            // cabeza
            // 
            this.cabeza.Image = ((System.Drawing.Image)(resources.GetObject("cabeza.Image")));
            this.cabeza.Location = new System.Drawing.Point(47, 187);
            this.cabeza.Margin = new System.Windows.Forms.Padding(4);
            this.cabeza.Name = "cabeza";
            this.cabeza.Size = new System.Drawing.Size(91, 81);
            this.cabeza.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.cabeza.TabIndex = 1;
            this.cabeza.TabStop = false;
            // 
            // manoizquierda
            // 
            this.manoizquierda.Image = ((System.Drawing.Image)(resources.GetObject("manoizquierda.Image")));
            this.manoizquierda.Location = new System.Drawing.Point(4, 274);
            this.manoizquierda.Margin = new System.Windows.Forms.Padding(4);
            this.manoizquierda.Name = "manoizquierda";
            this.manoizquierda.Size = new System.Drawing.Size(75, 66);
            this.manoizquierda.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.manoizquierda.TabIndex = 5;
            this.manoizquierda.TabStop = false;
            // 
            // manoderecha
            // 
            this.manoderecha.Image = ((System.Drawing.Image)(resources.GetObject("manoderecha.Image")));
            this.manoderecha.Location = new System.Drawing.Point(87, 274);
            this.manoderecha.Margin = new System.Windows.Forms.Padding(4);
            this.manoderecha.Name = "manoderecha";
            this.manoderecha.Size = new System.Drawing.Size(99, 66);
            this.manoderecha.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.manoderecha.TabIndex = 6;
            this.manoderecha.TabStop = false;
            // 
            // hazta
            // 
            this.hazta.Image = ((System.Drawing.Image)(resources.GetObject("hazta.Image")));
            this.hazta.Location = new System.Drawing.Point(4, 0);
            this.hazta.Margin = new System.Windows.Forms.Padding(4);
            this.hazta.Name = "hazta";
            this.hazta.Size = new System.Drawing.Size(339, 428);
            this.hazta.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.hazta.TabIndex = 0;
            this.hazta.TabStop = false;
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.BackColor = System.Drawing.Color.Transparent;
            this.label2.Font = new System.Drawing.Font("MS Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label2.ForeColor = System.Drawing.Color.SaddleBrown;
            this.label2.Location = new System.Drawing.Point(28, 97);
            this.label2.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(322, 24);
            this.label2.TabIndex = 12;
            this.label2.Text = "¡NO DEJES QUE BOB MUERA!";
            // 
            // NumLetrasBox
            // 
            this.NumLetrasBox.BackColor = System.Drawing.Color.OldLace;
            this.NumLetrasBox.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.NumLetrasBox.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.NumLetrasBox.Font = new System.Drawing.Font("MS Gothic", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.NumLetrasBox.ForeColor = System.Drawing.Color.DarkGreen;
            this.NumLetrasBox.Location = new System.Drawing.Point(447, 467);
            this.NumLetrasBox.Margin = new System.Windows.Forms.Padding(4);
            this.NumLetrasBox.Multiline = true;
            this.NumLetrasBox.Name = "NumLetrasBox";
            this.NumLetrasBox.Size = new System.Drawing.Size(48, 38);
            this.NumLetrasBox.TabIndex = 15;
            // 
            // Box1
            // 
            this.Box1.BackColor = System.Drawing.Color.DarkGreen;
            this.Box1.BackgroundImage = global::WindowsFormsApp1.Properties.Resources.piz;
            this.Box1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Box1.ForeColor = System.Drawing.Color.Linen;
            this.Box1.Location = new System.Drawing.Point(98, 183);
            this.Box1.Margin = new System.Windows.Forms.Padding(4);
            this.Box1.Name = "Box1";
            this.Box1.Padding = new System.Windows.Forms.Padding(4);
            this.Box1.Size = new System.Drawing.Size(559, 82);
            this.Box1.TabIndex = 17;
            this.Box1.TabStop = false;
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Font = new System.Drawing.Font("Microsoft Sans Serif", 15.75F, ((System.Drawing.FontStyle)((System.Drawing.FontStyle.Bold | System.Drawing.FontStyle.Italic))), System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label4.Location = new System.Drawing.Point(207, 251);
            this.label4.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(0, 31);
            this.label4.TabIndex = 18;
            // 
            // btnStart
            // 
            this.btnStart.BackColor = System.Drawing.Color.Tomato;
            this.btnStart.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.btnStart.Cursor = System.Windows.Forms.Cursors.Arrow;
            this.btnStart.Font = new System.Drawing.Font("MS Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnStart.ForeColor = System.Drawing.SystemColors.ControlText;
            this.btnStart.Location = new System.Drawing.Point(987, 11);
            this.btnStart.Margin = new System.Windows.Forms.Padding(4);
            this.btnStart.Name = "btnStart";
            this.btnStart.Size = new System.Drawing.Size(115, 49);
            this.btnStart.TabIndex = 8;
            this.btnStart.Tag = "btnstart";
            this.btnStart.Text = "START";
            this.btnStart.UseVisualStyleBackColor = false;
            this.btnStart.Click += new System.EventHandler(this.btnStart_Click);
            // 
            // timer1
            // 
            this.timer1.Interval = 1000;
            this.timer1.Tag = "timer";
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // lblValor
            // 
            this.lblValor.BackColor = System.Drawing.SystemColors.Window;
            this.lblValor.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblValor.Location = new System.Drawing.Point(1159, 740);
            this.lblValor.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.lblValor.Name = "lblValor";
            this.lblValor.Size = new System.Drawing.Size(40, 36);
            this.lblValor.TabIndex = 23;
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.DarkSeaGreen;
            this.panel2.Controls.Add(this.Boxjug);
            this.panel2.Controls.Add(this.label6);
            this.panel2.Controls.Add(this.btnInstrucciones);
            this.panel2.Controls.Add(this.btnStart);
            this.panel2.Location = new System.Drawing.Point(-9, 0);
            this.panel2.Margin = new System.Windows.Forms.Padding(4);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(1392, 72);
            this.panel2.TabIndex = 26;
            // 
            // Boxjug
            // 
            this.Boxjug.BackColor = System.Drawing.Color.DarkSeaGreen;
            this.Boxjug.Font = new System.Drawing.Font("MS Gothic", 10.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Boxjug.Location = new System.Drawing.Point(220, 29);
            this.Boxjug.Name = "Boxjug";
            this.Boxjug.Size = new System.Drawing.Size(106, 25);
            this.Boxjug.TabIndex = 27;
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Font = new System.Drawing.Font("MS Gothic", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label6.Location = new System.Drawing.Point(86, 30);
            this.label6.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(101, 24);
            this.label6.TabIndex = 26;
            this.label6.Text = "jugador";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(0, 0);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(0, 17);
            this.label7.TabIndex = 28;
            // 
            // lblMensaje
            // 
            this.lblMensaje.AutoEllipsis = true;
            this.lblMensaje.BackColor = System.Drawing.Color.LemonChiffon;
            this.lblMensaje.Font = new System.Drawing.Font("MS Gothic", 22.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblMensaje.Image = ((System.Drawing.Image)(resources.GetObject("lblMensaje.Image")));
            this.lblMensaje.Location = new System.Drawing.Point(116, 395);
            this.lblMensaje.Name = "lblMensaje";
            this.lblMensaje.Size = new System.Drawing.Size(346, 66);
            this.lblMensaje.TabIndex = 29;
            // 
            // label1
            // 
            this.label1.BackColor = System.Drawing.Color.Green;
            this.label1.Font = new System.Drawing.Font("MS Gothic", 12F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.ForeColor = System.Drawing.Color.Black;
            this.label1.Image = ((System.Drawing.Image)(resources.GetObject("label1.Image")));
            this.label1.Location = new System.Drawing.Point(109, 467);
            this.label1.Margin = new System.Windows.Forms.Padding(4, 0, 4, 0);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(312, 38);
            this.label1.TabIndex = 16;
            this.label1.Text = "Longitud de la palabra";
            this.label1.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // pictureBox2
            // 
            this.pictureBox2.BackColor = System.Drawing.Color.OldLace;
            this.pictureBox2.BackgroundImage = global::WindowsFormsApp1.Properties.Resources.piz;
            this.pictureBox2.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox2.Image")));
            this.pictureBox2.Location = new System.Drawing.Point(98, 380);
            this.pictureBox2.Name = "pictureBox2";
            this.pictureBox2.Size = new System.Drawing.Size(427, 178);
            this.pictureBox2.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox2.TabIndex = 30;
            this.pictureBox2.TabStop = false;
            // 
            // panelTeclado
            // 
            this.panelTeclado.BackColor = System.Drawing.Color.BlanchedAlmond;
            this.panelTeclado.BackgroundImage = global::WindowsFormsApp1.Properties.Resources.madera;
            this.panelTeclado.Location = new System.Drawing.Point(98, 685);
            this.panelTeclado.Margin = new System.Windows.Forms.Padding(4);
            this.panelTeclado.Name = "panelTeclado";
            this.panelTeclado.Size = new System.Drawing.Size(883, 145);
            this.panelTeclado.TabIndex = 22;
            // 
            // pictureBox26
            // 
            this.pictureBox26.BackgroundImage = global::WindowsFormsApp1.Properties.Resources.pizarra;
            this.pictureBox26.Image = global::WindowsFormsApp1.Properties.Resources.pizarra21;
            this.pictureBox26.Location = new System.Drawing.Point(33, 135);
            this.pictureBox26.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBox26.Name = "pictureBox26";
            this.pictureBox26.Size = new System.Drawing.Size(1017, 542);
            this.pictureBox26.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox26.TabIndex = 14;
            this.pictureBox26.TabStop = false;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Cursor = System.Windows.Forms.Cursors.No;
            this.pictureBox1.Image = global::WindowsFormsApp1.Properties.Resources.mano02_1;
            this.pictureBox1.Location = new System.Drawing.Point(1075, 670);
            this.pictureBox1.Margin = new System.Windows.Forms.Padding(4);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(319, 176);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox1.TabIndex = 27;
            this.pictureBox1.TabStop = false;
            // 
            // pictureBox3
            // 
            this.pictureBox3.Image = ((System.Drawing.Image)(resources.GetObject("pictureBox3.Image")));
            this.pictureBox3.Location = new System.Drawing.Point(0, 0);
            this.pictureBox3.Name = "pictureBox3";
            this.pictureBox3.Size = new System.Drawing.Size(280, 498);
            this.pictureBox3.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
            this.pictureBox3.TabIndex = 31;
            this.pictureBox3.TabStop = false;
            // 
            // ChatBox
            // 
            this.ChatBox.FormattingEnabled = true;
            this.ChatBox.ItemHeight = 16;
            this.ChatBox.Location = new System.Drawing.Point(5, 116);
            this.ChatBox.Margin = new System.Windows.Forms.Padding(4);
            this.ChatBox.Name = "ChatBox";
            this.ChatBox.Size = new System.Drawing.Size(255, 244);
            this.ChatBox.TabIndex = 4;
            // 
            // textChat
            // 
            this.textChat.Location = new System.Drawing.Point(5, 368);
            this.textChat.Margin = new System.Windows.Forms.Padding(4);
            this.textChat.Name = "textChat";
            this.textChat.Size = new System.Drawing.Size(255, 22);
            this.textChat.TabIndex = 3;
            // 
            // btnEnviarChat
            // 
            this.btnEnviarChat.BackColor = System.Drawing.Color.RosyBrown;
            this.btnEnviarChat.Font = new System.Drawing.Font("MS Gothic", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.btnEnviarChat.ForeColor = System.Drawing.SystemColors.ActiveCaptionText;
            this.btnEnviarChat.Location = new System.Drawing.Point(5, 398);
            this.btnEnviarChat.Margin = new System.Windows.Forms.Padding(4);
            this.btnEnviarChat.Name = "btnEnviarChat";
            this.btnEnviarChat.Size = new System.Drawing.Size(255, 36);
            this.btnEnviarChat.TabIndex = 2;
            this.btnEnviarChat.Tag = "btnChat";
            this.btnEnviarChat.Text = "Enviar";
            this.btnEnviarChat.UseVisualStyleBackColor = false;
            this.btnEnviarChat.Click += new System.EventHandler(this.btnEnviarChat_Click);
            // 
            // panelChat
            // 
            this.panelChat.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.panelChat.Controls.Add(this.btnEnviarChat);
            this.panelChat.Controls.Add(this.textChat);
            this.panelChat.Controls.Add(this.ChatBox);
            this.panelChat.Controls.Add(this.label3);
            this.panelChat.Controls.Add(this.pictureBox3);
            this.panelChat.Font = new System.Drawing.Font("Microsoft Sans Serif", 7.8F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.panelChat.Location = new System.Drawing.Point(1075, 135);
            this.panelChat.Margin = new System.Windows.Forms.Padding(4);
            this.panelChat.Name = "panelChat";
            this.panelChat.Size = new System.Drawing.Size(280, 498);
            this.panelChat.TabIndex = 8;
            // 
            // Form2
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.SystemColors.ButtonHighlight;
            this.ClientSize = new System.Drawing.Size(1384, 846);
            this.Controls.Add(this.lblMensaje);
            this.Controls.Add(this.NumLetrasBox);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.pictureBox2);
            this.Controls.Add(this.label7);
            this.Controls.Add(this.lblValor);
            this.Controls.Add(this.panelTeclado);
            this.Controls.Add(this.label4);
            this.Controls.Add(this.Box1);
            this.Controls.Add(this.label2);
            this.Controls.Add(this.panelChat);
            this.Controls.Add(this.panel1);
            this.Controls.Add(this.pictureBox26);
            this.Controls.Add(this.pictureBox1);
            this.Controls.Add(this.panel2);
            this.Margin = new System.Windows.Forms.Padding(4);
            this.Name = "Form2";
            this.Text = "Form2";
            this.panel1.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.piederecho)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pieizquierdo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cuerpo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cabeza)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.manoizquierda)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.manoderecha)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.hazta)).EndInit();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox2)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox26)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox3)).EndInit();
            this.panelChat.ResumeLayout(false);
            this.panelChat.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.Button btnInstrucciones;
        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.PictureBox pieizquierdo;
        private System.Windows.Forms.PictureBox piederecho;
        private System.Windows.Forms.PictureBox hazta;
        private System.Windows.Forms.TextBox NumLetrasBox;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.PictureBox cuerpo;
        private System.Windows.Forms.GroupBox Box1;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.PictureBox manoderecha;
        private System.Windows.Forms.PictureBox manoizquierda;
        private System.Windows.Forms.PictureBox cabeza;
        private System.Windows.Forms.Button btnStart;
        private System.Windows.Forms.FlowLayoutPanel panelTeclado;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Label lblValor;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.PictureBox pictureBox26;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label lblMensaje;
        private System.Windows.Forms.PictureBox pictureBox2;
        private System.Windows.Forms.TextBox Boxjug;
        private System.Windows.Forms.PictureBox pictureBox3;
        private System.Windows.Forms.ListBox ChatBox;
        private System.Windows.Forms.TextBox textChat;
        private System.Windows.Forms.Button btnEnviarChat;
        private System.Windows.Forms.Panel panelChat;
    }
}