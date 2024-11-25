#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

volatile sig_atomic_t child1_turn = 1; // Переменная для синхронизации
volatile sig_atomic_t running = 1;    // Флаг завершения работы процессов

// Обработчики сигналов
void handle_signal1(int sig) {
    child1_turn = 0; // Переход хода ко второму процессу
}

void handle_signal2(int sig) {
    child1_turn = 1; // Переход хода к первому процессу
}

void handle_termination(int sig) {
    running = 0; // Остановить цикл работы
}

int main() {
    pid_t child1, child2;

    // Установить обработчики сигналов для завершения
    signal(SIGUSR1, handle_signal1);
    signal(SIGUSR2, handle_signal2);
    signal(SIGTERM, handle_termination);

    child1 = fork();
    if (child1 == -1) {
        perror("Failed to fork child1");
        exit(EXIT_FAILURE);
    }

    if (child1 == 0) { // Первый сын
        int iteration = 1;
        while (running) {
            if (child1_turn) { // Ожидаем своей очереди
                printf("Child 1 [PID: %d] - Iteration: %d\n", getpid(), iteration++);
                kill(getppid(), SIGUSR1); // Уведомить отца, что ход завершен
                pause();                  // Ожидание следующего сигнала
            }
        }
        exit(0);
    }

    child2 = fork();
    if (child2 == -1) {
        perror("Failed to fork child2");
        kill(child1, SIGTERM); // Убиваем первого сына в случае ошибки
        wait(NULL);
        exit(EXIT_FAILURE);
    }

    if (child2 == 0) { // Второй сын
        int iteration = 1;
        while (running) {
            if (!child1_turn) { // Ожидаем своей очереди
                printf("Child 2 [PID: %d] - Iteration: %d\n", getpid(), iteration++);
                kill(getppid(), SIGUSR2); // Уведомить отца, что ход завершен
                pause();                  // Ожидание следующего сигнала
            }
        }
        exit(0);
    }

    // Процесс-отец
    sleep(1); // Даем сыновьям время на инициализацию
    int elapsed_time = 0;
    while (elapsed_time < 10) {
        pause(); // Ждем сигналов от детей
        if (child1_turn) {
            kill(child1, SIGCONT); // Передать ход первому сыну
        } else {
            kill(child2, SIGCONT); // Передать ход второму сыну
        }
        sleep(1);
        elapsed_time++;
    }

    // Завершаем сыновей
    kill(child1, SIGTERM);
    kill(child2, SIGTERM);

    // Ждем завершения, чтобы избежать «зомби»-процессов
    wait(NULL);
    wait(NULL);

    printf("Parent process [PID: %d] - Terminating\n", getpid());
    return 0;
}
