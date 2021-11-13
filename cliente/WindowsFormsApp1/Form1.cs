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
using System.Threading;

namespace WindowsFormsApp1
{
    public partial class Form1 : Form
    {
        Socket server;
        Thread atender;

        public Form1()
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false; //Para que en los formularios se puedan acceder desde
            //threads diferentes a los que se crearon
        }


        private void AtenderServidor()
        {
            while (true)
            {
                //Mensaje del servidor
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                string[] trozos = Encoding.ASCII.GetString(msg2).Split('/');
                int codigo = Convert.ToInt32(trozos[0]);
                string message = trozos[1].Split('\0')[0];

                switch (codigo) //todos lo que resibimos esta en el thread 
                {
                    case 0: //Creamos cuenta 
                         MessageBox.Show("Se ha creado el nuevo usuario");
                         break;
                    case 1: //login
                        if (message == "1")
                            MessageBox.Show("Has iniciado sesion como " + Username.Text);
                        else if (message == "2")
                            MessageBox.Show("Ya habías iniciado sesión como " + Username.Text + " en este cliente");
                        else if (message == "-1")
                            MessageBox.Show("No se ha podido iniciar sesion, la lista está llena");
                        else if (message == "-2")
                            MessageBox.Show("Datos de acceso invalidos");
                        break;

                    case 2://partidas ganadas
                        MessageBox.Show(Username.Text + " ha ganado" + message + " partidas");
                        break;


                    case 3: //N. puntos ganados
                        MessageBox.Show(Username.Text + " ha ganado " + message + " puntos");
                        break;

                    case 4: //Porcentaje ganadas
                        MessageBox.Show(Username.Text + " ha ganado el " + message + " % de las partidas");
                        break;

                    /*case 5: //Logout*/

                    case 8: //Notificacion lista de conectados 
                        int numero = Convert.ToInt32(message);
                        if (numero != 0)
                        {
                            ConectadosGrid.ColumnCount = 1;
                            ConectadosGrid.RowCount = numero;
                            for (int i = 2; i < trozos.Length; i++)
                                ConectadosGrid.Rows[i - 2].Cells[0].Value = trozos[i];

                        }
                        break;

                    default:
                        MessageBox.Show("Error en la peticion");
                        break;


                }
            }
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
            //Conectamos el servidror con el cliente atravez de la ip
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


            //Pongo en marcha el thread 
            ThreadStart ts = delegate { AtenderServidor(); };
            atender = new Thread(ts);
            atender.Start();

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

            }

            else if (PuntosTotales.Checked)
            {
                //Enviamos la petición al servidor (nºPuntosGanados)
                string message = "3/" + Username.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
            }

            else if (PorcentajeGanadas.Checked)
            {
                //Enviamos la petición al servidor(%PartidasGanadas)
                string message = "4/" + Username.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
            }

        }
        //Crear Cuenta
        private void button3_Click(object sender, EventArgs e)
        {
            //Enviamos la petición al servidor
            string message = "0/" + Username.Text + "/" + Password.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
            server.Send(msg);
            /*//Recibimos la respuesta del servidor
            byte[] msg2 = new byte[80];
            server.Receive(msg2);
            message = Encoding.ASCII.GetString(msg2).Split('\0')[0];
            MessageBox.Show("Se ha creado el nuevo usuario");*/
        }
        //LogIn
        private void button4_Click(object sender, EventArgs e)
        {
            try
            {
                if ((Username.Text != "") && (Password.Text != ""))
                {
                    //Enviamos la petición al servidor
                    string message = "1/" + Username.Text + "/" + Password.Text;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                    server.Send(msg);

                }
                else
                    MessageBox.Show("Error en los campos de datos de acceso");
            }
            catch (Exception)
            {
                //si hay excepcion imprimimos error y salimos del programa con un return
                MessageBox.Show("Error en el login");
                return;
            }
        }
        //Desconectar
        private void button2_Click(object sender, EventArgs e)
        {
            string mensaje = "6/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Nos desconectamos
            atender.Abort();
            this.BackColor = Color.Gray;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        private void textBox5_TextChanged(object sender, EventArgs e)
        {

        }

        private void PorcentajeGanadas_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void panel3_Paint(object sender, PaintEventArgs e)
        {

        }

        /* private void button6_Click(object sender, EventArgs e)
          {
              try
              {
                  //connectados
                  string mensaje = "8/";
                  //Enviamos petición al servidor
                  byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                  server.Send(msg);
              }
              catch (Exception)
              {
                  //Si hay excepcion imprimimos error y salimos del programa con return
                  MessageBox.Show("Error en la peticion");
                  return;
              }

          }*/




    }
}