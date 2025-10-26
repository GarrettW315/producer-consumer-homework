#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#include "table.hpp"

using namespace std;

int main() {
    int shm_fd = shm_open("/shm_table", O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) { perror("shm_open"); return 1; }

    ftruncate(shm_fd, sizeof(table_t));
    table_t *table = (table_t *)mmap(NULL, sizeof(table_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    table->in = 0;
    table->out = 0;

    sem_t *empty = sem_open("/empty_sem", O_CREAT, 0666, TABLE_SIZE);
    sem_t *full = sem_open("/full_sem", O_CREAT, 0666, 0);
    sem_t *mutex = sem_open("/mutex_sem", O_CREAT, 0666, 1);

    int item = 0;
    while (true) {
        item++;

        sem_wait(empty);
        sem_wait(mutex);

        table->buffer[table->in] = item;
        cout << "Producer produced: " << item << endl;
        table->in = (table->in + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(full);

        sleep(1);
    }
}