//servidor.c para la version 1 con: registro, login y consultas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//struct para la lista de conectados
typedef struct {
	char nombre[35];
	int socket;
} Conectado;

typedef struct {
	Conectado conectados[100];
	int num;
} ListaConectados;

ListaConectados lista;
//Creamos una lista de sockets
int sockets[100];
int i;

//inicio funciones para gestionar lista de conectados
int Pon (ListaConectados *lista, char nombre[35], int socket) {
	//anyade nuevos conectados
	//retorna 0 si OK, 1 si la lista esta llena
	if (lista->num == 100)
		return -1;
	else {
		strcpy (lista->conectados[lista->num].nombre, nombre);
		lista->conectados[lista->num].socket = socket;
		lista->num++;
		printf("Conectado %d \n", socket);
		return 0;
	}
}

int DamePosicion (ListaConectados *lista, char nombre[35]) {
	//devuelve la posicion o -1 si no esta en la lista
	int i=0;
	int encontrado=0;
	while ((i < lista->num) && !encontrado) {
		if (strcmp (lista->conectados[i].nombre, nombre) ==0)
			encontrado =1;
		if (!encontrado)
			i++;
	}
	if (encontrado)
		return i;
	else
		return -1;
}

int DameSocket (ListaConectados *lista, char nombre[35]) {
	//devuelve el socket o -1 si no esta en la lista
	int i=0;
	int encontrado = 0;
	while ((i < lista->num) && !encontrado)
	{
		if (strcmp(lista->conectados[i].nombre, nombre) == 0)
			encontrado = 1;
		if (!encontrado)
			i++;
	}
	
	if (encontrado)
		return lista->conectados[i].socket;
	else
		return -1;
}

int Eliminar (ListaConectados *lista, char nombre[35]) {
	//retorna 0 si elimina, -1 si no esta en la lista
	int pos = DamePosicion (lista, nombre);
	if (pos == -1)
		return -1;
	else {
		for (int i=pos; i < lista->num-1; i++)
			lista->conectados[i] = lista->conectados[i+1];
		lista->num--;
		return 0;
	}
}

void DameConectados (ListaConectados *lista, char conectados[512]) {
	//pone en conectados el nombre de todos los conectados separados por /
	//empieza con el numero de conectados, Ejemplo: "3/Juan/Maria/Pedro"
	sprintf (conectados, "%d", lista->num);
	for (int i=0; i < lista->num; i++)
		sprintf (conectados, "%s/%s", conectados, lista->conectados[i].nombre);
}
//fin funciones para gestionar la lista de conectados

//inicio funciones de utilidad (registro, login, logout)
int Registro(char usuario[35], char contrasenya[15], char respuesta[100], MYSQL *conn)
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	
	//verificar que se cumple el limite de caracteres
	if((strlen(usuario) > 35) || (strlen(contrasenya) > 15))
	{
		printf("Usuario o contrasenya demasiado largos\n");
		return -1;
	}
	
	//VERIFICAR SI EXISTE EL USUARIO
	//construir y hacer la consulta
	sprintf (consulta, "SELECT Jugador.Usuario FROM Jugador WHERE Jugador.Usuario = '%s'", usuario); 
	int err = mysql_query (conn, consulta); 
	if (err!=0) 
	{
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recoger el resultado de la consulta
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	//si NO existe el usuario
	if(row == NULL)
	{
		printf("Estas de suerte: el usuario no existe todavia\n");
		//buscar el siguiente ID 'libre'
		err=mysql_query (conn, "SELECT MAX(Jugador.ID) FROM Jugador");
		if (err!=0) {
			printf ("Error al consultar datos de la base %u %s\n",
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		resultado = mysql_store_result (conn);
		row = mysql_fetch_row (resultado);
		
		if (row == NULL) {
			printf("No se han obtenido datos en la consulta\n");
		} else {
			int ID = atoi(row[0]) + 1;
			sprintf(consulta,"INSERT INTO Jugador VALUES (%d,'%s','%s')", ID, usuario, contrasenya);
			
			//hacer la consulta
			err = mysql_query (conn, consulta);
			if (err!=0) {
				printf ("Error al introducir datos la base %u %s\n", 
						mysql_errno(conn), mysql_error(conn));
				exit (1);
			} else {
				printf("Registro correcto. Bienvenido, %s\n", usuario);
				strcpy(respuesta, "1/0");
				return 0;
			}
		}
	} else { //si SI existe el usuario
		printf("Lo siento :( El usuario %s ya existe\n", usuario);
		strcpy(respuesta, "1/-1");
	}
	return -1;
}

int LogIn(char usuario[35], char contrasenya[15], char respuesta[100], MYSQL *conn)
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	
	//confirmar si existe ese usuario con esa contrasenya
	sprintf (consulta, "SELECT Jugador.Usuario FROM Jugador WHERE Jugador.Usuario = '%s' AND Jugador.Contrasenya = '%s'", usuario, contrasenya);
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	
	//recoger el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL)
	{
		printf ("Mal! Te equivocaste en algo\n");
		strcpy(respuesta, "2/-3");
		return -1;
	} else {
		printf ("Bienvenido de nuevo, %s!\n", usuario);
		strcpy(respuesta, "2/0");
		return 0;
	}
}


//funcions para determinar si se puede anyadir al usuario a la lista de conectados
int AnyadirALista(char usuario[35], char respuesta[100], int sock_conn) 
{
	int pos = DamePosicion (&lista, usuario);
	int pon;
	
	if (pos == -1) {
		pthread_mutex_lock(&mutex);
		pon = Pon(&lista,usuario,sock_conn);
		pthread_mutex_unlock(&mutex);
		if (pon == -1) {
			printf("ERROR: lista de conectados llena; no se puede anyadir usuario\n");
			strcpy(respuesta, "2/-1");
			return -1;
		}else {
			printf("%s anyadido a la lista de conectados\n", usuario);
			strcpy(respuesta, "2/1");
			return 1;
		}
	}else {
		if (lista.conectados[pos].socket == sock_conn) {
			printf("ERROR: %s ya esta en la lista de conectados\n", usuario);
			strcpy(respuesta, "2/2");
			return 2;
		}
		printf("ERROR: %s ya habia iniciado sesion en otro cliente\n", usuario);
		strcpy(respuesta, "2/3");
		return 3;
	}	
}

int LogOut(ListaConectados *lista, char usuario[35], char respuesta[100])
{
	//borrar el cliente de la lista
	//importante que no interrumpan el proceso aqui
	pthread_mutex_lock(&mutex);
	int res = Eliminar(lista,usuario);
	pthread_mutex_unlock(&mutex);
	
	if (res == 0) {
		printf ("Se ha cerrado sesion\n");
		strcpy(respuesta, "5/1");
		return 1;
	} else if (res == -1) {
		printf ("No se habia iniciado sesion\n");
		strcpy(respuesta, "5/-1");
		return -1;
	}
}
//fin funciones de utilidad

//inicio funciones de consulta
void MuestraListaConectados(char respuesta[512])
{
	//en forma de notificacion
	char misConectados[512];
	DameConectados(&lista, misConectados);
	
	sprintf (respuesta, "7/%s", misConectados);
	printf("Conectados: %s\n",respuesta);
	for (int i=0; i < lista.num; i++)
		write (lista.conectados[i].socket, misConectados, strlen(misConectados));
}

int PartidasGanadas(char usuario[35], char respuesta[100], MYSQL *conn)
{	
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	int ganadas;
	
	strcpy(consulta,"SELECT COUNT(Partida.ID) FROM Jugador,Juego,Partida WHERE Jugador.Usuario = '");
	strcat(consulta,usuario);
	strcat(consulta,"' AND Jugador.ID = Juego.ID_J AND Juego.Id_P = Partida.ID AND Partida.Ganador = Jugador.ID;");
	
	//realizar la consulta
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	else
		printf("Consulta valida\n");
	
	//recoger el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) 
	{
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "2/fail");
	} else {
		strcpy(respuesta, "2/");
		while (row !=NULL) {
			sprintf(respuesta, "%s%s\n", respuesta, row[0]);
			printf ("\n%s, has ganado %s partidas\n", usuario, row[0]);
			ganadas = atoi(row[0]);
			if(ganadas < 3){
				printf("Podrias hacerlo mejor\n");
				return ganadas;
			} else {
				printf("Sigue asi campeon\n");
				return ganadas;
			}
			
		}
	}
	return ganadas;
}

int PuntosTotales(char usuario[35], char respuesta[100], MYSQL *conn)
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	
	strcpy(consulta, "SELECT SUM(Juego.Puntos) FROM Jugador,Juego,Partida WHERE Jugador.Usuario = '");
	strcat(consulta, usuario);
	strcat(consulta, "' AND Jugador.ID = Juego.ID_J");
	
	//realizar la consulta
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recoger el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	int puntos = atoi(row[0]);
	int puntosReales = puntos/3;
	char rowReal[5];
	sprintf(rowReal, "%d", puntosReales);
	
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "3/fail");
	} else {
		strcpy(respuesta, "3/");
		while (row !=NULL) {
			sprintf(respuesta, "%s%s\n", respuesta, rowReal);
			printf ("%s, tienes %d puntos\n", usuario, puntosReales);
			return 0;
		}
	}
	return puntos; 
}

int PorcentajePartidasGanadas(char usuario[35], char respuesta[100], MYSQL *conn)
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	float porcentaje;
	
	int ganadas = PartidasGanadas(usuario,respuesta,conn);
	
	strcpy(consulta, "SELECT COUNT(Partida.ID) FROM Jugador,Juego,Partida WHERE Jugador.Usuario = '");
	strcat(consulta, usuario);
	strcat(consulta, "' AND Jugador.ID = Juego.ID_J AND Juego.ID_P = Partida.ID;");
	
	//realizar la consulta
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recoger el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "3/fail");
	} else {
		strcpy(respuesta, "3/");
		while (row !=NULL) {
			sprintf(respuesta, "%s%s\n", respuesta, row[0]);
			int totales = atoi(row[0]);
			porcentaje = (float)ganadas/totales * 100;
			printf ("%s, has ganado el %.2f porciento\n", usuario, porcentaje);
			return (int)porcentaje;
		}
	}
	return (int)porcentaje;
}
//fin funciones de consulta

void *AtenderCliente (void *socket) 
{
	int sock_conn;
	int *s;
	s= (int *) socket;
	sock_conn= *s;
	
	int ret;
	
	char consulta [80];
	char peticion[512];
	char respuesta[512];
	char notificacion[512];
	
	MYSQL *conn;
	
	int sock_listen;
	int err;
	
	conn = mysql_init(NULL);
	//crear una conexion al servidor MYSQL 
	if (conn==NULL) {
		printf ("Error al crear la conexion myqls: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//inicializar la conexion
	conn = mysql_real_connect(conn, "localhost","root", "mysql", "BD",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion mysql: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	else
		printf("Conexion mysql valida\n");
	
	int terminar =0;
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar ==0)
	{
		printf("---------------------------------------------------\n");
		printf("Has entrado en la funcion (AtenderCliente)\n");
		// Ahora recibimos la peticion
		//sock_conn = accept(sock_listen, NULL, NULL);
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido, el socket conectado es: %d\n",sock_conn);
		
		// Tenemos que a\ufff1adirle la marca de fin de string
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		printf ("La peticion que vamos a procesar y buscar la funcion es: %s\n",peticion);
		
		char usuario[35];
		char contrasenya[15];
		
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		p = strtok( NULL, "/");
		strcpy (usuario, p);
		printf ("Codigo de peticion: %d, Usuario: %s\n", codigo, usuario);
		
		switch(codigo) 
		{
			case 0: //registro
				p=strtok(NULL,"/");
				strcpy(contrasenya,p);
				Registro(usuario,contrasenya,respuesta,conn);
				break;
			case 1: //login
				p=strtok(NULL,"/");
				strcpy(contrasenya,p);
				int check = LogIn(usuario,contrasenya,respuesta,conn);
				if (check==0) 
					AnyadirALista(usuario,respuesta,sock_conn); 
				break;
			case 2: //consulta partidas ganadas
				PartidasGanadas(usuario,respuesta,conn);
				write (sock_conn,respuesta, strlen(respuesta));
				break;
			case 3: //consulta puntos totales
				PuntosTotales(usuario,respuesta,conn);
				write (sock_conn,respuesta, strlen(respuesta));
				break;
			case 4: //consulta % partidas ganadas
				PorcentajePartidasGanadas(usuario,respuesta,conn);
				write (sock_conn,respuesta, strlen(respuesta));
				break;
			case 5: //logOut
				LogOut(&lista,usuario,respuesta);
				break;
			case 6: //desconectar
				terminar = 1;
				break;
				//7 reservado para mostrar la lista de conectados
			default:
				printf("Error en la peticion\n");
				break;
		}
		//ocasiones en que se ha modificado la lista de conectados: login, logout, desconexion
		//alguien podria desconectarse a lo bruto sin hacer logout antes
		if((codigo == 1) || (codigo == 5) || (codigo == 6))
			   MuestraListaConectados(respuesta);
	}
	//finalizar el servicio para este cliente
	mysql_close(conn); 
	close(sock_conn);
}

int main(int argc, char *argv[])
{	
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
	char peticion[512];
	char respuesta[512];
	// INICIALITZACIONS
	// Obrim el socket
	if ((sock_listen = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		printf("Error creando socket");
	// Fem el bind al port
	
	memset(&serv_adr, 0, sizeof(serv_adr));// inicialitza a zero serv_addr
	serv_adr.sin_family = AF_INET;
	
	// asocia el socket a cualquiera de las IP de la m?quina. 
	//htonl formatea el numero que recibe al formato necesario
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	// escucharemos en el port 9050
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0)
		printf ("Error al bind");
	//La cola de peticiones pendientes no podr? ser superior a 4
	else if (listen(sock_listen, 4) < 0)
		printf("Error en el Listen");
	else
	{
		//bucle infinito
		pthread_t thread;
		for(;;)
		{
			printf ("Escuchando\n");
			
			sock_conn = accept(sock_listen, NULL, NULL);
			printf ("He recibido conexion por el socket: %d\n",sock_conn);
			//sock_conn es el socket que usaremos para este cliente
			sockets[i] = sock_conn;
			
			pthread_create(&thread, NULL, AtenderCliente, &sockets[i] );
			i++;
		}
	}	
	exit(0);
}

