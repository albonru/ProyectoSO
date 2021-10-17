
namespace WindowsFormsApp1
{
    partial class Form1
    {
        /// <summary>
        /// Variable del diseñador necesaria.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Limpiar los recursos que se estén usando.
        /// </summary>
        /// <param name="disposing">true si los recursos administrados se deben desechar; false en caso contrario.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Código generado por el Diseñador de Windows Forms

        /// <summary>
        /// Método necesario para admitir el Diseñador. No se puede modificar
        /// el contenido de este método con el editor de código.
        /// </summary>
        private void InitializeComponent()
        {
            this.panel1 = new System.Windows.Forms.Panel();
            this.panel2 = new System.Windows.Forms.Panel();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.Username = new System.Windows.Forms.TextBox();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.textBox3 = new System.Windows.Forms.TextBox();
            this.Password = new System.Windows.Forms.TextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.button4 = new System.Windows.Forms.Button();
            this.PartidasGanadas = new System.Windows.Forms.RadioButton();
            this.PuntosTotales = new System.Windows.Forms.RadioButton();
            this.PorcentajeGanadas = new System.Windows.Forms.RadioButton();
            this.textBox5 = new System.Windows.Forms.TextBox();
            this.button5 = new System.Windows.Forms.Button();
            this.panel1.SuspendLayout();
            this.panel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // panel1
            // 
            this.panel1.BackColor = System.Drawing.Color.Azure;
            this.panel1.Controls.Add(this.button4);
            this.panel1.Controls.Add(this.button3);
            this.panel1.Controls.Add(this.Password);
            this.panel1.Controls.Add(this.textBox3);
            this.panel1.Controls.Add(this.textBox2);
            this.panel1.Controls.Add(this.Username);
            this.panel1.Location = new System.Drawing.Point(85, 93);
            this.panel1.Name = "panel1";
            this.panel1.Size = new System.Drawing.Size(589, 248);
            this.panel1.TabIndex = 0;
            this.panel1.Paint += new System.Windows.Forms.PaintEventHandler(this.panel1_Paint);
            // 
            // panel2
            // 
            this.panel2.BackColor = System.Drawing.Color.Azure;
            this.panel2.Controls.Add(this.button5);
            this.panel2.Controls.Add(this.textBox5);
            this.panel2.Controls.Add(this.PorcentajeGanadas);
            this.panel2.Controls.Add(this.PuntosTotales);
            this.panel2.Controls.Add(this.PartidasGanadas);
            this.panel2.Location = new System.Drawing.Point(85, 373);
            this.panel2.Name = "panel2";
            this.panel2.Size = new System.Drawing.Size(583, 233);
            this.panel2.TabIndex = 1;
            // 
            // button1
            // 
            this.button1.BackColor = System.Drawing.Color.NavajoWhite;
            this.button1.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button1.Location = new System.Drawing.Point(85, 43);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(285, 44);
            this.button1.TabIndex = 0;
            this.button1.Text = "Conectar";
            this.button1.UseVisualStyleBackColor = false;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.BackColor = System.Drawing.Color.NavajoWhite;
            this.button2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button2.Location = new System.Drawing.Point(387, 43);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(287, 43);
            this.button2.TabIndex = 2;
            this.button2.Text = "Desconectar";
            this.button2.UseVisualStyleBackColor = false;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // Username
            // 
            this.Username.Location = new System.Drawing.Point(337, 48);
            this.Username.Name = "Username";
            this.Username.Size = new System.Drawing.Size(208, 22);
            this.Username.TabIndex = 0;
            // 
            // textBox2
            // 
            this.textBox2.BackColor = System.Drawing.Color.Azure;
            this.textBox2.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox2.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox2.Location = new System.Drawing.Point(43, 50);
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(161, 20);
            this.textBox2.TabIndex = 1;
            this.textBox2.Text = "Usuario";
            // 
            // textBox3
            // 
            this.textBox3.BackColor = System.Drawing.Color.Azure;
            this.textBox3.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox3.Location = new System.Drawing.Point(43, 103);
            this.textBox3.Name = "textBox3";
            this.textBox3.Size = new System.Drawing.Size(161, 20);
            this.textBox3.TabIndex = 2;
            this.textBox3.Text = "Contraseña";
            // 
            // Password
            // 
            this.Password.Location = new System.Drawing.Point(337, 101);
            this.Password.Name = "Password";
            this.Password.Size = new System.Drawing.Size(208, 22);
            this.Password.TabIndex = 3;
            // 
            // button3
            // 
            this.button3.BackColor = System.Drawing.Color.NavajoWhite;
            this.button3.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button3.Location = new System.Drawing.Point(43, 171);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(204, 45);
            this.button3.TabIndex = 4;
            this.button3.Text = "Crear cuenta";
            this.button3.UseVisualStyleBackColor = false;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // button4
            // 
            this.button4.BackColor = System.Drawing.Color.NavajoWhite;
            this.button4.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button4.Location = new System.Drawing.Point(337, 169);
            this.button4.Name = "button4";
            this.button4.Size = new System.Drawing.Size(208, 47);
            this.button4.TabIndex = 5;
            this.button4.Text = "Iniciar sesion";
            this.button4.UseVisualStyleBackColor = false;
            this.button4.Click += new System.EventHandler(this.button4_Click);
            // 
            // PartidasGanadas
            // 
            this.PartidasGanadas.AutoSize = true;
            this.PartidasGanadas.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PartidasGanadas.Location = new System.Drawing.Point(58, 66);
            this.PartidasGanadas.Name = "PartidasGanadas";
            this.PartidasGanadas.Size = new System.Drawing.Size(224, 24);
            this.PartidasGanadas.TabIndex = 0;
            this.PartidasGanadas.TabStop = true;
            this.PartidasGanadas.Text = "Partidas Ganadas Totales";
            this.PartidasGanadas.UseVisualStyleBackColor = true;
            this.PartidasGanadas.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // PuntosTotales
            // 
            this.PuntosTotales.AutoSize = true;
            this.PuntosTotales.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PuntosTotales.Location = new System.Drawing.Point(58, 93);
            this.PuntosTotales.Name = "PuntosTotales";
            this.PuntosTotales.Size = new System.Drawing.Size(150, 24);
            this.PuntosTotales.TabIndex = 1;
            this.PuntosTotales.TabStop = true;
            this.PuntosTotales.Text = "Puntos ganados";
            this.PuntosTotales.UseVisualStyleBackColor = true;
            // 
            // PorcentajeGanadas
            // 
            this.PorcentajeGanadas.AutoSize = true;
            this.PorcentajeGanadas.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.PorcentajeGanadas.Location = new System.Drawing.Point(58, 120);
            this.PorcentajeGanadas.Name = "PorcentajeGanadas";
            this.PorcentajeGanadas.Size = new System.Drawing.Size(240, 24);
            this.PorcentajeGanadas.TabIndex = 2;
            this.PorcentajeGanadas.TabStop = true;
            this.PorcentajeGanadas.Text = "Prcentaje Partidas Ganadas";
            this.PorcentajeGanadas.UseVisualStyleBackColor = true;
            // 
            // textBox5
            // 
            this.textBox5.BackColor = System.Drawing.Color.Azure;
            this.textBox5.BorderStyle = System.Windows.Forms.BorderStyle.None;
            this.textBox5.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.2F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.textBox5.Location = new System.Drawing.Point(58, 22);
            this.textBox5.Name = "textBox5";
            this.textBox5.Size = new System.Drawing.Size(161, 20);
            this.textBox5.TabIndex = 4;
            this.textBox5.Text = "Consultas";
            this.textBox5.TextChanged += new System.EventHandler(this.textBox5_TextChanged);
            // 
            // button5
            // 
            this.button5.BackColor = System.Drawing.Color.NavajoWhite;
            this.button5.Font = new System.Drawing.Font("Microsoft Sans Serif", 10.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.button5.Location = new System.Drawing.Point(58, 174);
            this.button5.Name = "button5";
            this.button5.Size = new System.Drawing.Size(487, 39);
            this.button5.TabIndex = 5;
            this.button5.Text = "Consular";
            this.button5.UseVisualStyleBackColor = false;
            this.button5.Click += new System.EventHandler(this.button5_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(859, 618);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.panel2);
            this.Controls.Add(this.panel1);
            this.Name = "Form1";
            this.Text = "Form1";
            this.panel1.ResumeLayout(false);
            this.panel1.PerformLayout();
            this.panel2.ResumeLayout(false);
            this.panel2.PerformLayout();
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Panel panel1;
        private System.Windows.Forms.Button button4;
        private System.Windows.Forms.Button button3;
        private System.Windows.Forms.TextBox Password;
        private System.Windows.Forms.TextBox textBox3;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.TextBox Username;
        private System.Windows.Forms.Panel panel2;
        private System.Windows.Forms.Button button5;
        private System.Windows.Forms.TextBox textBox5;
        private System.Windows.Forms.RadioButton PorcentajeGanadas;
        private System.Windows.Forms.RadioButton PuntosTotales;
        private System.Windows.Forms.RadioButton PartidasGanadas;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
    }
}

