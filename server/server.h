
#ifndef SERVER_H
#define SERVER_H

#define APR_DECLARE_STATIC

#include <stdio.h>
#include <stdlib.h>
#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <string>
#include <assert.h>
#include <time.h>
#include <iostream>
#include <unistd.h>
#include <apr_general.h>
#include <apr_file_io.h>
#include <apr_strings.h>
#include <apr_network_io.h>

#define DEF_LISTEN_PORT	15667 						//Port que queremos escuchar
#define HOME_ADRESS "127.0.0.1"   						//Aca escribimos una IP cualquiera. Como queremos escuchar. La IP no importa para el server.
#define DEF_SOCKET_BACKLOG	SOMAXCONN  	//Cantidad maxima de conexiones que el Sistema Operativo Soporta.
#define BUFSIZE 516											//Longitud del buffer
#define SOCK_TIMEOUT (APR_USEC_PER_SEC * 5)	//Timeout (ver más adelante)
#define SECONDS_TO_WAIT 10

using namespace std;

class server {
public:
    server();
    server(const server& orig);
    virtual ~server();
    void doServerConnect(void);
    /** Función auxiliar doServTask()
    * Recibe un string del socket  serv_sock (recibido como primer parámetro) y lo imprimer por pantalla. Asume que serv_sock ya fue inicializado y la comunicación está activa.
    * En serv_sock recibe un apr_socket_t * inicializado con una conexión realizada (por ejemplo llamando a la función apr_socket_accept()).
    * En mp recibe un apr_pool_t * iniailizado que representa el memory pool necesario para el funcionamiento de las funciones de la librería APR.
    -> Este memory pool se debe haber creado previamente con apr_pool_create() y debe permanecer in scope mientras dure esta función.
    **/
    void receiveInfo(string &str);
    void sendInfo(string &str);
    apr_status_t getRV();
private:
    apr_status_t rv;   //esta variable la usamos para leer los errores y verificar cuando las cosas andan o no.
    apr_pool_t *mp;   //esta variable la conocemos del tp anterior.
    apr_socket_t *s;   //esta variable nombra un socket. El concepto de socket es central a networking y lo pueden encontrar acá: https://en.wikipedia.org/wiki/Network_socket.
    clock_t server_timer; //variable de tipo clock_t definida en time.h  donde guardaremos la cuenta de ticks.
    // Se crea una nueva variable, tipo apr_socket_t * (ns) que va a guardar la conexión que se genere con otra máquina (la otra máquia debería hacer un apr_socket_connect())
    // En otras palabras, el client si se quiere conectar a un server hace un apr_socket_connect().
    // El server primero abre un puerto para escuchar (en nuestro caso la variable s)
    // y luego con la función apr_socket_accept() escucha ese puerto y si ve que algún cliente se conectó allí
    // lo devuelve en su primer argumento (en nuestro caso ns).
    // Esto nos permite llamar varias veces a apr_socket_accept() con un arreglo de apr_socket * (por ejemplo apr_socket * ns[10]) y así
    // poder atender a varios clientes con un mismo server.
    // Se ve que la información de escucha queda guardada en la variable s y por ello es que se necesitan dos o más sockets en el server: uno que
    // identifica el puerto a escuchar (s en nuestro caso) y uno o mas que identifican las conexiones realiadas (ns en nuestro caso).
    apr_socket_t *ns;
    apr_status_t doListen();

};

#endif /* SERVER_H */
