

#include "server.h"

server::server() {
    //Inicializacón de APR. Igual que en el TP3
    apr_initialize();
    apr_pool_create(&mp, NULL);
    ns=NULL;

}

server::server(const server& orig) {
}

server::~server() {
    if(ns != NULL )
        // Cuando terminamos de recibir lo que teníamos que reicibir cerramos el puerto.
        apr_socket_shutdown(ns, APR_SHUTDOWN_READWRITE);
        apr_socket_close(ns);
    //Finalmente cerramos los dos sockets. devolviendo la memoria allocada.
    apr_socket_close(s);
    apr_pool_destroy(mp);
    apr_terminate();
    usleep(5000);
	
}

void server::doServerConnect()
{
    //LLamamos a la función doListen para que inicialice el puerto para escucha. El socket s va a tener la info del puerto que se está escuchando.
    if(doListen() == APR_SUCCESS)
    {
	printf("\nnow listening ! turn on the client!");
	
	// Se toma el tiempo actual para loopear por SECONDS_TO_WAIT segundos esperando 
	// que se conecte un client. Si en ese tiempo no se conecta nadie escribimos que no encontramos cliente y chau.
	// Si se conecta alguien llamamos a la función que recibe info y la muestra en pantalla.
	server_timer = clock();
	while(((clock() - server_timer)/CLOCKS_PER_SEC)<SECONDS_TO_WAIT && ns == NULL)
	{
            rv = apr_socket_accept(&ns, s, mp);
            
	}
        if(ns)
	{
            //cout<<"entra?";
            // Una vez que conseguimos un client, cerramos el puerto de escucha (Socket s), ya que no vamos a "atender" a nadie más
            // Sin embargo la conexión con el cliente sigue viva ya que el socket ns no fue cerrado.
            apr_socket_shutdown(s, APR_SHUTDOWN_READWRITE);
	
            printf("\nCONNECTION ACCEPTED");
			
            //Una vez que nos conectamos mantenemos el Network Socket en no bloqueante 
            //Y lo pasamos a 0 segundos para cumplir con nuestra hipótesis de Event Driven Programming.
            //En el client pueden ver una descripción más detallada de estas funciones.
            apr_socket_opt_set(ns, APR_SO_NONBLOCK, 1);
            apr_socket_timeout_set(ns, 0);					    
            rv = APR_SUCCESS;
            
            
	}
	else
        {
            printf("\n NO CLIENT CONNECTED");
            rv=true;
        }
    }
    else
    {
       	printf("\ncannot start listening");
        rv=true;
    }
        

}

apr_status_t server::doListen()
{
	apr_socket_t *m;      //Este es el "Network Socket" Según el mismo link. Allí se explica claramente la diferencia entre ellos.
	apr_sockaddr_t *sa; //Esta variable es un "Socket Address" es la combinación de una IP y un puerto. Para mas detalles ver la definición de Socket Address en https://en.wikipedia.org/wiki/Network_socket.

	
	// Acá creamos el "Socket Address". La dirección a la que escribimos y el puerto. Falta todavía definir con qué protocolo...
	// El primer parámetro es el ID de Socket Address. 
	 //El segundo la IP a la que nos queremos conectar. En el caso del server, como vamos a hacer un listen, esto se ignora. Se puede poner cualquier dirección. Ponemos la loopback.
	 //El tercero se usa INET (para más info https://apr.apache.org/docs/apr/1.5/group__apr__network__io.html)
	 //El cuarto el puerto al que nos queremos conectar.
	 //El quinto flags (para más info https://apr.apache.org/docs/apr/1.5/group__apr__network__io.html).
	 //El sexto es el pool de memoria que usa APR. previamente inicializado.
	 
	rv = apr_sockaddr_info_get(&sa,HOME_ADRESS, AF_INET, DEF_LISTEN_PORT, 0, mp); 

	if (rv == APR_SUCCESS) 
	{
	
		// Con el Socket Address creado, tenemos que crear ahora el "Network Socket" (básicamente el protocolo con el que le vamos a hablar al Socket Address...)
		// El pirmer parámetro es el ID del Network Address.
		// El segundo es el protocolo a utilizar enrte IPv4 e IPV6. Si ponemos APR_INET intenta primero IPv4 si no puede va por IPv6. Si ponemos APR_INET6 hace lo inverso.
		//     Si ponemos APR_UNSPEC le decimos que nos es indistinto. Que use cualquiera de los dos.
		// El tercer parámetro refiere al modo de conexión que queremos tener. Básicamente con SOCK_STREAM tenemos una conexión hasta que del otro lado deciden cortarla.
		//    Nosotros vamos a usar siempre SOCK_STREAM. Más detalles en: http://stackoverflow.com/questions/5815675/strange-thing-sock-dgram-and-sock-stream
		// El cuarto parámetro refier al protocolo a utilizar. Nosotros vamos a usar siempre TCP en la cátedra. Por lo tanto APR_PROTO_TCP.
		// El quinto parámetro es el pool de memoria que usa APR. previamente inicializado.
		
		rv = apr_socket_create(&m,APR_INET, SOCK_STREAM, APR_PROTO_TCP, mp);
		
		
		if (rv == APR_SUCCESS) 
		{
		
			//Si pudimos crear el Address Socket y Network Socket, antes de crear la conexión le tenemos que decir al programa como utilizar el Network Socket.
			//Nosotros vamos a configurar los timeouts del Network Socket para que sea no bloqueante ya que nos interesa trabajar con Event Driven Programming
			//Para lo que necesitamos funciones no bloqueantes.
			
			//Aqui ACTIVAMOS (PONIENDO 1) la opcion APR_SO_NONBLOCK. Si pusieramos 0 seria bloqueante.
			//Esto hace que al utilizar el Socket, la función que lo utiliza no se quede esperando a que pase algo con el socket. Se fija si pasó algo y vuelve enseguida.
			apr_socket_opt_set(m, APR_SO_NONBLOCK, 1);	
			
			//Una vez que seteamos no no es bloqueante, sino que vuelva, podemos poner en cuanto tiempo. Si queremos que vuelva en forma inmediata entonces le ponemos 0.
			//Sino le ponemos el tiempo que deseemos que espere para volver. Al principio le ponemos un tiempito para establecer la conexión.
			//Una vez establecida la conexión lo pasamos a 0 para que las lecturas/ escrituras sean instantáneas y no nos cuelguen nuestro Event Driven Programming.			
			apr_socket_timeout_set(m, 0);
			
			//Si en el futuro matamos la conexion (con shutdows), podemos reusar el socket
			apr_socket_opt_set(m, APR_SO_REUSEADDR, 1);	 

			 //Una vez creado el Network Socket y el Address Socket, se llama a apr_socket_bind()
			 //Que básicamente une los dos en en Network Socket (s) para luego poder usarlo para escuchar.
			rv = apr_socket_bind(m, sa);
			
			if (rv == APR_SUCCESS) 
			{
				// En caso de que hayan unido los dos sockets correctamente
				// Llamo a la función que escucha el puerto.
				// Recuerden que en s voy a tener el socket que escucha.
				// Por cada conexión que "escuchemos" se va a crear un nuevo socket para identificar dicha conexión (seguir viendo main).
				// Voy a poder levantar hasta SOMAXCONN conexiones en simultáneo.
				rv = apr_socket_listen(m, SOMAXCONN); 
				
				// Si logré comenzar a escuchar entonces le paso al usuario el apr_socket_t * en el que estoy escuchando.
				if (rv == APR_SUCCESS) 
					s = m;
				else
					printf("\nNOT LISTENING");
			
			}
			else
				printf("\nfailed to bind socket");
			
		}
		else
			printf("\ncannot create socket");
		
	}
	else
		printf("\ncannot get info?");
        return rv;
}

void server::receiveInfo(string &str)
{
    	
	bool quit = false;					//Variable de control
	char buf[BUFSIZE];					//Un buffer para levantar el buffer de la red.
	apr_size_t len;						
		
	printf("\nSTARTING RECEPTION LOOP");
	
	//La función simplemente recibe un chunk de información de la red.
	//Está diseñada para ilustrar el funcionamiento de la función apr_socket_recv() que se utiliza para recibir información de la conexión que se haya establecido.	
	do
	{
		
		//Inicializo el primer byte del buffer en 0 para detectar si no recibo nada.
		buf[0] = 0;											
		
		//Inicializamos len en cada ciclo ya que apr_socket_recv() la modifica. Tomamos len como BUFSIZE-1 para poner en buf[BUFSIZE-1] el '\0'. 
		//Ya que no podemos asumir que nos están mandando un NULL terminated string.
		len = BUFSIZE- 1;
																
		//La función apr_socket_recv() recibe la info del sock (Network Socket inicializado) y la pone en el buffer.
		//Recibe hasta len bytes (por eso le mandamos BUFSIZE-1, para poner en el último caracter de buf el terminador).
		//A su vez, devuelve en len la cantidad de bytes recibidos.
		//Cuidado con lo comentado arriba que es importante. len es un argumento tanto de "entrada" como de "salida", ya que al llamar la función se lo toma para
		//conocer el tamaño de buf (hasta cuantos bytes puedo poner en buf) y cuando la función vuelve sirve para saber cuantos bytes de buf son realmente válidos.
		//El len como parámetro de entrada sirve para que apr_socket_recv() no genere un buffer over run. len como parámetro de salida sirve para que nosotros
		//sepamos exáctamente cuantos datos del buffer de salida son válidos.
		//En su nombre devuelve si hubo un error o no con códigos APR. No conviene comparar su return value contra APR_SUCCESS como en
		//otras funciones ya que cuando usamos esta función en el modo NONBLOCK con timeout 0, si la llamamos muy seguido nos manda un código distinto de APR_SUCCESS
		//que significa que está PENDING (que no recibió nada porque el controlador todavía no está listo para recibir). Este código no tiene un define detrás.
		//Para filtrar este tipo de códigos, la macro APR_STATUS_IS_EOF nos asegura que si devuelve FALSE no hay ningún problema con la comunicación. A su vez, para 
		//saber si recibimos algo o no utilizamos el valor de len.
		//Con estas dos variables podemos por lo tanto controlar que no haya problemas de red (APR_STATUS_IS_EOF() == false) 
		//y si recibimos algo o no (len > 0 recibimos, len == 0 no recibimos nada)
		rv = apr_socket_recv(ns, buf, &len);	
		
		buf[len] = '\0'; //Ponemos un terminador en buf[len] (len tiene la cantidad de bytes recibidos). como len <=BUFSIZE-1 no hay posibilidad de buffer overrun.
		
		//Por último se recibimos algo lo imprimimos. Sino le explicamos al usuario que no hay nada más que recibir.
		if(len>0)
			str = string(buf);
		else
			printf("\nNOTHING FURTHER TO RECEIVE");
	}
		//Asume que del otro lado nos van a mandar un chunk de información continua, y por lo tanto, cuando no haya más información a recibir (len == 0) 
		//o cuando detecte algún error de red (APR_STATUS_IS_EOF(rv) == true) termina la comunicacion (quit = true);
		while ( !(APR_STATUS_IS_EOF(rv) || (len == 0)));

}

apr_status_t server::getRV()
{
    return rv;
}

void server::sendInfo(string& str)
{
   	apr_size_t len = str.length();
	//La funcion apr_socket_send manda len bytes (3er parámetro) 
	//del buffer (req_hdr en este caso) que recibe en su segundo parámetro 
	//al socket de su primer parámetro (sock en este caso).
	//Nótese que al definir len como strlen(req_hdr) no se manda el terminador '\0' ya que se mandan del 0 al len-1 bytes (en total len bytes) y no len+1 bytes.
	//En este caso mandamos un string en ASCII, pero se ve claramente que se puede mandar cualquier chunk de bytes.
	//la función nos devuelve cuantos bytes logró enviar en len. En rigo deberíamos verificar len para asegurarnos de que se mandó todo el chunk que deseamos mandar.
	//Como se trata de un chunk extremadamente pequeño (28 bytes) en este programa de prueba se ignora el len. Correctamente se debería verificar que se haya enviado todo.
	rv = apr_socket_send(ns, str.c_str(), &len); 
        
}

bool server::isEvent(string& packet)
{
    bool isev=false;
    //Inicializo el primer byte del buffer en 0 para detectar si no recibo nada.
    char buf[BUFSIZE];	
    len = BUFSIZE-1;
    apr_socket_recv(s,buf,&len);
    buf[len] = '\0'; 
    packet=string(buf);
    if(APR_STATUS_IS_EOF(rv) || len==0)
        isev= false;
    else
        isev= true;
    return isev;
}