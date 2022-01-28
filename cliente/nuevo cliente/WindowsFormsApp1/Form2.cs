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
using System.Threading;
using System.Media;
using System.Diagnostics;
using System.IO;

namespace WindowsFormsApp1
{
    public partial class Form2 : Form
    {
        // guarda los caracteres de las letras actuales
        List<Label> labels = new List<Label>();
        //recibimos palabra aleatoria del servidor 
        public string PalabraOculta { get; set; }
        // Default character for hidden word letters
        public string DefaultChar { get { return "__"; } }

        
        //string PalabraOculta;

        //Variables globales
        int idPartida;
        Socket server;
        string username;
        string[] jugadores = new string[4]; //añadir a la lista de invitados
        
        //partes del cuerpo de Bob
        List<string> estados = new List<string> { "Nada", "Hazta", "Cabeza", "ManoIzquierda", "ManoDerecha", "PiernaIzquierda", "PiernaDerecha" };
        int estadoActual; //útima parte de Bob dibujada
        int conteo; //contador para usar el timer
        

        public Form2(int idPartida, Socket server, string username)
        {
            InitializeComponent();
            CheckForIllegalCrossThreadCalls = false;
            addBotones();

            this.BackColor = Color.Azure;

            //Hacamos invisibles las partes del cuerpo de Bob
            manoderecha.Visible = false;
            manoizquierda.Visible = false;
            cabeza.Visible = false;
            piederecho.Visible = false;
            pieizquierdo.Visible = false;
            cuerpo.Visible = false;
            hazta.Visible = true;
            
            this.idPartida = idPartida;
            this.server = server;
            this.username = username;
            conteo = 0; //inicializamos a 0 el contador del timer
        }

        //Teclado
        private void addBotones()
        {
            List<string> abecedario = new List<string> { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "Ñ", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z" };
            foreach (var item in abecedario)
            {
                Button letras = new Button();
                letras.Text = item;
                letras.BackColor = Color.LightGray;
                letras.Click += letras_Click;
                letras.Size = new Size(40, 40);
                panelTeclado.Controls.Add(letras);
            }
            panelTeclado.Enabled = false;
        }

        public void close() 
        {
            this.close();
        }
        //Evalúa si las letras seleccionadas existen o no en la palabra
        void letras_Click(object sender, EventArgs e)
        {
            Button letras = (Button)sender;
            char charClicked = letras.Text.ToCharArray()[0];
            letras.Enabled = false;

            if ((PalabraOculta = PalabraOculta.ToUpper()).Contains(charClicked))
            {
                //La letra está en la palabra
                lblMensaje.Text = "ES CORRECTO!";
                lblMensaje.ForeColor = Color.Green;
                char[] charArray = PalabraOculta.ToCharArray();
                for (int i = 0; i < PalabraOculta.Length; i++)
                {
                    if (charArray[i] == charClicked)
                        labels[i].Text = charClicked.ToString();
                    ((Button)sender).BackColor = Color.LightGreen;
                }

                //Acierto de todas las letras de la palabras             
                if (labels.Where(x => x.Text.Equals(DefaultChar)).Any())
                    return;
                lblMensaje.ForeColor = Color.Peru;
                lblMensaje.Text = "HAS GANADO";
                panelTeclado.Enabled = false;
                timer1.Enabled = false; //Detiene timer. Si va cambiar por timer1.Stop();

                //Envía al servidor: 14/IDpartida/jugador/1/tiempo
                string podio = "14/" + idPartida + "/" + username + "/" + "1" + "/" + conteo;
                byte[] msg = System.Text.Encoding.ASCII.GetBytes(podio);
                server.Send(msg);
            }
            else
            {
                //La letra no está en la palabra
                lblMensaje.Text = "MAL";
                lblMensaje.ForeColor = Color.Tomato;
                estadoActual += 1;
                DibujaBob(estadoActual);
                ((Button)sender).BackColor = Color.PaleVioletRed;
                panel1.Invalidate();

                //Se han cometido 6 errores
                if (estadoActual == 6)
                {
                    timer1.Enabled = false; //Detiene timer. Si cambiara por timer1.Stop();
                    lblMensaje.Text = "PERDISTE";
                    lblMensaje.ForeColor = Color.Tomato;
                    panelTeclado.Enabled = false;

                    //Revela la palabra oculta
                    for (int i = 0; i < PalabraOculta.Length; i++)
                    {
                        if (labels[i].Text.Equals(DefaultChar))
                        {
                            labels[i].Text = PalabraOculta[i].ToString();
                            labels[i].ForeColor = Color.Black;
                        }
                    }

                    //Envía al servidor: 14/IdPartida/jugador/0/tiempo
                    string podio = "14/" + idPartida + "/" + username + "/" + "0" + "/"  + conteo;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(podio);
                    server.Send(msg);
                }
            }
        }

        //funcion para cambiar de estado e ir dibujando a Bob
        public void DibujaBob(int estadoActual)
        {
            switch (estadoActual)
            {
                case 0:
                    hazta.Visible = true;
                    break;
                case 1:
                    cabeza.Visible = true;
                    break;
                case 2:
                    cuerpo.Visible = true;
                    break;
                case 3:
                    manoizquierda.Visible = true;
                    break;
                case 4:
                    manoderecha.Visible = true;
                    break;
                case 5:
                    piederecho.Visible = true;
                    break;
                case 6:
                    pieizquierdo.Visible = true;
                    break;

                default:
                    MessageBox.Show("Bob se ha roto");
                    break;
            }
        }

        //START: envia al servidor para tener de vuelta la palabra
        private void btnStart_Click(object sender, EventArgs e)
        {
            Boxjug.Text = username.ToString();
            timer1.Enabled = true;//inicia el timer. si funciona canviar codi per timer1.Start();

            Limpiar();
            SeleccionarPalabra();
            AddLabels();
        }

        //Buscamos palabra aleatoria
        private void SeleccionarPalabra()
        {
            //lugar donde guardamos la lista de palabras
            string fichero = Path.Combine(Path.GetDirectoryName(System.Reflection.Assembly.GetExecutingAssembly().Location), "Words.txt");
            using (TextReader tr = new StreamReader(fichero, Encoding.ASCII))
            {
                Random r = new Random();
                var listaPalabras = tr.ReadToEnd().Split(new[] { '\n', '\r' }, StringSplitOptions.RemoveEmptyEntries);
                PalabraOculta= listaPalabras[r.Next(0, listaPalabras.Length - 1)];
            }
        }

        private void Limpiar()
        {
            panelTeclado.Controls.Clear();
            addBotones();
            lblMensaje.Text = "";
            panelTeclado.Enabled = true;
        }

        private void AddLabels()
        {
            // Adding word to labels and labels to group Box
            Box1.Controls.Clear();
            labels.Clear();
            // pasar por parametro palabra del servidor 
            if (PalabraOculta == null)
                MessageBox.Show(PalabraOculta);
            else
            {

                char[] palabraCaracteres = PalabraOculta.ToCharArray();
                int NumLetters = palabraCaracteres.Length;
                int refer = Box1.Width / NumLetters;

                for (int i = 0; i < NumLetters; i++)
                {
                    Label l = new Label();
                    l.Text = DefaultChar;
                    l.Location = new Point(10 + i * refer, Box1.Height - 30);
                    l.Parent = Box1;
                    l.BringToFront();
                    labels.Add(l);
                }

                //Escribimos en label para saber numero de letras 
                NumLetrasBox.Text = NumLetters.ToString();
            }
        }


      
        //Botón enviar mensaje chat
        private void btnEnviarChat_Click(object sender, EventArgs e)
        {
            try
            {
                if (textChat.Text != null)
                {
                    string mensaje = "11/" + idPartida + "/" + textChat.Text;
                    byte[] msg = System.Text.Encoding.ASCII.GetBytes(mensaje);
                    server.Send(msg);
                }
            }
            catch
            {
                MessageBox.Show("Error al enviar mensaje");
            }
        }
        //Insertamos mesaje en el chat
        public void chat(string mensaje)
        {
            ChatBox.Items.Insert(0, mensaje);
        }
         
        //Timer
        private void timer1_Tick(object sender, EventArgs e)
        {
            conteo++;
            lblValor.Text = conteo.ToString();
        }

        //Instrucciones
        private void btnInstrucciones_Click(object sender, EventArgs e)
        {
            Process proceso = new Process();
            proceso.StartInfo.FileName = @"C:\Share\instrucciones.txt";
            proceso.Start();
        }

    }
}