#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "table.hpp"

using namespace std;

int main(){
    int shm_fd = shm_open("/shm_table", O_RDWR, 0666);
    if(shm_fd < 0){ perror("shm_open"); return 1; }

    table_t *table = (table_t *)mmap(NULL, sizeof(table_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t *empty = sem_open("/empty_sem", 0);
    sem_t *full = sem_open("/full_sem", 0);
    sem_t *mutex = sem_open("/mutex_sem", 0);

    while(true){
        sem_wait(full);
        sem_wait(mutex);

        int item = table->buffer[table->out];
        cout << "Consumer consumed: " <<item << endl;
        table->out = (table->out + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(empty);

        sleep(2);
    }
}