//servidor.c para la version 4 con: lista de partidas e invitaciones
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

//para garantizar el acceso excluyente
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
		printf("Conectado por el socket %d \n", socket);
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

int DamePosicionSegunSocket (ListaConectados *lista, int socket) {
	//Devuelve la posicion o -1 si no esta en la lista
	int i=0;
	int encontrado=0;
	while ((i < lista->num) && !encontrado) {
		if (lista->conectados[i].socket == socket)
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

int DameUsuario (ListaConectados *lista, int socket, char nombre[35]) {
	//devuelve -1 si no esta en la lista
	int i=0;
	int encontrado = 0;
	while ((i < lista->num) && !encontrado)
	{
		if (lista->conectados[i].socket == socket)
			strcpy (nombre, lista->conectados[i].nombre);
		if (!encontrado)
			i++;
	}
	
	if (encontrado)
		return 0;
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

int EliminaSegunSocket (ListaConectados *lista, int socket) {
	//Retorna 0 si elimina, -1 si no esta en la lista
	int pos = DamePosicionSegunSocket(lista, socket);
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

//struct para la tabla de partidas: invitacion metodo B (complejo)
typedef struct{
	char user1[35];
	int sock1;
	char user2[35];
	int sock2;
} Partida;

typedef Partida TablaPartidas[40];
TablaPartidas tabla;

//inicio funciones para gestionar la tabla de partidas
void InicializarTablaPartidas (TablaPartidas tabla) {
	for (int i=0; i<40;i++){
		tabla[i].sock1 = -1;
		tabla[i].sock2 = -1;
	}
}

int CrearPartida (TablaPartidas tabla, int sock1, char user2[35])
{
	//buscar la primera posicion vacia en la tabla de partidas
	int posTabla = -1;
	for (int i=0; i<40 && posTabla==-1; i++) {
		if (tabla[i].sock1 == -1)
			posTabla = i;
	}
	
	if (posTabla != -1) {
		char user1[35];
		DameUsuario(&lista,sock1, user1); 
		int sock2 = DameSocket(&lista,user2);
		
		//llenar la fila de la partida
		strcpy(tabla[posTabla].user1, user1);
		tabla[posTabla].sock1 = sock1;
		strcpy(tabla[posTabla].user2, user2);
		tabla[posTabla].sock2 = sock2;
	}
	printf("Idpartida: %d\nUser1: %s\nSock1: %d\nUser2: %s\nSock2: %d\n", posTabla,tabla[posTabla].user1,tabla[posTabla].sock1,tabla[posTabla].user2,tabla[posTabla].sock2);
	return posTabla;
}

void EliminarPartida (int IDpartida) {
	tabla[IDpartida].sock1 = -1;
	tabla[IDpartida].sock2 = -1;
}

//inicio funciones para invitaciones
void Invitar (int socket, char user2[35]) {
	char notificacion[512];
	printf("Chivato has entrado en invitar\n");
	
	pthread_mutex_lock(&mutex);
	int posPartida = CrearPartida(tabla,socket,user2);
	pthread_mutex_unlock(&mutex);
	printf("Chivato posPartida: %d\n", posPartida);
	
	printf("Civato posicion partida: %d\n", posPartida);
	if(posPartida == -1) {
		printf("No se puede crear partida\n");
		strcpy(notificacion, "7/-1");
		printf("Chivato notificacion error: %s\n", notificacion);
		//devuelve error al cliente que ha mandado la invitacion
		write (socket, notificacion, strlen(notificacion));
	}else {
		//manda al cliente invitado algo del tipo: 7/Juan/2
		//Juan te invita a jugar en la partida 2
		sprintf(notificacion, "7/%s/%d", tabla[posPartida].user1, posPartida);
		printf("Chivato notificacion bien: %s\n", notificacion);
		printf("Chivato socket del invitado: %d\n", tabla[posPartida].sock2);
		write (tabla[posPartida].sock2, notificacion, strlen(notificacion));
	}
}

void RespuestaInvitacion(int IDpartida, char invRespuesta[5]) {
	printf("Chivato has entrado en respuesta invitacion\n");
	char notificacion[512];
	int socket = tabla[IDpartida].sock1;
	printf("Chivato socket1 (anfitrion): %d\n", socket);
	sprintf(notificacion,"9/%s/%d",invRespuesta,IDpartida);
	
	printf("Chivato notificacion: %s\n", notificacion);
	//si se rechaza la invitacion, borramos la partida
	if (strcmp(invRespuesta, "no")==0) {
		EliminarPartida(IDpartida);
		write (socket, notificacion, strlen(notificacion));
	} else if (strcmp(invRespuesta, "si")==0)
		write (socket, notificacion, strlen(notificacion));
	else
		printf("Error en la respuesta a la invitacion\n");
}

void Jugada(int IDpartida, char jugada[5], int sock_conn)
{
	//char fila = jugada[0];
	//int columna = atoi(jugada[1]);
	
	char notificacion[512];
	sprintf(notificacion,"10/%d%s",IDpartida,jugada);
	if(sock_conn == tabla[IDpartida].sock1)
		write (tabla[IDpartida].sock2, notificacion, strlen(notificacion));
	else if(sock_conn == tabla[IDpartida].sock2)
		write (tabla[IDpartida].sock1, notificacion, strlen(notificacion));
}

//inicio funciones de utilidad (registro, login, logout, desconexion)
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
				strcpy(respuesta, "0/0");
				return 0;
			}
		}
	} else { //si SI existe el usuario
		printf("Lo siento :( El usuario %s ya existe\n", usuario);
		strcpy(respuesta, "0/-1");
	}
	return -1;
}

int LogIn(char usuario[35], char contrasenya[15], char respuesta[512], MYSQL *conn)
{
	printf("Chivato entra en login\n");
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[200];
	
	//confirmar si existe ese usuario con esa contrasenya
	sprintf (consulta, "SELECT Jugador.Usuario FROM Jugador WHERE Jugador.Usuario = '%s' AND Jugador.Contrasenya = '%s'", usuario, contrasenya);
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	printf("Chivato ha hecho la consulta sin problemas\n");
	
	//recoger el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	printf("Chivato recoge row: %s\n", row[0]);
	if (row == NULL)
	{
		printf ("Mal! Te equivocaste en algo\n");
		strcpy(respuesta, "1/-3");
		return -1;
	} else {
		printf ("Bienvenido de nuevo, %s!\n", usuario);
		strcpy(respuesta, "1/0");
		printf("Respuesta: %s\n", respuesta);
		return 0;
	}
}


//funcions para determinar si se puede anyadir al usuario a la lista de conectados
int AnyadirALista(char usuario[35], char respuesta[100], int sock_conn) 
{
	int pos = DamePosicion(&lista, usuario);
	int pon;
	
	if (pos == -1) {
		pthread_mutex_lock(&mutex);
		pon = Pon(&lista,usuario,sock_conn);
		pthread_mutex_unlock(&mutex);
		if (pon == -1) {
			printf("ERROR: lista de conectados llena; no se puede anyadir usuario\n");
			strcpy(respuesta, "1/-1");
			return -1;
		}else {
			printf("%s anyadido a la lista de conectados\n", usuario);
			strcpy(respuesta, "1/1");
			return 1;
		}
	}else {
		printf("ERROR: %s ya habia iniciado sesion en otro cliente\n", usuario);
		strcpy(respuesta, "1/2");
		return 2;
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

//funcion identica a logout excepto que no devuelve respuesta al cliente
//evita que haya problemas si alguien se desconecta sin antes haber hecho logout
int Desconectar(ListaConectados *lista, int sock_conn)
{
	//borrar el cliente de la lista
	//importante que no interrumpan el proceso aqui
	pthread_mutex_lock(&mutex);
	int res = EliminaSegunSocket(lista,sock_conn);
	pthread_mutex_unlock(&mutex);
	
	if (res == 0) {
		printf ("Se ha cerrado sesion\n");
		return 1;
	} else if (res == -1) {
		printf ("No se habia iniciado sesion\n");
		return -1;
	}
}
//fin funciones de utilidad

//inicio funciones de consulta
void MuestraListaConectados(char respuesta[512], int sock_conn)
{
	//pasa al cliente la lista en formato: 3/irene/angelica/alba
	char misConectados[512];
	pthread_mutex_lock( &mutex);
	DameConectados(&lista, misConectados);
	pthread_mutex_unlock( &mutex);
	
	sprintf (respuesta, "8/%s", misConectados);
	printf("Conectados: %s\n",respuesta);
	
	for (int j=0; j < lista.num; j++)
		write (lista.conectados[j].socket, respuesta, strlen(respuesta));
	
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
	printf("Chivato entra funcion puntos\n");
	
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
	
	printf("Chivato ha recogido row: %s\n", row[0]);
	
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "3/fail");
	} else {
		printf("Chivato ha entrado en row no null\n");
		
		int puntos = atoi(row[0]);
		int puntosReales = puntos/3;
		char rowReal[5];
		sprintf(rowReal, "%d", puntosReales);
		
		printf("Chivato ha pasado a integer: %d\n", puntosReales);
		
		strcpy(respuesta, "3/");
		sprintf(respuesta, "%s%s\n", respuesta, rowReal);
		printf ("%s, tienes %d puntos\n", usuario, puntosReales);
		
		return puntos;
	}
	return -1;
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
		strcpy(respuesta, "4/fail");
	} else {
		strcpy(respuesta, "4/");
		while (row !=NULL) {
			int totales = atoi(row[0]);
			porcentaje = (float)ganadas/totales * 100;
			printf ("%s, has ganado el %.2f porciento\n", usuario, porcentaje);
			
			sprintf(respuesta, "%s%d\n", respuesta, (int)porcentaje);
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
	
	char consulta[80];
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
	conn = mysql_real_connect(conn, "localhost","root", "mysql", "M4BD",0, NULL, 0);
	if (conn==NULL) {
		printf ("Error al inicializar la conexion mysql: %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	else
		printf("Conexion mysql valida\n");
	
	int terminar = 0;
	// Entramos en un bucle para atender todas las peticiones de este cliente
	//hasta que se desconecte
	while (terminar == 0)
	{
		printf("---------------------------------------------------\n");
		printf("Has entrado en la funcion (AtenderCliente)\n");
		// Ahora recibimos la peticion
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido, el socket conectado es: %d\n",sock_conn);
		
		// Tenemos que a\ufff1adirle la marca de fin de string
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		printf ("La peticion que vamos a procesar y buscar la funcion es: %s\n",peticion);
		
		char usuario[35];
		char contrasenya[15];
		
		char user2[35];
		char invRespuesta[5];
		int IDpartida;
		char jugada[5];
		
		char *p = strtok( peticion, "/");
		int codigo =  atoi (p);
		printf ("Codigo de peticion: %d\n", codigo);
		
		switch(codigo) 
		{
		case 0: //registro
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			p=strtok(NULL,"/");
			strcpy(contrasenya,p);
			Registro(usuario,contrasenya,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 1: //login
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			printf("Chivato usuario: %s\n", usuario);
			p=strtok(NULL,"/");
			strcpy(contrasenya,p);
			printf("Chivato contrasenya: %s\n", contrasenya);
			int check = LogIn(usuario,contrasenya,respuesta,conn);
			printf("Chivato respuesta login: check = %d\n", check);
			if (check==0) 
				AnyadirALista(usuario,respuesta,sock_conn);
			printf("Chivato respuesta: %s\n", respuesta);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 2: //consulta partidas ganadas
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			PartidasGanadas(usuario,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 3: //consulta puntos totales
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			PuntosTotales(usuario,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 4: //consulta % partidas ganadas
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			PorcentajePartidasGanadas(usuario,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 5: //logOut
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			LogOut(&lista,usuario,respuesta);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 6: //desconectar
			/*p = strtok( NULL, "/");
			strcpy (usuario, p);
			Desconectar(&lista,usuario);*/
			//se desconecta segun socket para que no le haga falta el username
			//asi se puede deconectar correctamente incluso si antes no ha hecho login
			//en vez de bloquearse en las lineas de codigo comentadas 
			Desconectar(&lista,sock_conn);
			terminar = 1;
			break;
		case 7: //invitacion
			p = strtok(NULL, "/");
			strcpy (user2, p);
			printf("Chivato user2: %s\n", user2);
			Invitar(sock_conn, user2);
			break;
			/*case 8:
			MuestraListaConectados(respuesta,sock_conn);
			break;*/
		case 9: //respuesta invitacion
			p = strtok( NULL, "/");
			strcpy (invRespuesta, p);
			p=strtok(NULL,"/");
			IDpartida = atoi(p);
			RespuestaInvitacion(IDpartida,invRespuesta);
			break;
		case 10: //jugada
			p=strtok(NULL,"/");
			IDpartida = atoi(p);
			p = strtok( NULL, "/");
			strcpy (jugada, p);
			Jugada(IDpartida,jugada,sock_conn);
			break;
		default:
			printf("Error en la peticion\n");
			break;
		}
		//ocasiones en que se ha modificado la lista de conectados: login, logout, desconexion
		//alguien podria desconectarse a lo bruto sin hacer logout antes
		if((codigo == 1) || (codigo == 5) || (codigo == 6))
			   MuestraListaConectados(respuesta, sock_conn);
		
	}
	//finalizar el servicio para este cliente
	mysql_close(conn); 
	close(sock_conn);
}

int main(int argc, char *argv[])
{	
	int i;
	int sock_conn, sock_listen, ret;
	struct sockaddr_in serv_adr;
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
	// estabamos en el port 9020
	serv_adr.sin_port = htons(9050);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0){
		printf ("Error al bind");
		exit(0);
	}
	//La cola de peticiones pendientes no podr? ser superior a 4
	else if (listen(sock_listen, 4) < 0) {
		printf("Error en el Listen");
		exit(0);
	}
	
	InicializarTablaPartidas(tabla);
	pthread_t thread;
	//bucle infinito
	for(;;)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion por el socket: %d\n",sock_conn);
		//sock_conn es el socket que usaremos para este cliente
		lista.conectados[i].socket  = sock_conn;
			
		pthread_create(&thread, NULL, AtenderCliente, &lista.conectados[i].socket);
		i++;
	}
	exit(0);
}
