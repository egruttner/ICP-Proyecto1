#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <cmath>

using namespace std;
using namespace std::chrono;


// Variables Globales
vector<int> arr;
ofstream secuencialOutputFile;
ofstream paralelo_1OutputFile;
ofstream paralelo_2OutputFile;
bool flag_mide_threads;
string directorio = "";

int cantidad_experimientos=1;




// 1.- Suma de prefijos de forma secuencial
void sumaSecuencial(vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        vec[i] += vec[i - 1]; // Operar directamente en vec
    }
}


// 2.- (usada por sumaParalela_1) Suma de prefijos con rango
void prefixSum(vector<int>& vec, int start, int end) {
    for (int i = start + 1; i < end; ++i) {
        vec[i] += vec[i - 1];
    }
}

// 2.- Suma de prefijos usando paralelización (threads)
void sumaParalela_1(vector<int>& vec, int numThreads) {

    int size = vec.size();
    int blockSize = (size + numThreads - 1) / numThreads; // Ajuste del tamaño del bloque

    vector<thread> threads;

    // Calculamos la suma de prefijos en paralelo
    for (int i = 0; i < numThreads; ++i) {
        int start = i * blockSize;
        int end = min((i + 1) * blockSize, size);

        threads.push_back(thread(prefixSum, ref(vec), start, end));
    }

    // Esperamos a que todas las hebras terminen
    for (auto& t : threads) {
        t.join();
    }

    // Hacemos el ajuste final para garantizar que el cálculo de la suma de prefijos sea correcto
    for (int i = 1; i < numThreads; ++i) {
        int start = i * blockSize;
        int end = min((i + 1) * blockSize, size);
        vec[start] += vec[start - 1]; // Ajuste del primer elemento del bloque
        for (int j = start + 1; j < end; ++j) {
            vec[j] += vec[start - 1]; // Usamos el valor del último elemento del bloque anterior
        }
    }

}


// 3.- (usado por sumaParalela_2) Permite el cálculo de acuerdo al algoritmo de Hillis/Steele
void hillis_steele_step(vector<int>& data, const vector<int>& temp, int step, int start, int end) {
    for (int i = start; i < end; ++i) {
        if (i >= step) {
            data[i] = temp[i] + temp[i - step];
        } else {
            data[i] = temp[i];
        }
    }
}

// 3.- Suma de prefijos usando el algoritmo de Hillis/Steele
void sumaParalela_2(vector<int>& vec, int numThreads) {

    int n = vec.size();
    int max_steps = ceil(log2(n));
    vector<thread> threads(numThreads);

    for (int s = 0; s < max_steps; ++s) {

        int step = 1 << s; // Compute 2^s for the current step
        vector<int> temp(vec); // Create a copy of data for current step
        for (int t = 0; t < numThreads; ++t) {
            int start = t * n / numThreads;
            int end = (t + 1) == numThreads ? n : (t + 1) * n / numThreads;
            threads[t] = thread(hillis_steele_step, ref(vec), cref(temp), step, start, end);
        }

        // Join threads after each step
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
}


//BLOQUE DE PROCESAMIENTO Y MEDICIÓN DE TIEMPO

//PROCESA SECUENCIAL
void procesaSecuencial(int tam)
{
    // Calculando la suma de prefijos secuencialmente
    double mm_total_time = 0;
    for(int j = 0; j < cantidad_experimientos; j++){ 
        auto start_time_seq = high_resolution_clock::now();
        sumaSecuencial(arr);
        auto stop_time_seq = high_resolution_clock::now();
        auto duration_seq = duration_cast<nanoseconds>(stop_time_seq - start_time_seq);

        mm_total_time += duration_seq.count();
    }
    double mm_avg_time = mm_total_time / cantidad_experimientos;

    secuencialOutputFile << tam << "," << mm_avg_time << endl;
}

//PROCESA PARALELO 1
void procesaParalelo_1(int tam, int cant_hebras)
{
    // Calculando la suma de prefijos opción Paralela 1
    double mm_total_time = 0;
    for(int j = 0; j < cantidad_experimientos; j++){ 
        auto start_time_seq = high_resolution_clock::now();
        sumaParalela_1(arr, cant_hebras);
        auto stop_time_seq = high_resolution_clock::now();
        auto duration_seq = duration_cast<nanoseconds>(stop_time_seq - start_time_seq);

        mm_total_time += duration_seq.count();
    }
    double mm_avg_time = mm_total_time / cantidad_experimientos;

    if (flag_mide_threads)
    {
        paralelo_1OutputFile << cant_hebras << "," << mm_avg_time << endl;
    }
    else
    {
        paralelo_1OutputFile << tam << "," << mm_avg_time << endl;
    }

}

//PROCESA PARALELO 2
void procesaParalelo_2(int tam, int cant_hebras)
{

  // Calculando la suma de prefijos opción Paralela 2
    double mm_total_time = 0;
    for(int j = 0; j < cantidad_experimientos; j++){ 
        auto start_time_seq = high_resolution_clock::now();
        sumaParalela_2(arr, cant_hebras);
        auto stop_time_seq = high_resolution_clock::now();
        auto duration_seq = duration_cast<nanoseconds>(stop_time_seq - start_time_seq);

        mm_total_time += duration_seq.count();
    }
    double mm_avg_time = mm_total_time / cantidad_experimientos;

    paralelo_2OutputFile << tam << "," << mm_avg_time << endl;

}

void inicializar_array(int tam)
{
    arr.resize(tam);
    for (int i = 0; i < tam; ++i) {
        arr[i] = i + 1;
    }
}


int main() {

    vector<int> experimentos = {1,2};

    vector<int> tams = {16,32,64,128,
                        256,512,1024,
                        2048,4096,8192,
                        16384,32768,65536,
                        131072,262144,524288,
                        1048576,2097152,
                        4194304};


    int hebra = 0;
    int tam=0;
    flag_mide_threads=false;

    //EXPERIMENTOS
    /*
    1 - 16 hebras, todos los tamaños
    2 - Tamaño 4194304, considera hasta 32 threads
    */

    for (const int experimento : experimentos) 
    {
        switch (experimento)
        {
        //10 HEBRAS, todos los tamaños
        case 1:
            hebra=16;
            directorio="1";

            //PROCESA PARALELO1
            paralelo_1OutputFile.open("./csv/" + directorio + "/sum_paralela_1.csv");
            paralelo_1OutputFile << "n,tiempo[ns]" << endl;
            for (const int tam : tams) {
                //PROCESA SECUENCIAL
                inicializar_array(tam);
                procesaParalelo_1(tam, hebra);
            }
            paralelo_1OutputFile.close();

            //PROCESA SECUENCIAL
            secuencialOutputFile.open("./csv/" + directorio + "/sum_secuencial.csv");
            secuencialOutputFile << "n,tiempo[ns]" << endl;
            for (const int tam : tams) {
                //PROCESA SECUENCIAL
                inicializar_array(tam);
                procesaSecuencial(tam);
            }
            secuencialOutputFile.close();

            //PROCESA PARALELO2
            paralelo_2OutputFile.open("./csv/" + directorio + "/sum_paralela_2.csv");
            paralelo_2OutputFile << "n,tiempo[ns]" << endl;
            for (const int tam : tams) {
                //PROCESA SECUENCIAL
                inicializar_array(tam);
                procesaParalelo_2(tam, hebra);
            }
            paralelo_2OutputFile.close();

            break;

        case 2:
            tam = 4194304;
            directorio="2";
            flag_mide_threads=true;

             //PROCESA PARALELO1
            paralelo_1OutputFile.open("./csv/" + directorio + "/sum_paralela_1.csv");
            paralelo_1OutputFile << "n,tiempo[ns]" << endl;
            inicializar_array(tam);

            for (hebra = 1; hebra < 33; ++hebra) {
                procesaParalelo_1(tam, hebra);
            }

            paralelo_1OutputFile.close();

            break;


        default:
            break;
        }

    }

    return 0;
}


//CODIGO UTIL
/*
    cout << "Sumado: ";
    for (int i = 0; i < vect_original.size(); ++i) {
        cout << vect_original[i] << " ";
    }
    cout << endl;

   vector<int> tams = {131072,262144,524288,
                        1048576, 2097152,
                        4194304, 8338608,
                        16777216, 33554432,
                        67108864};


*/