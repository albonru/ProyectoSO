using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net;
using System.Net.Sockets;
using System.IO;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        Socket server;
        public Form1()
        {
            InitializeComponent();
        }

        private void panel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }
        //Conectar
        private void button1_Click(object sender, EventArgs e)
        {
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9050);
            //Crear el socket
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep);
                this.BackColor = Color.Green;
                MessageBox.Show("Conexion establecida correctamente");
            }
            catch (SocketException ex)
            {
                MessageBox.Show("No se ha podido conectar con el servidor");
                return;
            }
        }
        //Consultar peticion
        private void button5_Click(object sender, EventArgs e)
        {
            if (PartidasGanadas.Checked)
            {
                //Enviamos la petición al servidor (nºpartidasGanadas)
                string message = "2/" + Username.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                message = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(Username.Text + "ha ganado" + message + " partidas");
            }
            else if (PuntosTotales.Checked)
            {
                //Enviamos la petición al servidor (nºPuntosGanados)
                string message = "3/" + Username.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                message = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(Username.Text + " ha ganado " + message + "puntos");
            }
            else if (PorcentajeGanadas.Checked)
            {
                //Enviamos la petición al servidor(%PartidasGanadas)
                string message = "4/" + Username.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
                //Recibimos la respuesta del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                message = Encoding.ASCII.GetString(msg2).Split('\0')[0];
                MessageBox.Show(Username.Text + " ha ganado el " + message + "% de las partidas");
            }

        }
        //Crear Cuenta
        private void button3_Click(object sender, EventArgs e)
        {
            //Enviamos la petición al servidor
            string message = "0/" + Username.Text + "/" + Password.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
            server.Send(msg);
            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            message = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show("Se ha creado el nuevo usuario");
        }
        //LogIn
        private void button4_Click(object sender, EventArgs e)
        {
            //Enviamos la petición al servidor
            string message = "1/" + Username.Text + "/" + Password.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
            server.Send(msg);
            //Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            message = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show("Entrada con exito");
        }
        //Desconectar
        private void button2_Click(object sender, EventArgs e)
        {
            string mensaje = "5/";

            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
