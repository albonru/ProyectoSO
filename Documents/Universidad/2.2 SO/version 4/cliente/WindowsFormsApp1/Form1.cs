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

                string test = Encoding.ASCII.GetString(msg2);
                string[] trozos = Encoding.ASCII.GetString(msg2).Split('/');
                //MessageBox.Show("Message es: " + BitConverter.ToString(msg2));
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
                    case 7: // Invitacion a jugar

                        string[] invitacion = message.Split('/');
                        string anfitrion = invitacion[0];
                        int idPartida = Convert.ToInt32(trozos[2]);
                        string texto = anfitrion + " te ha invitado a jugar.\n ¿Deseas aceptar la invitación?\n";
                        DialogResult res = MessageBox.Show(texto, "Invitación", MessageBoxButtons.YesNo);

                        switch (res) //Respuesta Invitacion (4)
                        {
                            case DialogResult.Yes:
                                string aceptar = "9/" + "si" + "/" + idPartida;
                                byte[] msg1 = System.Text.Encoding.ASCII.GetBytes(aceptar);
                                server.Send(msg1);
                                break;
                            case DialogResult.No:
                                string cancelar = "9/" + "no" + "/" + idPartida;
                                byte[] msg = System.Text.Encoding.ASCII.GetBytes(cancelar);
                                server.Send(msg);
                                break;
                        }
                        break;

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
                        //respuesta invitacion a jugar

                    case 9:
                        string[] resInvitacion = message.Split('/');
                        string answer = resInvitacion[0];

                        if (answer == "si")
                            MessageBox.Show("La respuesta a tu invitacion es: " + answer + "\nEmpieza la partida!!");
                        else if (answer == "no")
                            MessageBox.Show("La respuesta a tu invitacion es: " + answer + "\nNadie te quiere :(");
                        else
                            MessageBox.Show("Algo raro pasa aquí");
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

        private void button6_Click(object sender, EventArgs e)
        {
            {
                //Validar los datos
                int n = this.ConectadosGrid.SelectedCells.Count;

                if (n == 0)
                {
                    MessageBox.Show("Numero de jugadores inválido");
                    return;
                }
                else
                {
                    int i = 0;
                    while (n > i)
                    {
                        string nombresgrid = ConectadosGrid.Rows[i].Cells[0].Value.ToString();
                        nombresgrid = nombresgrid.TrimEnd('\0');
                        string message = "7/" + nombresgrid;
                        byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                        server.Send(msg);
                        i++;
                    }
                }

            }

        }

    }
}