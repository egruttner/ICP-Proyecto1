#Comparaciones

python3 plot_uno.py \
./csv/1/sum_secuencial.csv \
Secuencial "Desempeño_Secuencial" "Tamaño muestra" "Tiempo[ns]"

python3 plot_uno.py \
./csv/1/sum_paralela_1.csv \
Paralela "Desempeño_Paralaleo" "Tamaño muestra" "Tiempo[ns]"

python3 plot_uno.py \
./csv/1/sum_paralela_2.csv \
Hillis-Steele "Desempeño_Hillis-Steele" "Tamaño muestra" "Tiempo[ns]"

python3 plot_dos.py \
./csv/1/sum_paralela_1.csv \
./csv/1/sum_paralela_2.csv \
Paralela Hillis/Steele "Paralelas" "Tamaño muestra" "Tiempo[ns]"

python3 plot_dos.py \
./csv/1/sum_secuencial.csv \
./csv/1/sum_paralela_1.csv \
Secuencial Paralela "Secuencial_vs_Paralela" "Tamaño muestra" "Tiempo[ns]"

python3 plot_dos.py \
./csv/1/sum_secuencial.csv \
./csv/1/sum_paralela_2.csv \
Secuencial Hillis-Steele "Secuencial_vs_Hillis-Steele" "Tamaño muestra" "Tiempo[ns]"

python3 plot_tres.py \
./csv/1/sum_secuencial.csv \
./csv/1/sum_paralela_1.csv \
./csv/1/sum_paralela_2.csv \
Secuencial Paralela Hillis-Steele "Todas" "Tamaño muestra" "Tiempo[ns]"

python3 plot_uno.py \
./csv/2/sum_paralela_1.csv \
Paralela "Rendimiento_hebras" "Cantidad hebras" "Tiempo[ns]"