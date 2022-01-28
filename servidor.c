//servidor.c para arreglar el desastre de la v4 v5
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
#include <time.h>

//PARA GARANTIZAR EL ACCESO EXCLUYENTE
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//STRUCT PARA LA LISTA DE CONECTADOS
typedef struct {
	char nombre[35];
	int socket;
} Conectado;

typedef struct {
	Conectado conectados[100];
	int num;
} ListaConectados;

ListaConectados lista;

//INICIO FUNCIONES PARA GESTIONAR LISTA DE CONECTADOS

/* anyade nuevos conectados, retorna 0 si OK, 1 si la lista esta llena */
int Pon (ListaConectados *lista, char nombre[35], int socket) {
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

/* a partir del nombre, devuelve la posicion o -1 si no esta en la lista */
int DamePosicion (ListaConectados *lista, char nombre[35]) {
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

/* a partir del socket, devuelve la posicion o -1 si no esta en la lista */
int DamePosicionSegunSocket (ListaConectados *lista, int socket) {
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

/* devuelve el socket o -1 si no esta en la lista */
int DameSocket (ListaConectados *lista, char nombre[35]) {
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

/* a partir del socket, devuelve el nombre y 0 o -1 si no esta en la lista */
int DameUsuario (ListaConectados *lista, int socket, char nombre[35]) {
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

/* a partir del nombre, elimina de conectados y retorna 0 o -1 si no esta en la lista */
int Eliminar (ListaConectados *lista, char nombre[35]) {
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

/* a partir del socket, elimina de conectados y retorna 0 o -1 si no esta en la lista */
int EliminaSegunSocket (ListaConectados *lista, int socket) {
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

/* pone en conectados el nombre de todos los conectados separados por /
empieza con el numero de conectados, Ejemplo: "3/Juan/Maria/Pedro" */
void DameConectados (ListaConectados *lista, char conectados[512]) {
	sprintf (conectados, "%d", lista->num);
	for (int i=0; i < lista->num; i++)
		sprintf (conectados, "%s/%s", conectados, lista->conectados[i].nombre);
}

/* actualiza automaticamente la lista de conectados; la devuelve al cliente como notificacion */
void MuestraListaConectados(char respuesta[512], int sock_conn) {
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
//FIN DE FUNCIONES PARA GESTIONAR LA LISTA DE CONECTADOS

//STRUCT PARA LA TABLA DE PARTIDAS
typedef struct{
	Conectado jugadoresPartida[4]; //lista con los jugadores, tienen nombre y socket
	int numJugadores;
	int respuesta[4]; //si=1, no=0
	int numRespuesta;
	int tiempo[4]; 	//lo que tarda cada jugador en completar la partida
	//vale -1 si el jugador ha perdido
	int finalizados; //numero de jugadores que han terminado la partida
} Partida;

typedef Partida TablaPartidas[40];
TablaPartidas tabla;

//INICIO FUNCIONES PARA GESTIONAR LA TABLA DE PARTIDAS

/* pone el primer socket de todas las partidas a -1
inicializa el numero de jugadores y el numero de respuestas a 0 */
void InicializarTablaPartidas (TablaPartidas tabla) {
	for (int i=0; i<40;i++){
		tabla[i].jugadoresPartida[0].socket = -1;
		tabla[i].numJugadores = 0;
		tabla[i].numRespuesta = 0;
		tabla[i].finalizados = 0;
	}
}

/* vuelve a poner socket0 de una partida a -1 */
void EliminarPartida (int IDpartida) {
	tabla[IDpartida].jugadoresPartida[0].socket = -1;
	tabla[IDpartida].numJugadores = 0;
	tabla[IDpartida].numRespuesta = 0;
	tabla[IDpartida].finalizados = 0;
}

/* ocupa la primera posicion libre en la lista de partidas e introduce datos de:
anfitrion, jugadores, numero de jugadores. devuelve la posicion de la partida en la tabla */
int CrearPartida (TablaPartidas tabla, int sock_conn, char jugadores[300]) {
	//buscar la primera posicion vacia en la tabla de partidas
	int posTabla = -1;
	for (int i=0; i<40 && posTabla==-1; i++) {
		if (tabla[i].jugadoresPartida[0].socket == -1)
			posTabla = i;
	}
	
	if (posTabla != -1) {
		//buscar y llenar los datos del anfitrion
		char anfitrion[35];
		DameUsuario(&lista,sock_conn, anfitrion); 
		strcpy(tabla[posTabla].jugadoresPartida[0].nombre, anfitrion);
		tabla[posTabla].jugadoresPartida[0].socket = sock_conn;
		tabla[posTabla].numJugadores++;
		tabla[posTabla].respuesta[0] = 1;
		tabla[posTabla].numRespuesta++;
		printf("CREADA PARTIDA %d\n\tAnfition: %s con respuesta: %d (esperado valor 1)\n\tNumJugadores: %d y numRespuesta: %d (esperados valores 1)\n", posTabla,anfitrion,tabla[posTabla].respuesta[0],tabla[posTabla].numJugadores,tabla[posTabla].numRespuesta);
		
		int j = 1;
		int socket;
		char *p=strtok(jugadores,",");
		while(p!=NULL) {
			printf("Nombre invitado%d: %s\n", j,p);
			socket = DameSocket(&lista,p);
			if (socket != -1) {
				strcpy(tabla[posTabla].jugadoresPartida[j].nombre, p);
				tabla[posTabla].jugadoresPartida[j].socket = socket;
				tabla[posTabla].numJugadores++;
				printf("Anyadido a la partida: usuario %s con socket %d\n", 
					   tabla[posTabla].jugadoresPartida[j].nombre,tabla[posTabla].jugadoresPartida[j].socket);
			}
			else return -1; //el nombre del usuario que esta procesando no esta en la lista
			j++;
			p = strtok(NULL, ",");
		}
		printf("Creada partida %d con %d jugadores invitados\n", posTabla, tabla[posTabla].numJugadores-1);
		if(tabla[posTabla].numJugadores < 2)
			EliminarPartida(posTabla);
	}
	return posTabla;
}

/* devuelve string de sockets de los participantes de una partida */
void DameSocketsJugadores(int IDpartida, char sockets[50]) {
	for(int i=0; i < tabla[IDpartida].numJugadores; i++)
		sprintf(sockets,"%s,%d",sockets,tabla[IDpartida].jugadoresPartida[i].socket);
}

/* para cuando un jugador se desconecta en medio de una partida/s
lo elimina de cada partida que estubiera jugando y notifica a los demas */
void SeRetira (char jugador[35]) {
	char notificacion[512];
	
	//buscar todas las partidas que esta jugando el usuario que se ha desconectado
	for (int i=0; i<40; i++) {
		int abandonos = 0;
		for(int j=0; j < tabla[i].numJugadores; j++) {
			if(strcmp(tabla[i].jugadoresPartida[j].nombre,jugador)==0) {
				sprintf(notificacion,"15/%d/%s", i, jugador);
				//eliminar el jugador de la partida
				tabla[i].jugadoresPartida[j].socket = -1;
				strcpy(tabla[i].jugadoresPartida[j].nombre,"\0");
				//write (tabla[i].jugadoresPartida[k].socket,notificacion, strlen(notificacion));
			}
		}
		//verificar si para la partida actual aun quedan jgadores activos
		for(int k=0; k < tabla[i].numJugadores; k++) {
			if (tabla[i].jugadoresPartida[k].socket = -1)
				abandonos++;
		}
		/* si ya no quedan jugadores activos, eliminar la partida
		se considera que todos los jugadores se han desconectado sin haber finalizado */
		if(abandonos = tabla[i].numJugadores)
			EliminarPartida(i);
	}
}
//FIN DE FUNCIONES PARA GESTIONAR LA TABLA DE PARTIDAS

//INICIO FUNCIONES PARA GESTIONAR INVITACIONES

/* crea partida y manda invitaciones hacia los sockets de los clientes invitados */
void Invitar (int socket, char invitados[300]) {
	printf("Chivato has entrado en invitar\n");
	
	char jugadores[300];
	char notificacion[512];
	strcpy(jugadores,invitados);
	
	pthread_mutex_lock(&mutex);
	int IDpartida = CrearPartida(tabla,socket,jugadores);
	pthread_mutex_unlock(&mutex);
	printf("Chivato IDpartida: %d\n", IDpartida);
	
	if (IDpartida == -1) {
		printf("No se ha podido crear partida\n");
		strcpy(notificacion,"7/-1");
		printf("Chivato notificacion error: %s\n", notificacion);
		//devuelve error al cliente que ha mandado la invitacion
		write (socket, notificacion, strlen(notificacion));
	}
	else if (tabla[IDpartida].numJugadores < 2)
	{
		printf("No se han invitado a suficientes jugadores\n");
		strcpy(notificacion,"7/-2");
		printf("Chivato notificacion error: %s\n", notificacion);
		//devuelve error al cliente que ha mandado la invitacion
		write (socket, notificacion, strlen(notificacion));
	}
	else {
		//manda a los clientes invitados notificacion del tipo: 7/anfitrion/IDpartida
		sprintf(notificacion,"7/%s/%d",tabla[IDpartida].jugadoresPartida[0].nombre,IDpartida);
		printf("Chivato notificacion bien: %s\n",notificacion);
		for(int i=1; i < tabla[IDpartida].numJugadores; i++){
			write (tabla[IDpartida].jugadoresPartida[i].socket,notificacion, strlen(notificacion));
		}
	}
}

/* anyade la respuesta a la invitacion y devuelve numero de respuestas */
int AnyadirRespuesta(TablaPartidas *Tabla, int respuesta, int IDpartida, char nombre[35]) {
	
	int i=0;
	int encontrado=0;		
	//buscar la posicion del jugador dentro de la lista
	while(encontrado==0){
		if (strcmp(tabla[IDpartida].jugadoresPartida[i].nombre,nombre)==0)
			encontrado=1;
		else
			i++;
	}
	//anyadir respuesta a la lista
	tabla[IDpartida].respuesta[i]=respuesta;
	tabla[IDpartida].numRespuesta++;
	
	return tabla[IDpartida].numRespuesta;
}

/* eliminar de la partida a los jugadores que rechazaron la invitacion */
void EliminarJugadoresPartida(int IDpartida) {
	printf("Chivato entrando en eliminar jugadores: numero de jugadores = %d\n",tabla[IDpartida].numJugadores);
	int del=0;
	for (int j=0; j < tabla[IDpartida].numJugadores; j++) {
		printf("Chivato para j=%d: respuesta de %s es: %d\n",j,tabla[IDpartida].jugadoresPartida[j].nombre,tabla[IDpartida].respuesta[j]);
		if(tabla[IDpartida].respuesta[j]==0) {
			printf("Jugador %s ha rechazado invitacion (respuesta = %d). Eliminando de la partida %d...\n",tabla[IDpartida].jugadoresPartida[j].nombre,tabla[IDpartida].respuesta[j],IDpartida);
			tabla[IDpartida].jugadoresPartida[j].socket = -1;
			strcpy(tabla[IDpartida].jugadoresPartida[j].nombre,"\0");
			del++;
		}
	}
	tabla[IDpartida].numJugadores -= del;
	printf("Nuevo numero de jugadores para la partida %d: %d jugadores (anfitrion + invitados)\n",IDpartida, tabla[IDpartida].numJugadores);
}

/* devuelve a anfitrion respuesta a su invitacion
formato tipo 9/respuesta/IDpartida de parte de cada uno de los invitados */
void RespuestaInvitacion(int socket, int IDpartida, char invRespuesta[5]) {
	printf("Chivato has entrado en respuesta invitacion\nRespuesta recibida: %s\n", invRespuesta);
	char notificacion[512];
	int respuesta;
	char invitado[35];
	DameUsuario(&lista,socket,invitado);
	
	int sockAnfitrion = tabla[IDpartida].jugadoresPartida[0].socket;
	printf("Chivato socket anfitrion: %d\n", sockAnfitrion);
	
	//si se rechaza la invitacion, borramos la partida
	if (strcmp(invRespuesta, "no")==0)
		respuesta = 0;
	else if (strcmp(invRespuesta, "si")==0)
		respuesta = 1;
	else
		printf("Error en la respuesta a la invitacion\n");
	printf("Respuesta de %s: %s = %d\n", invitado,invRespuesta,respuesta);
	
	int res = AnyadirRespuesta(&tabla,respuesta,IDpartida,invitado);
	printf("Chivato numRespuesta para partida%d: %d\n", IDpartida,res);
	
	//cuando se han recibido ya todas las respuestas eliminar los que han denegado
	//mirar si se puede iniciar la partida o no y devolver a anfitrion respuesta
	if (res == tabla[IDpartida].numJugadores) {
		printf("Ya tenemos todas las respuestas a invitaciones\n");
		EliminarJugadoresPartida(IDpartida);
		printf("Chivato para partida%d tras respuesta a invitaciones: numJugadores = %d\n",IDpartida,tabla[IDpartida].numJugadores);
		//si hay un minimo de jugadores (2) que quieran jugar, se devuelve un si
		if(tabla[IDpartida].numJugadores > 1) {
			sprintf(notificacion,"9/si/%d",IDpartida);
			//enviar respuesta solo a los jugadores que han aceptado + anfitrion
			for (int i=0; i<4; i++) {
				if (tabla[IDpartida].jugadoresPartida[i].socket != -1)
					write (tabla[IDpartida].jugadoresPartida[i].socket, notificacion, strlen(notificacion));
			}
		}
		//si no se puede iniciar partida, devolver un no y borrar partida
		else {
			EliminarPartida(IDpartida);
			sprintf(notificacion,"9/no/%d",IDpartida);
			write (sockAnfitrion, notificacion, strlen(notificacion));
		}
	}
}

/* chat entre jugadores de una misma partida
pasamos el mensaje como notificacion hacia todos los sockets menos el que envia */
void Mensaje (int IDpartida, char mensajeRecibido[200], int sock_conn) {
	char notificacion[512];
	char jugador[35];
	DameUsuario(&lista,sock_conn,jugador);
	printf("%s dice: %s\n",jugador,mensajeRecibido);
	
	sprintf (notificacion, "11/%d/%s:%s", IDpartida,jugador,mensajeRecibido);
	for(int i=0; i<tabla[IDpartida].numJugadores; i++) {
		if(tabla[IDpartida].jugadoresPartida[i].socket != -1)
			write (tabla[IDpartida].jugadoresPartida[i].socket,notificacion, strlen(notificacion));
	}
}
//FIN FUNCIONES PARA GESTIONAR LAS INVITACIONES

//INICIO FUNCIONES PARA JUGAR LA PARTIDA

/* gestiona el tiempo de los ganadores y selecciona el que ha tardado menos de entre ellos
devuelve este ganador de ganadores y su tiempo a todos los jugadores de la partida */
void EstadoPartida(int IDpartida, char jugador[35], int estado, int tiempo, MYSQL *conn) {
	char state[20];
	char notificacion[512];
	if(estado == 1)
		strcpy(state,"ganador");
	else
		strcpy(state,"perdedor");
	printf("Chivato para Estado Partida %d:\n%s es un %s y ha tardado %d segundos\n", IDpartida,jugador,state,tiempo);
	
	for (int i=0; i<tabla[IDpartida].numJugadores; i++) {
		//para aquellos que hayan ganado, guardar el tiempo que han tardado
		if ((strcmp(tabla[IDpartida].jugadoresPartida[i].nombre,jugador)==0)) {
			if (estado == 1)
				tabla[IDpartida].tiempo[i] = tiempo;
			else
				tabla[IDpartida].tiempo[i] = -1;
			tabla[IDpartida].finalizados++;
		}
	}
	
	int jugadoresActivos = 0;
	for (int l=0; l<tabla[IDpartida].numJugadores; l++) {
		if (tabla[IDpartida].jugadoresPartida[l].socket != -1)
			jugadoresActivos++;
	}
	
	printf("Chivato jugadores en la partida: %d\nChivato jugadores que han terminado: %d\n",jugadoresActivos,tabla[IDpartida].finalizados);
	//cuando todos los jugadores de la partida ya han terminado
	if(jugadoresActivos == tabla[IDpartida].finalizados) {
		int tiempomin = 100000000;
		char ganador[35];
		//inicializacion por defecto por si nadie ha ganado
		strcpy(ganador,"No hay ganador");
		for (int j=0; j<tabla[IDpartida].numJugadores; j++) {
			//si el jugador habia aceptado la invitacion
			if (tabla[IDpartida].jugadoresPartida[j].socket != -1) {
				//si el jugador habia ganado
				if (tabla[IDpartida].tiempo[j] > 0) {
					//comparar su tiempo con el tiempo minimo actual
					if (tabla[IDpartida].tiempo[j] < tiempomin) {
						tiempomin = tabla[IDpartida].tiempo[j];
						strcpy(ganador,tabla[IDpartida].jugadoresPartida[j].nombre);
					}
				}
			}
		}
		printf("Chivato ganador definitivo: %s, que ha tardado %d segundos\n",ganador,tiempomin);
		sprintf(notificacion,"14/%s/%d",ganador,tiempomin);
		for (int k=0; k<tabla[IDpartida].numJugadores; k++) {
			if (tabla[IDpartida].jugadoresPartida[k].socket != -1)
				write (tabla[IDpartida].jugadoresPartida[k].socket,notificacion, strlen(notificacion));
		} 
		
		GuardarPartidaBD(ganador,tiempomin,IDpartida,conn);
	}
}
//FIN DE FUNCIONES PARA JUGAR LA PARTIDA

//INICIO FUNCIONES DE UTILIDAD (registro, login, logout, desconexion)

/* carga usuario en la base de datos, devuelve 0 si se ha podido o -1 si hay algun problema */
int Registro(char usuario[35], char contrasenya[15], char respuesta[512], MYSQL *conn) {
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

/* da de alta al usuario si ya esta cargado en la base de datos
devuelve 0 si se ha podido o -1 si hay algun problema */
int LogIn(char usuario[35], char contrasenya[15], char respuesta[512], MYSQL *conn) {
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

/* funcions para determinar si se puede anyadir al usuario a la lista de conectados
si el login es correcto se verifica que se pueda anyadir a la lista de conectados
devuelve 0 si se ha podido o valor negativo si no */
int AnyadirALista(char usuario[35], char respuesta[512], int sock_conn) {
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
			return 0;
		}
	}else {
		printf("ERROR: %s ya habia iniciado sesion en otro cliente\n", usuario);
		strcpy(respuesta, "1/2");
		return 2;
	}	
}

/* borrar el cliente de la lista, devuelve 0 si se ha podido */
int LogOut(ListaConectados *lista, char usuario[35], char respuesta[100]) {
	//importante que no interrumpan el proceso aqui
	pthread_mutex_lock(&mutex);
	int res = Eliminar(lista,usuario);
	pthread_mutex_unlock(&mutex);
	
	if (res == 0) {
		printf ("Se ha cerrado sesion\n");
		strcpy(respuesta, "5/1");
		return 0;
	} else if (res == -1) {
		printf ("No se habia iniciado sesion\n");
		strcpy(respuesta, "5/-1");
		return -1;
	}
}

/* da de baja el usuario en la base de datos y lo quita de la lista de conectados */
int EliminarUsuario (char usuario[35], MYSQL *conn, int sock_conn) {
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	char respuesta[512];
	sprintf (consulta,"DELETE FROM Jugador WHERE Jugador.Usuario = '%s';",usuario);
	
	int err=mysql_query (conn, consulta);
	if (err!=0){
		
		printf ("Error al consultar datos de la base %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result (conn);
	printf ("Usuario %s eliminado de la base de datos\n", usuario);
	
	pthread_mutex_lock (&mutex);
	Eliminar (&lista,usuario);
	pthread_mutex_unlock (&mutex);
	strcpy(respuesta,"12/0");
	write (sock_conn,respuesta, strlen(respuesta));
	return 0;
}

/* funcion identica a logout excepto que no devuelve respuesta al cliente
evita que haya problemas si alguien se desconecta sin antes haber hecho logout */
int Desconectar(ListaConectados *lista, int sock_conn) {
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
//FIN FUNCIONES DE UTILIDAD

//INICIO FUNCIONES DE GESTION DE LA BASE DE DATOS

/* consulta para devolver el ganador de una partida (el mas rapido) */
void GanadorRapido (char respuesta[100], MYSQL *conn, int sock_conn){
	//devolver el ganador: el mas rapido de una partida
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	//buscar la jugador cuya duracion de partida sea la menor
	int err=mysql_query (conn, "SELECT Partida.Ganador FROM Partida WHERE Partida.Duracion = (SELECT MIN(Partida.Duracion) FROM Partida)");
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemor el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "18/-1");
	} else {
		sprintf(respuesta, "18/%s", row[0]);
		printf("%s\n", respuesta);
	}
}

/* consulta para la base de datos; devuelve el # de partidas ganadas */
int PartidasGanadas(char usuario[35], char respuesta[512], MYSQL *conn) {	
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	int ganadas;
	
	strcpy(consulta,"SELECT COUNT(Partida.ID) FROM Jugador,Juego,Partida WHERE Jugador.Usuario = '");
	strcat(consulta,usuario);
	strcat(consulta,"' AND Jugador.ID = Juego.ID_J AND Juego.ID_P = Partida.ID AND Partida.Ganador = Jugador.Usuario;");
	
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

/* consulta para la base de datos; devuelve el #puntos o -1 */
int PuntosTotales(char usuario[35], char respuesta[512], MYSQL *conn) {
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

/* consulta para la base de datos, devuelve el % de partidas ganadas */
int PorcentajePartidasGanadas(char usuario[35], char respuesta[512], MYSQL *conn) {
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

/* una vez finalizada la partida, se guardan los datos en la BD */
int GuardarPartidaBD (char ganador[35], int duracion, int IDpartida, MYSQL *conn) {
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[512];
	int puntos = 0;
	
	//buscar la siguiente posicion a llenar
	sprintf (consulta, "SELECT MAX(Partida.ID) FROM Partida");
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemos el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL)
		printf ("No se han obtenido datos en la consulta\n");
	else {
		int ID = atoi (row[0]) + 1;
		printf("Chivato siguiente ID partida a llenar el base de datos: %d\n", ID);
		char fecha[150];
		int cont = 0;
		time_t tiempo = time(0);
		struct tm *tlocal=localtime(&tiempo);
		strftime(fecha,150,"%d-%m-20%y %H:%M",tlocal);
		//crear la consulta para guardar la partida
		sprintf (consulta, "INSERT INTO Partida VALUES (%d,'%s',%d,'%s')", ID, fecha, duracion, ganador);
		printf("Chivato consulta a BD: %s\n", consulta);
		
		//realizar la consulta
		err = mysql_query(conn, consulta);
		if (err!=0) {
			printf ("Error al introducir datos la base %u %s\n", 
					mysql_errno(conn), mysql_error(conn));
			exit (1);
		} else {
			printf("Chivato partida anyadida correctamente\n");
			//para todos los jugadores de la partida
			for (int i=0; i<tabla[IDpartida].numJugadores; i++) {
				//si la han terminado, sea ganada o perdida
				if (tabla[IDpartida].jugadoresPartida[i].socket != -1) {
					//buscar el ID a partir del nombre de usuario
					sprintf (consulta, "SELECT Jugador.ID FROM Jugador WHERE Jugador.Usuario = '%s'", tabla[IDpartida].jugadoresPartida[i].nombre);
					int err=mysql_query (conn, consulta);
					if (err!=0) {
						printf ("Error al consultar datos de la base %u %s\n",mysql_errno(conn), mysql_error(conn));
						exit (1);
					}
					resultado = mysql_store_result (conn);
					row = mysql_fetch_row (resultado);
					if (row == NULL) {
						printf ("No se han obtenido datos en la consulta\n");
					} else {
						int IDjugadorBD = atoi(row[0]);
						printf("IDjugadorBD: %d\n", IDjugadorBD);
						if (strcmp(tabla[IDpartida].jugadoresPartida[i].nombre,ganador)==0)
							puntos = 10;
						//consulta para relacionar las tabla: insertar datos en juego
						sprintf (consulta, "INSERT INTO Juego VALUES (%d,%d,%d)",IDjugadorBD, ID, puntos);
						printf("Chivato consulta para anyadir a juego: %s\n", consulta);
						//anyadir a la BD
						err = mysql_query(conn, consulta);
						if (err!=0) {
							printf ("Error al introducir datos la base %u %s\n", 
									mysql_errno(conn), mysql_error(conn));
							exit (1);
						} else {
							printf("Se ha anyadido la relacion correctamente\n");
							cont++;
						}
					}
					//cuando ya se hayan guardado todos los jugadores que han terminado
					if (tabla[IDpartida].finalizados == cont) {
						printf("Chivato ya se han guardado todos los jugadores activos\n");
						return 0;
					}
				}
			}
		}
	}
}

/* consulta que devuelve los jugadores contra los que ha jugado un usuario */
void JugadoresBDContra (char usuario[35], char respuesta[100], MYSQL *conn){
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[512];
	
	//buscar los jugadores de una partida a partir del identificador
	sprintf (consulta, "SELECT DISTINCT Jugador.Usuario FROM (Jugador, Partida, Juego) WHERE Partida.ID IN (SELECT Partida.ID FROM (Jugador, Partida, Juego) WHERE Jugador.Usuario = '%s' AND Jugador.ID = Juego.ID_J AND Partida.ID = Juego.ID_P) AND Jugador.ID = Juego.ID_J AND Partida.ID = Juego.ID_P AND Jugador.Usuario != '%s'", usuario, usuario);
	
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemor el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "15/fail");
	} else {
		strcpy(respuesta, "15/");
		while (row !=NULL) {
			printf("%s\n", row[0]);
			sprintf(respuesta, "%s%s,", respuesta, row[0]);
			row = mysql_fetch_row (resultado);
		}
		printf("Chivato respuesta a la peticion jugadores contra: %s\n", respuesta);
	}
}

/* consulta que devuelve las partidas jugadas en un intervalo de tiempo concreto */
void IntervaloFecha (char fechai[50], char fechaf[50], char respuesta[100], MYSQL *conn) {
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[100];
	int counter=0;
	int linea;
	
	sprintf(consulta,"SELECT Partida.ID, Partida.Fecha FROM (Partida)");
	int err = mysql_query(conn, consulta);
	if (err!=0){
		printf ("Error al consultar la base de datos %u %s\n", mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	resultado = mysql_store_result(conn);
	row = mysql_fetch_row(resultado);
	if (row == NULL)
	{
		strcpy(respuesta,"16/fail");
		printf("No se han obtenido datos en la primera consulta\n");
	}
	else {
		strcpy(respuesta,"16/");
		//vamos comparando fechas con el rango que ha proporcionado el cliente
		while (row != NULL) {
			char *p=strtok(row[1],"-");
			int fechadia= atoi(p);
			p = strtok(NULL,"-");
			int fechames= atoi(p);
			p = strtok(NULL,"-");
			int fechanyo=atoi(p);
			int fechapartida = fechadia+(fechames*30)+(fechanyo*365);
			if ((fechapartida>=fechai) && ( fechapartida<=fechaf)){
				sprintf(respuesta,"%s%s,%d-%d-%d/\n",respuesta,row[0],fechadia,fechames,fechanyo);
			}				
			row = mysql_fetch_row (resultado);
		}
	}
	printf("Chivato respuesta consulta partidas entre fechas: %s\n",respuesta);
}

/* consulta para devolver informacion de todas las partidas que ha jugado un usuario */
void ConsultaPartidas (char jugador[35], char respuesta[100], MYSQL *conn){
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	//buscar el ganador de una partida a partir del identificador
	sprintf (consulta, "SELECT * FROM (Partida,Jugador,Juego) WHERE Jugador.Usuario='%s' AND Jugador.ID=Juego.ID_J AND Juego.ID_P=Partida.ID",jugador);
	strcpy(respuesta,"17/");
	int err=mysql_query (conn, consulta);
	if (err!=0) {
		printf ("Error al consultar datos de la base %u %s\n",
				mysql_errno(conn), mysql_error(conn));
		exit (1);
	}
	//recogemr el resultado de la consulta 
	resultado = mysql_store_result (conn);
	row = mysql_fetch_row (resultado);
	if (row == NULL) {
		printf ("No se han obtenido datos en la consulta\n");
		strcpy(respuesta, "17/fail");
	} else {
		sprintf(respuesta,"%sPartida %s, el jugador %s gano y tardo %s segundos\n",respuesta, row[0], row[3], row[2]);
		printf("Chivato respuesta peticion ganador: %s\n", respuesta);
	}
}
//FIN FUNCIONES DE GESTION DE LA BASE DE DATOS

//FUNCION QUE MANEJA EL THREAD DE UN CLIENTE Y ATIENDE SUS PETICIONES HASTA QUE SE DESCONECTA
void *AtenderCliente (void *socket) {
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
	conn = mysql_real_connect(conn, "localhost","root", "mysql", "MG04BD",0, NULL, 0);
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
		
		char invitados[300];
		char invRespuesta[5];
		int IDpartida;
		char mensajeRecibido[200];
		char fecha1[50];
		char fecha2[50];
		char jugadorCon[35];
		
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
			printf("Chivato usuario: %s\n", respuesta);
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
			/*se desconecta segun socket para que no le haga falta el username
			y poder deconectar correctamente aunque antes no se haya hecho login*/
			SeRetira(usuario);
			Desconectar(&lista,sock_conn);
			terminar = 1;
			break;
		case 7: //invitacion; espera algo del tipo 7/invitado1,invitado2
			p = strtok(NULL, "/");
			strcpy (invitados, p);
			printf("Chivato invitados: %s\n", invitados);
			Invitar(sock_conn, invitados);
			break;
		case 9: //respuesta invitacion; espera algo del tipo 9/siOno/IDpartida
			p = strtok( NULL, "/");
			strcpy (invRespuesta, p);
			p=strtok(NULL,"/");
			IDpartida = atoi(p);
			printf("Para partida %d, respuesta invitacion: %s\n", IDpartida,invRespuesta);
			RespuestaInvitacion(sock_conn,IDpartida,invRespuesta);
			break;
		case 11: //chat
			p=strtok(NULL,"/");
			IDpartida = atoi(p);
			p = strtok(NULL, "/");
			strcpy (mensajeRecibido, p);
			printf("Para partida %d, mensaje recibido: %s\n", IDpartida,mensajeRecibido);
			Mensaje(IDpartida,mensajeRecibido,sock_conn);
			printf("Chivato salgo del case 11: chat\n");
			break;
		case 12: //eliminar usuario de BD
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			printf("andes de eliminar\n");
			EliminarUsuario(usuario, conn, sock_conn);
			printf("despues de eliminar antes de Seretira\n");
			SeRetira(usuario);
			printf("andes de desconectar\n");
			Desconectar(&lista,sock_conn);
			printf("despues de desconectar\n");
			terminar = 1;
			break;
		/*case 13: //peticion para generar palabra oculta: 13/IDpartida
			p = strtok(NULL, "/");
			IDpartida = atoi(p);
			printf("Chivato palabraGenerada = %d\n", tabla[IDpartida].palabraGenerada);
			GenerarPalabra(IDpartida,sock_conn);
			break;*/
		case 14: //estatus jugadores al finalizar partida: 14/IDpartida/jugador/estado/tiempo
			p = strtok(NULL, "/");
			IDpartida = atoi(p);
			char jugador[35];
			p = strtok(NULL, "/");
			strcpy(jugador,p);
			p = strtok(NULL, "/");
			int estado = atoi(p);
			p = strtok(NULL, "/");
			int tiempo = atoi(p);
			EstadoPartida(IDpartida,jugador,estado,tiempo,conn);
			break;
		case 15: //consulta jugadores contra, recibe 15/usuario
			p = strtok( NULL, "/");
			strcpy (usuario, p);
			JugadoresBDContra(usuario,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 16: //partidas intervalo de tiempo, recibe 16/fechainicial/fechafinal
			p = strtok(NULL, "/");
			strcpy (fecha1, p);
			p = strtok(NULL, "/");
			strcpy (fecha2, p);
			IntervaloFecha(fecha1,fecha2,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		case 17: //consulta para saber el ganador de una partida concreta: 17/jugador
			p=strtok(NULL,"/");
			strcpy(jugadorCon,p);
			ConsultaPartidas(jugadorCon,respuesta,conn);
			write(sock_conn,respuesta, strlen(respuesta));
			break;
		default:
			printf("Error en la peticion\n");
			break;
		}
		//ocasiones en que se ha modificado la lista de conectados: login, logout, desconexion
		//alguien podria desconectarse a lo bruto sin hacer logout antes
		if((codigo == 1) || (codigo == 5) || (codigo == 6) || (codigo == 12))
			   MuestraListaConectados(respuesta, sock_conn);
		
	}
	//finalizar el servicio para este cliente
	mysql_close(conn); 
	close(sock_conn);
}

//FUNCION PRINCIPAL CON CONEXION AL SOCKET Y MANEJO DEL BUCLE DE THREADS DE CLIENTES
int main(int argc, char *argv[]) {	
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
	serv_adr.sin_port = htons(9020);
	if (bind(sock_listen, (struct sockaddr *) &serv_adr, sizeof(serv_adr)) < 0){
		printf ("Error al bind\n");
		exit(0);
	}
	//La cola de peticiones pendientes no podra ser superior a 4
	else if (listen(sock_listen, 4) < 0) {
		printf("Error en el Listen\n");
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
