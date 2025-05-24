#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <windows.h>


// var  de tip mutex (mutual exclusion) - se asigura ca un singur thread poate accesa o seciune critica de cod la un moment dat
pthread_mutex_t lock1;
pthread_mutex_t lock;

// numarul de procese si nr de threaduri pt fiecare proces
#define NUM_THREADS 3
#define NUM_PROCESE 3

// Worker thread 1: Multiplica cu 4
void *worker_thread1(void *arg) {
    pthread_mutex_lock(&lock); // obtine lock
    printf("Threadul 1 intra in sectiunea critica in procesul %d.\n", GetCurrentProcessId());
    Sleep(4000); // simulare sleep in milisecunde

    // Citeste valoarea curenta din fisier
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Thread 1: Eroare la deschiderea fisierului pentru citire.\n");
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    int valoareFisier;
    fscanf(file, "%d", &valoareFisier);
    fclose(file);

    printf("Thread 1: Valoare originala fisier: %d\n", valoareFisier);
    valoareFisier *= 4;
    printf("Thread 1: Valoare dupa inmultirea cu 4: %d\n", valoareFisier);
    Sleep(4000); // simulare treaba

    file = fopen("input.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", valoareFisier);
        fclose(file);
        printf("Thread 1: Valoarea a fost actualizata in fisier.\n");
    } else {
        printf("Thread 1: Eroare la deschiderea fisierului pentru scriere.\n");
    }

    printf("Threadul 1 a parasit sectiunea critica in procesul %d.\n", GetCurrentProcessId());
    pthread_mutex_unlock(&lock); // elibereaza lock
    pthread_exit(NULL);
}

// Worker thread 2: Scade cu 2
void *worker_thread2(void *arg) {
    pthread_mutex_lock(&lock); // obtine lock
    printf("Threadul 2 intra in sectiunea critica in procesul %d.\n", GetCurrentProcessId());
    Sleep(4000); // simulare sleep in milisecunde

    // Citeste valoarea curenta din fisier
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Thread 2: Eroare la deschiderea fisierului pentru citire.\n");
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    int valoareFisier;
    fscanf(file, "%d", &valoareFisier);
    fclose(file);


    printf("Thread 2: Valoare originala fisier: %d\n", valoareFisier);
    valoareFisier -= 2;
    printf("Thread 2: Valoare dupa scaderea cu 2: %d\n", valoareFisier);
    Sleep(4000); // simulare treaba

    file = fopen("input.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", valoareFisier);
        fclose(file);
        printf("Thread 2: Valoarea a fost actualizata in fisier.\n");
    } else {
        printf("Thread 2: Eroare la deschiderea fisierului pentru scriere.\n");
    }
    printf("Threadul 2 a parasit sectiunea critica in procesul %d.\n", GetCurrentProcessId());
    pthread_mutex_unlock(&lock); // elibereaza lock
    pthread_exit(NULL);
}

// Worker thread 3: Imparte la 2
void *worker_thread3(void *arg) {
    pthread_mutex_lock(&lock); // obtine lock
    printf("Threadul 3 intra in sectiunea critica in procesul %d.\n", GetCurrentProcessId());
    Sleep(4000); // simulare sleep in milisecunde

    // Citeste valoarea curenta din fisier
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Thread 3: Eroare la deschiderea fisierului pentru citire.\n");
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    int valoareFisier;
    fscanf(file, "%d", &valoareFisier);
    fclose(file);

    printf("Thread 3: Valoare originala fisier: %d\n", valoareFisier);
    valoareFisier /= 2;
    printf("Thread 3: Valoare dupa impartirea cu 2: %d\n", valoareFisier);
    Sleep(4000); // simulare treaba

    file = fopen("input.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d\n", valoareFisier);
        fclose(file);
        printf("Thread 3: Valoarea a fost actualizata in fisier.\n");
    } else {
        printf("Thread 3: Eroare la deschiderea fisierului pentru scriere.\n");
    }
    printf("Threadul 3 a parasit sectiunea critica in procesul %d.\n", GetCurrentProcessId());
    pthread_mutex_unlock(&lock); // elibereaza lock
    pthread_exit(NULL);
}

// functia thread a procesului parinte
void *parent_process(void *arg) {
    int id = *(int *)arg;

    // sectiune critica
    pthread_mutex_lock(&lock1); // obtine lockul
    printf("Procesul parinte %d a inceput lucrul.\n", id);

    pthread_t threads[NUM_THREADS]; // declarare vector de threads
    int thread_ids[NUM_THREADS];    // declarare vector int pentru IDurile threadurilor

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        if(i==0) { if (pthread_create(&threads[i], NULL, worker_thread1, &thread_ids[i]) != 0) {
                // creare thread nou, IDul threadului va fi salvat in threads[i], comportamentul acestuia va fi dat de functia worker, careia i se transmite nrul threadului
            printf("Eroare la crearea threadului %d in procesul parinte.\n", i + 1);
            return NULL;
        }
        }
        if(i==1) { if (pthread_create(&threads[i], NULL, worker_thread2, &thread_ids[i]) != 0) {
                // creare thread nou, IDul threadului va fi salvat in threads[i], comportamentul acestuia va fi dat de functia worker, careia i se transmite nrul threadului
            printf("Eroare la crearea threadului %d in procesul parinte.\n", i + 1);
            return NULL;
        }
        }
        if(i==2) { if (pthread_create(&threads[i], NULL, worker_thread3, &thread_ids[i]) != 0) {
                // creare thread nou, IDul threadului va fi salvat in threads[i], comportamentul acestuia va fi dat de functia worker, careia i se transmite nrul threadului
            printf("Eroare la crearea threadului %d in procesul parinte.\n", i + 1);
            return NULL;
        }
        }
    }

    // se asteapta threadurile create in procesul parinte
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Citeste valoarea curenta din fisier
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Proces parinte %d: Eroare la deschiderea fisierului pentru citire.\n",id);
        pthread_mutex_unlock(&lock1);
        pthread_exit(NULL);
    }
    int valoareFisier;
    fscanf(file, "%d", &valoareFisier);
    fclose(file);

    printf("Procesul parinte %d a terminat. Valoare fisier: %d\n",id,valoareFisier);
    pthread_mutex_unlock(&lock1); // elibereaza lockul
    return NULL;
}


int main() {
    // Citeste val curenta din fisier
    FILE *file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Error: nu s-a putut deschide fisierul.\n");
        return EXIT_FAILURE;
    }
    int valoareFisier;
    fscanf(file, "%d", &valoareFisier);
    fclose(file);

    printf("Valoarea initiala citita din fisier: %d\n", valoareFisier);

    int nr_proces[NUM_PROCESE];
    // initializare mutex, cu atribute default (NULL)
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Error: initializare mutex esuata.\n");
        return EXIT_FAILURE;
    }

    // declarare variabile tip obiect pthread_t pt stocare thread ID
    pthread_t threads[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        nr_proces[i] = i + 1;
        if (pthread_create(&threads[i], NULL, parent_process, &nr_proces[i]) != 0) {
                // creare thread nou, IDul threadului va fi salvat in threads[i], comportamentul acestuia va fi dat de functia worker, careia i se transmite nrul threadului
            printf("Eroare la crearea threadului %d in procesul parinte.\n", i + 1);
            return NULL;
        }
    }


    // se asteapta threadurile principale
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // sterge lockul mutex
    pthread_mutex_destroy(&lock);

    // Citeste valoarea curenta din fisier
    file = fopen("input.txt", "r");
    if (file == NULL) {
        printf("Thread 3: Eroare la deschiderea fisierului pentru citire.\n");
        pthread_mutex_unlock(&lock);
        pthread_exit(NULL);
    }
    fscanf(file, "%d", &valoareFisier);
    fclose(file);

    printf("Valoarea finala dupa incheierea operatiunilor: %d\n", valoareFisier);

    return 0;
}

