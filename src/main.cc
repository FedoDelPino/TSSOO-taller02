#include <global.h>
#include <checkArgs.hpp>

int main(int argc, char **argv)
{
	std::mutex Candado;

	uint64_t totalElementos;
	uint32_t numThreads;
	int lInf;
	int LSup;

	uint32_t sumaSerial = 0;
	uint32_t sumaThreads = 0;
	std::vector<std::thread *> threads;
	std::vector<uint32_t> ArrElementos;
	std::vector<uint32_t> SumasParciales;

	//Entrada de Argumentos
	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	lInf = argumentos->getArgs().lInf;
	LSup = argumentos->getArgs().LSup;

	//Generador de Numeros Aleatorios en cuanto a los Limites
	static thread_local std::random_device device;
	static thread_local std::mt19937 rng(device());
	std::uniform_int_distribution<uint32_t> nRandom(lInf, LSup);

	if (totalElementos < numThreads)
	{
		std::cout << "Por favor, corroborar que el tamaño del arreglo sea igual o mayor que los Hilos creados para realizar la correcta suma" << std::endl;
		exit(EXIT_FAILURE);
	}
	//Posible Solucion: Agregar la condicional dentro del proceso de suma, para luego ajustar los rangos de corte y pasar un elemento a cada hilo, arreglar mas adelante
	//Analizar, creo que lo tome bien, paso innecesario
	for (size_t i = 0; i < totalElementos; i++)
	{
		Candado.lock();
		ArrElementos.push_back(nRandom(rng));
		SumasParciales.push_back(0);
		Candado.unlock();
	}

	//======SERIAL======
	for (auto &num : ArrElementos)
	{
		Candado.lock();
		sumaSerial += num;
		Candado.unlock();
	}

	auto sumaParcial = [](std::vector<uint32_t> &ArrElementos, std::vector<uint32_t> &SumasParciales, size_t posArr, size_t Left, size_t Right) {
		for (size_t i = Left; i < Right; ++i)
		{
			SumasParciales[posArr] += ArrElementos[i];
		}
	};

	for (size_t i = 0; i < numThreads; ++i)
	{
		Candado.lock();
		threads.push_back(new std::thread(sumaParcial, std::ref(ArrElementos), std::ref(SumasParciales), i, i * (totalElementos) / numThreads, (i + 1) * (totalElementos) / numThreads));
		Candado.unlock();
	}
	for (auto &th : threads)
	{
		th->join();
	}
	//(2) Reducción (Consolidación de resultados parciales)
	for (auto &sumaTh : SumasParciales)
	{
		Candado.lock();
		sumaThreads += sumaTh;
		Candado.unlock();
	}

	//======RESULTADOS=====
	std::cout << "====Serial====" << std::endl;
	std::cout << "Suma Serial:" << sumaSerial << std::endl;

	std::cout << "====Threads====" << std::endl;
	std::cout << "sumaHilos: " << sumaThreads << std::endl;

	return (EXIT_SUCCESS);
}
