/*---------------------------------------------------------
 *
 * Programación avanzada: Manejo de threads
 *
 * Fecha: 22 de mayo de 2020
 * 
 * Autores: Diana Ortiz
 *          Marcela Arcos
 *
 * Matricula: A01209403
 *            A01703191
 *
 *---------------------------------------------------------*/

/* 
 * TIEMPOS DE EJECUCIÓN 
 * 
 * Primer prueba : 1 thread 
 * Tiempo de ejecución : 17.86986 s
 * 
 * Segunda prueba : 2 threads
 * Tiempo de ejecución : 7.43704 s
 *
 * Tercer prueba : 6 threads
 * Tiempo de ejecución : 3.38546 s
 *
 * Cuarta prueba : 8 threads
 * Tiempo de ejecución : 2.49128 s 
 *
 * Quinta prueba : 25 threads
 * Tiempo de ejecución : 0.82802 s
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

int dim_matrix1;
int dim_matrix2;
int threads_amount;

float **Matrix1;
float **Matrix2;
float **Result;

/* Memoria dinamica */
void dm(int dim_matrix1){
  
    Matrix1 = malloc(dim_matrix1 * sizeof(float*)); 
    for (int i = 0; i < dim_matrix1; i++) {
        Matrix1[i] = malloc(dim_matrix1 * sizeof(float));
    }
    
    Matrix2 = malloc(dim_matrix2 * sizeof(float*)); 
    for (int i = 0; i < dim_matrix2; i++) {
        Matrix2[i] = malloc(dim_matrix1 * sizeof(float));
    }

    Result = malloc(dim_matrix1 * sizeof(float*)); 
    for (int i = 0; i < dim_matrix1; i++) {
        Result[i] = malloc(dim_matrix1 * sizeof(float));
    }
}

/* Realizar multiplicación de las matrices */
void *multiply(void *arg){
    int m;
    int n;
    int begin; 
    int end;
    float acum;

  /* Dividir numero de threads */
    int rem = dim_matrix1 % threads_amount;
    m = *(int* )(arg);
    n = ceil(dim_matrix1 / threads_amount);
    begin = m * n;
    end = (m + 1) * n;

    if(m == threads_amount - 1){
      end = end + rem;
    }

    for (int i = begin; i < end; i++) { 
        for (int j = 0; j < dim_matrix1; j++) { 
            acum = 0;
            for (int k = 0; k < dim_matrix1; k++) { 
                acum = acum + Matrix1[i][k] * Matrix2[k][j];
            }
            Result[i][j] = acum;
        }
    }
}

int main(int argc, char* argv[]){
    int f1;
    int f2;
    double acum = 0;
    double time = 0;
    pthread_t *threads;

  /* Verificar que reciba la cantidad correcta de parámetros : 
   * El nombre del programa se guarda en argv[0], por lo tanto la numeración de parametros empieza en argv[1]
   * Argc, es utilizado para conocer el número de parámetros ingresados y realizar la evaluación de estos
   * En caso de que no se cumpla, el programa regresa -2 y termina
   */
    if (argc != 7) {
        printf("%s -a matrix_file_a.txt -b matrix_file_b.txt -t thread_amount\n", argv[0]);
        return -2;
    }

  /* Verificar que reciba los parámetros sean correctos : 
   * Strcmp, compara dos cadenas de texto, en este caso la ingresada por el usuario y la indicada en el programa
   */
    if (strcmp(argv[1], "-a") != 0) {
      /* En caso de que falte "-a", el programa regresa -3 */
        printf("%s la bandera para la primer matrix debe ser -a", argv[1]);
        return -3;
    }
    if (strcmp(argv[3], "-b") != 0) {
      /* En caso de que falte "-b", el programa regresa -4 */
        printf("%s la bandera para la segunda matrix debe ser -b\n", argv[3]);
        return -4;
    }
    if (strcmp(argv[5], "-t") != 0) {
      /* En caso de que falte "-t", el programa regresa -5 */
        printf("%s la bandera para los threads debe ser -t\n", argv[5]);
        return -5;
    }

  /* Verificar que los archivos existan */  
    if ((f1 = open(argv[2], O_RDONLY)) < 0) {
        /* Archivo para la primer matrix 
         * En caso de que no sea válido, el programa regresa -6 
         */
        printf("%s (matrix_file_a) el archivo no es válido\n", argv[2]);
        return -6;
    } 
    if ((f2 = open(argv[4], O_RDONLY)) < 0) {
        /* Archivo para la segunda matrix 
         * En caso de que no sea válido, el programa regresa -7
         */
        printf("%s (matrix_file_b) el archivo no es válido\n", argv[4]);
        return -7;
    }

    /* Verificar que threads_amount sea un número entero positivo 
     *  En caso de que no sea válido, el programa regresa -8
     */  
    threads_amount = atoi(argv[6]);
    if(threads_amount <= 0){
        printf("El número de threads debe ser un entero mayor a 0\n");
        return -8;
    }

   /* Abrir archivos en modo lectura */  
    FILE *File1, *File2;
    File1 = fopen(argv[2],"r");
    File2 = fopen(argv[4],"r");

    /* Verificar que los archivos no esten vacios */  
    if((File1 = fopen(argv[2],"r")) != NULL && (File2 = fopen(argv[4],"r")) != NULL) {
        fscanf(File1,"%d",&dim_matrix1);
        fscanf(File2,"%d",&dim_matrix2);
    }

    /* Verificar que el tamaño de las matrices sea el mismo 
     * En caso de que no sea el mismo, el programa regresa -9
     */
    if(dim_matrix1 == dim_matrix2){
        dm(dim_matrix1);
        for(int i = 0; i < dim_matrix1; i++){
            for(int j = 0; j < dim_matrix1; j++){
                if(!fscanf(File1, "%f", &Matrix1[i][j])){
                    break;
                }
            }
        }
        for(int i = 0; i < dim_matrix2; i++){
            for(int j = 0; j < dim_matrix2; j++){
                if(!fscanf(File2, "%f", &Matrix2[i][j])){
                    break;
                }
            }
        }
        fclose(File1);
        fclose(File2);
    } else {
        printf("El tamaño de las matrices debe de ser el mismo\n");
        return -9;
    }

    threads = (pthread_t*) malloc(threads_amount * sizeof(pthread_t));
    clock_t begin = clock();

    for (int i = 0; i < threads_amount; i++) {
        int *m;
        m = (int *) malloc(sizeof(int));
        *m = i;
        pthread_create(&threads[i], NULL, multiply, (void *)m);
    } 
    for (int i = 0; i < threads_amount; i++) {
        pthread_join(threads[i], NULL);
    }
    clock_t end = clock();

  /* Imprimir resultado de la matriz */
    printf("Resultado de la multiplicación de la matriz: \n");
    for(int i = 0; i < dim_matrix1; i++){
        for(int j = 0; j < dim_matrix1; j++){
            printf("%f ", Result[i][j]);
        }
        printf("\n");
    }
  
  /* Mostrar tiempos de ejecución dependiendo del numero de threads */
    time += (double)(end - begin) / CLOCKS_PER_SEC;   
    printf("Threads : %i, Tiempo : %.5f s\n",threads_amount, time/threads_amount);
}
