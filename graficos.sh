#Comparaciones

# python3 plot_uno.py \
# ./csv/1/sum_secuencial.csv \
# Secuencial "16 Threads" "Tamaño muestra" "Tiempo[ns]"

# python3 plot_uno.py \
# ./csv/1/sum_paralela_1.csv \
# Paralela "16 Threads" "Tamaño muestra" "Tiempo[ns]"

# python3 plot_uno.py \
# ./csv/1/sum_paralela_2.csv \
# Hillis/Steele "16 Threads" "Tamaño muestra" "Tiempo[ns]"

# python3 plot_dos.py \
# ./csv/1/sum_paralela_1.csv \
# ./csv/1/sum_paralela_2.csv \
# Paralela Hillis/Steele "16 Threads - Paralelas" "Tamaño muestra" "Tiempo[ns]"

# python3 plot_dos.py \
# ./csv/1/sum_secuencial.csv \
# ./csv/1/sum_paralela_1.csv \
# Secuencial Paralela "16 Threads - Secuencial vs Paralela" "Tamaño muestra" "Tiempo[ns]"

# python3 plot_dos.py \
# ./csv/1/sum_secuencial.csv \
# ./csv/1/sum_paralela_2.csv \
# Secuencial Hillis/Steele "16 Threads - Secuencial vs Paralela" "Tamaño muestra" "Tiempo[ns]"

python3 plot_tres.py \
./csv/1/sum_secuencial.csv \
./csv/1/sum_paralela_1.csv \
./csv/1/sum_paralela_2.csv \
Secuencial Paralela Hillis/Steele "16 Threads - Todas" "Tamaño muestra" "Tiempo[ns]"


python3 plot_uno.py \
./csv/2/sum_paralela_1.csv \
Paralela "Arreglo tamaño 4.194.304 registros" "Cantidad hebras" "Tiempo[ns]"