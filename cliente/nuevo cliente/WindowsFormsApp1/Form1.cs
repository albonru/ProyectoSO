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

        //Variables globales
        string username;

        string[] jugadores = new string[4];  //lista de invitados
        int numSeleccionados = 0; //contador número de invitados
        int[] filasGrid = new int[3];
        int idPartida;

        //estructura donde se guardan los formularios que se van creando
        List<Form2> formularios = new List<Form2>();  
        int[] partidas = new int [40];

        //delegados
        delegate void DelegadoParaPonerGrid(string[] trozos);
        delegate void DelegadoParaPonerTexto(string mensaje);
        delegate void DelegadoParaEscribirAlChat(string mensaje);
        delegate void DelegadorParaEcribirRepuesta(string mensaje);


        //funcion para escribir consultas
        private void Mensaje_Respuesta(string respuesta)
        {
            label17.Text = respuesta;
        }


        public Form1()
        {
            InitializeComponent();
        }

        //Actualizacion lista de conectados 
        private void Form1_Load(object sender, EventArgs e)
        {
            ConectadosGrid.Columns.Add("Usuario", "Usuario");
            ConectadosGrid.RowCount = 10;
            ConectadosGrid.ColumnHeadersVisible = true;
            ConectadosGrid.RowHeadersVisible = false;
            ConectadosGrid.AutoSizeColumnsMode = DataGridViewAutoSizeColumnsMode.AllCells;
            int i;
            for (i = 0; i < 10; i++)
            {
                ConectadosGrid[0, i].Value = "";
            }
        }

        //Abre el form2 y envía el idPartida el socket y el nombre del usuario
        public void Form2Abrir(int idPartida) 
        {
            partidas[idPartida] = formularios.Count();
            Form2 f2 = new Form2(idPartida, server, username);//Se guarda el formulario que se acaba de de crear
            formularios.Add(f2);
            f2.ShowDialog();
            
        }

        //Actualiza el grid de la lista de conectados
        private void ActualizaGrid(string[] trozos)
        {
            int numero = Convert.ToInt32(trozos[1]);
            if (numero != 0)
            {
                ConectadosGrid.ColumnCount = 1;
                ConectadosGrid.RowCount = numero;
                for (int i = 2; i < trozos.Length; i++)
                    ConectadosGrid.Rows[i - 2].Cells[0].Value = trozos[i];
            }
        }

        //thread que atiende los mensajes del servidor
        private void AtenderServidor() 
        {
            while (true)
            {
                byte[] msg2 = new byte[80];
                server.Receive(msg2);
                string[] trozos = Encoding.ASCII.GetString(msg2).Split('/');
                int codigo = Convert.ToInt32(trozos[0]);
                string message = trozos[1].Split('\0')[0]; //Eliminamos la basura que acompaña a la frase
                

                //Averiguo el tipo de mensaje 
                switch (codigo)
                {
                    case 0: //Creamos cuenta 
                        MessageBox.Show("Se ha creado el nuevo usuario");
                        break;

                    case 1: //Login
                        if (message == "1")
                            MessageBox.Show("Has iniciado sesion como " + username);
                        else if (message == "2")
                            MessageBox.Show("Ya habías iniciado sesión como " + username + " en este cliente");
                        else if (message == "-1")
                            MessageBox.Show("No se ha podido iniciar sesion, la lista está llena");
                        break;

                    case 2://Partidas ganadas
                        MessageBox.Show(username + " ha ganado" + message + " partidas");
                        break;

                    case 3: //N. puntos ganados
                        MessageBox.Show(username + " ha ganado " + message + " puntos");
                        break;

                    case 4: //Porcentaje partidas ganadas
                        MessageBox.Show(username + " ha ganado el " + message + " % de las partidas");
                        break;

                    
                    case 7: //Invitacion a jugar. 
                        string[] invitacion = message.Split('/');
                        string anfitrion = invitacion[0];
                        idPartida = Convert.ToInt32(trozos[2]);
                        string texto = anfitrion + " te ha invitado a jugar.\n ¿Deseas aceptar la invitación?\n";
                        DialogResult res = MessageBox.Show(texto, "Invitacion", MessageBoxButtons.YesNo);

                        switch (res) //Respuesta invitación 
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
                        ConectadosGrid.Invoke(new DelegadoParaPonerGrid(ActualizaGrid), new Object[] { trozos });
                        break;

                    case 9: //Respuesta invitacion
                        string answer = message;

                        if (answer == "si") //Invitación aceptada
                        {
                            MessageBox.Show("La respuesta a tu invitacion es: " + answer + "\nEmpieza la partida!!");
                            string chat = trozos[2].Split('\0')[0];
                            //idPartida = Convert.ToInt32(message); //message = trozos[1]
                           
                            ThreadStart ts = delegate { Form2Abrir(idPartida); };
                            Thread T = new Thread(ts);
                            T.Start();
                          
                        }
                        else if (answer == "no")//Invitación denegada
                            MessageBox.Show("La respuesta a tu invitacion es: " + answer + "\nNadie te quiere :(");
                        else
                            MessageBox.Show("Algo raro pasa aquí");
                        break;

                    case 11: //Chat (enviar mensaje del chat al formulario)
                        string test = trozos[2].Split('\0')[0];
                        idPartida = Convert.ToInt32(message);
                        formularios[idPartida].chat(test);
                        break;

                    case 12://Eliminar cuenta
                        if (message != "0")
                            MessageBox.Show("No se ha podido eliminar");
                        else
                        {
                            //Nos desconectamos
                            this.BackColor = Color.Gray;
                            MessageBox.Show("se ha eliminado de la base de datos");
                            atender.Abort();
                            server.Shutdown(SocketShutdown.Both);
                            server.Close();
                        }
                        break;

                    /*case 13: // Palabra aleatoria  
                        string RecibidaPalabra = trozos[2].Split('\0')[0];
                        idPartida = Convert.ToInt32(message);
                        formularios[idPartida].word(RecibidaPalabra);
                        break;*/

                    case 14: //Ganador de la partida. Recibe:"14/username/tiempo"
                        string[] podio = message.Split('/');
                        string ganador = podio[0];
                        MessageBox.Show("Ha ganado el jugador " + ganador);
                        formularios[idPartida].Close();
                        break;
                    case 15: //Consulta: Jugadores contra los que has jugado.
                        if (message != "0")
                        {
                            this.Invoke(new DelegadorParaEcribirRepuesta(Mensaje_Respuesta), new Object[] { message });
                        }
                        else
                        {
                            label7.Text = "No se han encontrado datos en la consulta";
                        }
                        break;
                        

                    case 16://Consulta: partidas que se han jugador en un intervalo de tiempo.
                        if (message != "0")
                        {
                            this.Invoke(new DelegadorParaEcribirRepuesta(Mensaje_Respuesta), new Object[] { message });
                        }
                        else
                        {
                            label7.Text = "No se han encontrado datos en la consulta";
                        }
                        break;
                       
                    case 17: //Consulta: ganador de una partida en concreto  
                        if (message != "0")
                        {
                            this.Invoke(new DelegadorParaEcribirRepuesta(Mensaje_Respuesta), new Object[] { message });
                        }
                        else
                        {
                            label7.Text = "No se han encontrado datos en la consulta";
                        }
                        break;
                        
                    default:
                        MessageBox.Show("Error en la peticion");
                        break;
                }
            }
        }

        //Conectar
        private void btnConectar_Click(object sender, EventArgs e)
        {
            //Conectamos un IPEendPint con el ip del servidor y el puerto del servidor al que deseamos conectarnos 
            IPAddress direc = IPAddress.Parse("192.168.56.102");
            IPEndPoint ipep = new IPEndPoint(direc, 9020);

            //Creamos el socket
            server = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
            try
            {
                server.Connect(ipep); //Intentamos conectar el socket
                //lblConectado.BackColor = Color.LightGreen;
                button1.BackColor = Color.LightGreen;
                button2.BackColor = Color.LightGreen;
                button3.BackColor = Color.LightGreen;
                button4.BackColor = Color.LightGreen;
                button8.BackColor = Color.LightGreen;
                MessageBox.Show("Conexion establecida correctamente");

                //Pongo en marcha el thread que atenderá los mensajes del servidor
                ThreadStart ts = delegate { AtenderServidor(); };
                atender = new Thread(ts);
                atender.Start();
            }
            catch (SocketException ex)
            {
                //Si hay excepcion imprimimos error y salimos del programa con return
                MessageBox.Show("No se ha podido conectar con el servidor");
                return;
            }
        }

        //Consultar peticion
        private void btnConsulta_Click(object sender, EventArgs e)
        {
            if (PartidasGanadas.Checked)
            {
                //Enviamos la petición al servidor (nºpartidasGanadas)
                string message = "2/" + username;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
            }

            else if (PuntosTotales.Checked)
            {
                //Enviamos la petición al servidor (nºPuntosGanados)
                string message = "3/" + username;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
            }

            else if (PorcentajeGanadas.Checked)
            {
                //Enviamos la petición al servidor(%PartidasGanadas)
                string message = "4/" + username;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
                server.Send(msg);
            }
            else if (JugadoresVsJugado.Checked) //Consulta: Jugadores contra los que has jugado.
            {
                //Envía: "15/username"
                string consultaJugVSSend = "15/" + username;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(consultaJugVSSend);
                server.Send(msg);
            }
            else if (PartidasIntervalo.Checked) //Consulta: partidas que se han jugador en un intervalo de tiempo.
            {
                //Envía "16/dd-mm-aa/dd-mm-aa
                string mensaje = "16/" + fechai.Text + "/" + fechaf.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje); //Envio un vector de bytes
                server.Send(msg);
            }
            else if (GanadorPartidaParticular.Checked) //Consulta: ganador de una partida en concreto  
            {
                //Envía 17/usernamejugador"
                string consultaGanadorSend = "17/" + usernameJug.Text;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(consultaGanadorSend);
                server.Send(msg);
            }
        }


        //Crear Cuenta
        private void btnCrearCuenta_Click(object sender, EventArgs e)
        {
            //Enviamos la petición al servidor
            username = Username.Text;
            string message = "0/" + username + "/" + Password.Text;
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(message);
            server.Send(msg);
        }

        //LogIn
        private void btnInicarSesion_Click(object sender, EventArgs e)
        {
            try
            {
                username = Username.Text;
                if ((username != "") && (Password.Text != ""))
                {
                    //Enviamos la petición al servidor
                    string message = "1/" + username + "/" + Password.Text;
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
        private void btnDesconectar_Click(object sender, EventArgs e)
        {
            //Enviamos la petición de desconexión
            string mensaje = "6/";
            byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
            server.Send(msg);

            //Nos desconectamos
            atender.Abort();
            //lblConectado.BackColor = Color.Tomato;
            button1.BackColor = Color.Tomato;
            button2.BackColor = Color.Tomato;
            button3.BackColor = Color.Tomato;
            button4.BackColor = Color.Tomato;
            button8.BackColor = Color.Tomato;
            server.Shutdown(SocketShutdown.Both);
            server.Close();
        }

        //Invitar a jugar
        private void btnInvitar_Click(object sender, EventArgs e)
        {
            string mensaje = "7/";
            if (numSeleccionados > 0)
            {
                for (int i = 0; i < numSeleccionados; i++)
                    mensaje = mensaje + jugadores[i] + ",";
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                server.Send(msg);
            }
            numSeleccionados = 0;
            for (int i = 0; i < 3; i++)
                jugadores[i] = "";
        }

        //Jugadores conectados
        private void ConectadosGrid_CellContentClick(object sender, DataGridViewCellEventArgs e)
        {
            if (ConectadosGrid.CurrentCell.Value != null)
            {
                string sel = ConectadosGrid.CurrentCell.Value.ToString();
                string seleccionado = sel.Split('\0')[0];
                if (seleccionado != username)
                {
                    if (numSeleccionados == 3)
                        MessageBox.Show("Maximo numero de invitados alcanzado");
                    else if ((seleccionado != jugadores[0]) && (seleccionado != jugadores[1]) && (seleccionado != jugadores[2]))
                    {
                        jugadores[numSeleccionados] = seleccionado;
                        ConectadosGrid.Rows[e.RowIndex].DefaultCellStyle.BackColor = Color.Gold;
                        numSeleccionados++;
                        MessageBox.Show("Has seleccionado para invitar a " + seleccionado);
                    }
                    else
                    {
                        bool encontrado = false;
                        for (int i = 0; i < numSeleccionados; i++)
                        {
                            if (jugadores[i] == seleccionado)
                                encontrado = true;
                            if (encontrado)
                                jugadores[i] = jugadores[i + 1];
                        }
                        numSeleccionados--;
                    }
                }
            }
        }

        //Eliminar cuenta
        private void btnEliminarCuenta_Click(object sender, EventArgs e)
        {
            if ((Username.Text != "") && (Password.Text != ""))
            {
                // El usuario se quiere dar de baja
                string mensaje = "12/" + Username.Text /*+ "/" + Password.Text*/;
                // Enviamos al servidor el nombre tecleado
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje); //Envio un vector de bytes
                server.Send(msg);
            }
            else
            {
                MessageBox.Show("No te has podido dar de baja, debes introducir un nombre y contraseña validos");
            }
        }

    }
}
