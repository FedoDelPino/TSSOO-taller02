#include <global.h>
#include <checkArgs.hpp>

uint64_t* ArregloComun = nullptr;
uint64_t* ArregloSerial = nullptr;
uint64_t* ArregloParalelo = nullptr;
uint64_t SumaParalelo = 0;
std::vector<std::thread *> threads;
std::vector<std::thread *> threadsSumas;
std::mutex Candado;

void LLenadoArreglo(size_t Left, size_t Right, size_t RandType){
	for(size_t i = Left; i < Right; ++i){		
		// Arreglo[i] = unif(rng);
		switch(RandType){
			case 0: 
				ArregloSerial[i] = ArregloComun[i]; 
//				std::cout << "Elemento del Arreglo Serial: "<< ArregloSerial[i] << std::endl;
				break;
			case 1:
				ArregloParalelo[i] = ArregloComun[i];
//				std::cout << "Elemento del Arreglo: Paralelo"<< ArregloParalelo[i] << std::endl;
				break;
		}
	}
}
void SumadoParalelo(uint32_t Left, uint32_t Right){
	//Candado.lock();
	for(uint32_t i = Left; i < Right; i++){
		SumaParalelo += ArregloParalelo[i];
	}
	//Candado.unlock();
	//std::cout << "Suma En Paralelo: "<< SumaParalelo << std::endl;
}

int main(int argc, char** argv){
	
	uint64_t totalElementos;
	uint32_t numThreads;
	uint32_t LInferior;
	uint32_t LSuperior;
	
	auto argumentos = (std::shared_ptr<checkArgs>) new checkArgs(argc, argv);
	
	totalElementos = argumentos->getArgs().TamProblema;
	numThreads     = argumentos->getArgs().numThreads;
	LInferior	   = argumentos->getArgs().LInferior;
	LSuperior 	   = argumentos->getArgs().LSuperior;	
	std::cout << "Elementos: " << totalElementos << std::endl;
	std::cout << "Threads  : " << numThreads     << std::endl;
	std::cout << "Limite Inferior Rand: "<< LInferior << std::endl;
	std::cout << "Limite Superior Rand: "<< LSuperior << std::endl;
	
	//================Modulo LLenado================
	ArregloComun = new uint64_t[totalElementos];
	std::srand(std::time(0)); 
	std::random_device device;
	std::mt19937 rng(device());
	std::uniform_int_distribution<> unif(LInferior, LSuperior);
	for(uint32_t i = 0; i < totalElementos; i++){
		ArregloComun[i] = unif(rng);
		// std::cout << "Arreglo comun" << ArregloComun[i] << std::endl;
	}

	//======Llenado En Serie======
	ArregloSerial = new uint64_t[totalElementos];
	auto start = std::chrono::high_resolution_clock::now();
	LLenadoArreglo(0,totalElementos,0);
	auto end     = std::chrono::high_resolution_clock::now(); 
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeS = elapsed.count();

	//======Llenado En Paralelo======
	ArregloParalelo = new uint64_t[totalElementos];
	for (size_t i= 0; i < numThreads ; i++){
		threads.push_back(new std::thread(LLenadoArreglo, i*(totalElementos)/numThreads, (i+1)*(totalElementos)/numThreads,1));
	}
	start = std::chrono::high_resolution_clock::now();
	for(auto &thLlenado : threads){
		thLlenado->join();
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeP = elapsed.count();
	
	//================Modulo Sumado================

	//======Sumado En Serie======
	uint64_t SumaSerial=0;
	start     = std::chrono::high_resolution_clock::now(); 
	for (size_t i=0;i<totalElementos;i++){
		SumaSerial += ArregloSerial[i];
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSumaS = elapsed.count();
	std::cout <<"Suma En Serie: " <<SumaSerial << std::endl;	
	//======Sumado En Paralelo======
	//SumaParalelo = new uint64_t[totalElementos];
	for (size_t i=0;i<numThreads;i++){
		threadsSumas.push_back(new std::thread(SumadoParalelo, i*(totalElementos)/numThreads,(i+1)*(totalElementos)/numThreads));
	}
	start     = std::chrono::high_resolution_clock::now();
	for(auto &thS : threadsSumas){
		thS->join();
	}
	end     = std::chrono::high_resolution_clock::now(); 
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	auto totalTimeSumaP = elapsed.count();
	std::cout << "Suma Total en Paralelo: " << SumaParalelo << "\n" << std::endl;
	std::cout << "==========Tiempos de Llenado==========" << std::endl;
	std::cout << "TiempoSerial :"  << totalTimeS << "[ms]" << std::endl;
	std::cout << "TiempoParalelo:"  << totalTimeP << "[ms]" << std::endl;
	std::cout << "Desempeño entre S y P = " << (double)totalTimeS/totalTimeP << "\n" <<std::endl;
	std::cout << "==========Tiempos de Sumado==========" << std::endl;
	std::cout << "TiempoSerial :"  << totalTimeSumaS << "[ms]" << std::endl;
	std::cout << "TiempoParalelo:"  << totalTimeSumaP << "[ms]" << std::endl;
	std::cout << "Desempeño entre S y P = " << (double)totalTimeSumaS/totalTimeSumaP <<std::endl;
	return(EXIT_SUCCESS);
}
