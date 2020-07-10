#ifndef _CHECKARGS_H_
#define _CHECKARGS_H_

#include <global.h>
#include <unistd.h>

extern char *optarg;
extern int optind, opterr, optopt;

class checkArgs {
private:
	// 1) Modificar esta sección
	const std::string optString = "N:t:l:L:h";
	
	const std::string opciones = "-N Tam_problema -t threads -l Límite Inferior -L Límite Superior [-h]";

	const std::string descripcion  = "Descripción:\n"
		                             "\t-N   Tamaño del Arreglo de Elementos\n"
									 "\t-t   Cantidad de Threads a utilizar\n"
									 "\t-l   Límite Inferior para Generar Números Aleatorios\n"
									 "\t-L   Límite Superior para Generar Números Aleatorios\n"
									 "\t-h   Muestra esta ayuda y termina\n";
	
	typedef struct args_t{
		uint64_t TamProblema;
		uint32_t numThreads;
		uint32_t LInferior;
		uint32_t LSuperior;
	} args_t;
	
	// 2) Modificar constructor
	// 3) Modificar ciclo "getopt" en método checkArgs::getArgs()
	// 4) Recuerde que para compilar nuevamente, y por tratarse
	//    de un archivo header, debe hacer primero un make clean
	
	args_t  parametros;
	
	int argc;
	char **argv;

	
public:
	checkArgs(int _argc , char **_argv);
	~checkArgs();
	args_t getArgs();
	
private:
	void printUsage();
	
	
};

checkArgs::checkArgs(int _argc , char **_argv){
	parametros.TamProblema = 0;
	parametros.numThreads  = 0;
	parametros.LInferior   = 0;
	parametros.LSuperior   = 0;

	argc = _argc;
	argv = _argv;
	
}

checkArgs::~checkArgs(){
	
}

checkArgs::args_t checkArgs::getArgs(){
	int opcion;
	
	while ((opcion = getopt (argc, argv, optString.c_str())) != -1){
		switch (opcion) {
			case 'N':
					parametros.TamProblema = atoi(optarg);
					break;
			case 't':
					parametros.numThreads = atoi(optarg);
					break;
			case 'l':
					parametros.LInferior = atoi(optarg);
					break;
			case 'L':
					parametros.LSuperior = atoi(optarg);
					break;
			case 'h':
			default:
					printUsage();
					exit(EXIT_FAILURE);
		}
	}

	if ( parametros.TamProblema == 0 ||
		 parametros.numThreads  == 0 ||
		 parametros.LSuperior   == 0 ){
		printUsage();
		exit(EXIT_FAILURE);
	}
	
	return(parametros);
}

void checkArgs::printUsage(){
	std::cout << "Uso: " << argv[0] << " " << opciones << " " << descripcion << std::endl;
}



#endif