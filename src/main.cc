#include <global.h>
#include <checkArgs.hpp>
#include <future>

std::vector<uint32_t> ArrElementos;

int main(int argc, char **argv)
{

	uint64_t totalElementos;
	uint32_t numThreads;
	int lInf;
	int LSup;

	uint32_t sumaSerial = 0;
	//uint32_t sumaParcial = 0;
	uint32_t sumaThreads = 0;
	std::vector<std::thread *> threads;
	std::vector<uint32_t> ArrElementos;
	std::vector<uint32_t> SumasParciales;

	auto argumentos = (std::shared_ptr<checkArgs>)new checkArgs(argc, argv);

	totalElementos = argumentos->getArgs().tamProblema;
	numThreads = argumentos->getArgs().numThreads;
	lInf = argumentos->getArgs().lInf;
	LSup = argumentos->getArgs().LSup;

	static thread_local std::random_device device;
	static thread_local std::mt19937 rng(device());
	std::uniform_int_distribution<uint32_t> nRandom(lInf, LSup);

	//	std::cout << "Elementos: " << totalElementos << std::endl;
	//	std::cout << "Threads  : " << numThreads << std::endl;
	//	std::cout << "Limite Inferior  : " << lInf << std::endl;
	//	std::cout << "Limite Superior  : " << LSup << std::endl;
	for (size_t i = 0; i < totalElementos; i++)
	{
		ArrElementos.push_back(nRandom(rng));
		SumasParciales.push_back(0);
	}

	//auto LlenadoArreglo = [](uint32_t totalElementos, std::vector<uint32_t> ArrElementos) {
	//	for (size_t i = 0; i < totalElementos; i++)
	//	{
	//		ArrElementos.push_back(i);
	//	}
	//	return (ArrElementos);
	//};

	//for (size_t i = 0; i < numThreads; ++i)
	//{
	//	threads.push_back(new std::thread(LlenadoArreglo, std::ref(totalElementos), std::ref(ArrElementos)));
	//}

	//for (auto &th : threads)
	//{
	//	th->join();
	//}

	//======SERIAL======
	for (auto &num : ArrElementos)
	{
		sumaSerial += num;
	}

	auto sumaParcial = [](std::vector<uint32_t> &ArrElementos, std::vector<uint32_t> &SumasParciales, size_t pos, size_t Left, size_t Right) {
		for (size_t i = Left; i < Right; ++i)
		{
			SumasParciales[pos] += ArrElementos[i];
		}
		return SumasParciales;
	};

	//======THREADS======
	//(1) Separación del trabajo
	//for (size_t i = 0; i < numThreads; i++)
	//{
	//	auto th = std::async(std::launch::async, sumaParcial, std::ref(ArrElementos), 0, ArrElementos.size() / 2);
	//}
	//auto t1 = std::async(std::launch::async, sumaParcial, std::ref(ArrElementos), 0, ArrElementos.size() / 2);
	//auto t2 = std::async(std::launch::async, sumaParcial, std::ref(ArrElementos), ArrElementos.size() / 2, ArrElementos.size());

	//sumaParcial1 = t1.get();
	//sumaParcial2 = t2.get();

	for (size_t i = 0; i < numThreads; ++i)
	{
		threads.push_back(new std::thread(sumaParcial,
										  std::ref(ArrElementos), std::ref(SumasParciales), i,
										  i * (totalElementos) / numThreads,
										  (i + 1) * (totalElementos) / numThreads));
	}
	for (auto &th : threads)
	{
		th->join();
	}
	//(2) Reducción (Consolidación de resultados parciales)
	for (auto &sumaTh : SumasParciales)
	{
		std::cout << "Suma de Parciales: " << sumaTh << std::endl;
		std::cout << "Arreglo en i: " << sumaTh << std::endl;
		sumaThreads += sumaTh;
	}

	//======RESULTADOS=====
	std::cout << "====Serial====" << std::endl;
	std::cout << "Suma Serial:" << sumaSerial << std::endl;

	std::cout << "====Threads====" << std::endl;
	std::cout << "sumaHilos: " << sumaThreads << std::endl;

	return (EXIT_SUCCESS);
}
