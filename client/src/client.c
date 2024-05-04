#include "client.h"
int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();
	log_info(logger,"Hola! Soy un log");
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();
	valor = config_get_string_value(config,"CLAVE");
	ip = config_get_string_value(config,"IP");
	puerto = config_get_string_value(config,"PUERTO");
	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'

	// Loggeamos el valor de config
	log_info(logger,"Valor leido de la config: %s", valor);

	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	int32_t handshake = 1;
	int32_t result;

	send(conexion, &handshake, sizeof(int32_t), NULL);
	recv(conexion, &result, sizeof(int32_t), MSG_WAITALL);
	if (result == 0) {
    	log_info(logger,"Conexion exitosa");// Handshake OK
	} else {
   		 log_info(logger,"Eror al establecer la conexion");// Handshake ERROR
	}

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	nuevo_logger=log_create("tp0.log","tp0",true,LOG_LEVEL_INFO); 

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = NULL;//supongo para que pueda saltar el error
	nuevo_config = config_create("cliente.config");
	if (nuevo_config == NULL) {
		printf("No se pudo crear el config!");
    exit(-1);
}
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	while (1){
	// La primera te la dejo de yapa
		leido = readline("> ");
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
		if((strcmp(leido,"")) == 0){
			break;
	}
		log_info(logger,"%s",leido);
	}
	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();
	// Leemos y esta vez agregamos las lineas al paquete
	while (1)
	{
		leido = readline("> ");
		agregar_a_paquete(paquete,leido,strlen(leido)+1);
		if((strcmp(leido,"")) == 0){
			break;
	}
		printf("%s",leido);
	}
	enviar_paquete(paquete,conexion);
	eliminar_paquete(paquete);
	free(leido);

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);
	  config_destroy(config);
	  liberar_conexion(conexion);
}
