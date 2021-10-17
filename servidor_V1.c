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

int Registro(char usuario[35], char contrasenya[15], char respuesta[100], MYSQL *conn, int sock_con)
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
}

int LogIn(char usuario[35], char contrasenya[15], char respuesta[100], MYSQL *conn, int sock_conn)
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

int LogOut(char respuesta[100], MYSQL *conn, int sock_conn)
{
	write(sock_conn,respuesta,strlen(respuesta));
	close(sock_conn);  
	//necesario pera que el cliente detecte EOF 
	//cerrar la conexion con el servidor MYSQL
	mysql_close (conn);
	return 0;
}

int PartidasGanadas(char usuario[35], char respuesta[100], MYSQL *conn, int sock_conn)
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
				printf("Podrias hacelo mejor\n");
				return ganadas;
			} else {
				printf("Sigue asi campeon\n");
				return ganadas;
			}
			
		}
	}
	return ganadas;
}

int PuntosTotales(char usuario[35], char respuesta[100], MYSQL *conn, int sock_conn)
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

int PorcentajePartidasGanadas(char usuario[35], char respuesta[100], MYSQL *conn, int sock_conn)
{
	MYSQL_RES *resultado;
	MYSQL_ROW row;
	char consulta[300];
	float porcentaje;
	
	int ganadas = PartidasGanadas(usuario,respuesta,conn,sock_conn);
	
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
	if (listen(sock_listen, 4) < 0)
		printf("Error en el Listen");
	
	char usuario[35];
	char contrasenya[15];
	int codigo;
	
	int i;
	// Atenderemos solo 5 peticione
	for(i=0;i<5;i++)
	{
		printf ("Escuchando\n");
		
		sock_conn = accept(sock_listen, NULL, NULL);
		printf ("He recibido conexion\n");
		//sock_conn es el socket que usaremos para este cliente
		
		// Ahora recibimos su nombre, que dejamos en buff
		ret=read(sock_conn,peticion, sizeof(peticion));
		printf ("Recibido\n");
		
		// Tenemos que a?adirle la marca de fin de string 
		// para que no escriba lo que hay despues en el buffer
		peticion[ret]='\0';
		
		//Escribimos el nombre en la consola
		printf ("Se ha conectado por socket con la peticion: %s\n",peticion);
		
		char *p = strtok( peticion, "/");
		codigo =  atoi (p);
		p = strtok( NULL, "/");
		strcpy (usuario, p);
		printf ("Codigo de peticion: %d, Usuario: %s\n", codigo, usuario);
		
		MYSQL *conn;
		int err;
		
		conn = mysql_init(NULL);
		//crear una conexion al servidor MYSQL 
		if (conn==NULL) 
		{
			printf ("Error al crear la conexion myqls: %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		//inicializar la conexion
		conn = mysql_real_connect(conn, "localhost","root", "mysql", "BD",0, NULL, 0);
		if (conn==NULL) 
		{
			printf ("Error al inicializar la conexion mysql: %u %s\n", mysql_errno(conn), mysql_error(conn));
			exit (1);
		}
		else
			printf("Conexion mysql valida\n");
		
		switch(codigo) 
		{
		case 0: //registro
			p=strtok(NULL,"/");
			strcpy(contrasenya,p);
			Registro(usuario,contrasenya,respuesta,conn,sock_conn);
			break;
		case 1: //login
			p=strtok(NULL,"/");
			strcpy(contrasenya,p);
			LogIn(usuario,contrasenya,respuesta,conn,sock_conn);
			break;
		case 2: //consulta partidas ganadas
			PartidasGanadas(usuario,respuesta,conn,sock_conn);
			write (sock_conn,respuesta, strlen(respuesta));
			break;
		case 3: //consulta puntos totales
			PuntosTotales(usuario,respuesta,conn,sock_conn);
			write (sock_conn,respuesta, strlen(respuesta));
			break;
		case 4: //consulta % partidas ganadas
			PorcentajePartidasGanadas(usuario,respuesta,conn,sock_conn);
			write (sock_conn,respuesta, strlen(respuesta));
			break;
		case 5: //logOut
			LogOut(respuesta,conn,sock_conn);
			//final = 1;
			break;
		default:
			printf("Error en la peticion\n");
			break;
		}
		mysql_close(conn);
		
	}
	close(sock_conn);
	exit(0);
}

