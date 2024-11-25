#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <ctype.h>
#include <errno.h>

#define INITIAL_BUFFER_SIZE 1024
#define INITIAL_WORD_ARRAY_SIZE 10

// Проверка управляющего символа
int is_control_char(char c) {
    return (c == '&' || c == '|' || c == ';' || c == '>' || c == '<' || c == '(' || c == ')');
}

// Добавление слова в массив
void add_word(char *word, char ***words, int *word_count, int *word_array_size) {
    if (*word_count >= *word_array_size) {
        *word_array_size *= 2;
        *words = realloc(*words, (*word_array_size) * sizeof(char*));
        if (*words == NULL) {
            fprintf(stderr, "Ошибка выделения памяти для массива слов\n");
            exit(EXIT_FAILURE);
        }
    }
    (*words)[*word_count] = strdup(word);
    (*word_count)++;
}

// Разбор строки на слова
void parse_line(char *line, char ***words, int *word_count, int *in_quotes) {
    int word_array_size = INITIAL_WORD_ARRAY_SIZE;
    *words = malloc(word_array_size * sizeof(char*));
    *word_count = 0;
    *in_quotes = 0;

    if (*words == NULL) {
        fprintf(stderr, "Ошибка выделения памяти для массива слов\n");
        exit(EXIT_FAILURE);
    }

    char word[INITIAL_BUFFER_SIZE] = {0};
    int word_index = 0;

    for (int i = 0; line[i] != '\0'; i++) {
        char c = line[i];

        if (c == '"') {
            *in_quotes = !(*in_quotes);
            continue;
        }

        if (*in_quotes) {
            word[word_index++] = c;
            continue;
        }

        if (isspace(c)) {
            if (word_index > 0) {
                word[word_index] = '\0';
                add_word(word, words, word_count, &word_array_size);
                word_index = 0;
                memset(word, 0, sizeof(word));
            }
            continue;
        }

        if (is_control_char(c)) {
            if (word_index > 0) {
                word[word_index] = '\0';
                add_word(word, words, word_count, &word_array_size);
                word_index = 0;
                memset(word, 0, sizeof(word));
            }

            word[0] = c;
            word[1] = '\0';
            if ((c == '&' && line[i + 1] == '&') || 
                (c == '|' && line[i + 1] == '|') || 
                (c == '>' && line[i + 1] == '>')) {
                word[1] = line[++i];
                word[2] = '\0';
            }
            add_word(word, words, word_count, &word_array_size);
            memset(word, 0, sizeof(word));
            continue;
        }

        word[word_index++] = c;
    }

    if (word_index > 0) {
        word[word_index] = '\0';
        add_word(word, words, word_count, &word_array_size);
    }
}

// Чтение строки из файла или stdin
char* read_line(FILE *input_file) {
    size_t buffer_size = INITIAL_BUFFER_SIZE;
    size_t position = 0;
    char *buffer = malloc(buffer_size);

    if (!buffer) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(EXIT_FAILURE);
    }

    int c;
    while ((c = fgetc(input_file)) != EOF && c != '\n') {
        buffer[position++] = c;
        if (position >= buffer_size) {
            buffer_size *= 2;
            buffer = realloc(buffer, buffer_size);
            if (!buffer) {
                fprintf(stderr, "Ошибка выделения памяти при расширении буфера\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    buffer[position] = '\0';
    if (position == 0 && c == EOF) {
        free(buffer);
        return NULL;
    }
    return buffer;
}

// Обработка перенаправления ввода-вывода
void handle_redirection(char **args) {
    for (int i = 0; args[i] != NULL; i++) {
        if (strcmp(args[i], ">") == 0) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) perror("Ошибка открытия файла для записи");
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        } else if (strcmp(args[i], ">>") == 0) {
            int fd = open(args[i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            if (fd < 0) perror("Ошибка открытия файла для добавления");
            dup2(fd, STDOUT_FILENO);
            close(fd);
            args[i] = NULL;
        } else if (strcmp(args[i], "<") == 0) {
            int fd = open(args[i + 1], O_RDONLY);
            if (fd < 0) perror("Ошибка открытия файла для чтения");
            dup2(fd, STDIN_FILENO);
            close(fd);
            args[i] = NULL;
        }
    }
}

void execute_pipeline(char **args) {
    int pipefd[2];
    pid_t pid;
    int fd_in = 0;

    while (*args != NULL) {
        char *command[INITIAL_WORD_ARRAY_SIZE];
        int i = 0;
        int status;
        
        if (strcmp(args[i], "|") == 0) {
            // Проверяем наличие команды до и после '|'
            if (i == 0 || args[i + 1] == NULL) {
                break;
            }
        }

        while (*args != NULL && strcmp(*args, "|") != 0) {
            command[i++] = *args++;
        }
        command[i] = NULL;

        if (*args != NULL) args++; // Пропустить символ "|"

        pipe(pipefd);

        if ((pid = fork()) == -1) {
            perror("Ошибка создания процесса");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            dup2(fd_in, STDIN_FILENO); // Входной поток
            if (*args != NULL) dup2(pipefd[1], STDOUT_FILENO); // Выходной поток
            close(pipefd[0]);
            execvp(command[0], command);
            perror("Ошибка выполнения команды");
            exit(EXIT_FAILURE);
        } else {
            waitpid(pid, &status, 0); // Ожидание завершения процесса
            close(pipefd[1]);
            fd_in = pipefd[0]; // Обновить входной поток
        }
    }
}

void execute_conditional_commands(char **args) {
    int status = 0;
    for (int i = 0; args[i] != NULL;) {
        char *command[INITIAL_WORD_ARRAY_SIZE];
        int j = 0;

        while (args[i] != NULL && strcmp(args[i], "&&") != 0 && strcmp(args[i], "||") != 0 && strcmp(args[i], ";") != 0) {
            command[j++] = args[i++];
        }
        command[j] = NULL;

        if (fork() == 0) { // Дочерний процесс для выполнения команды
            execvp(command[0], command);
            perror("Ошибка выполнения команды");
            exit(EXIT_FAILURE);
        } 
        else {
            wait(&status); // Ожидание завершения процесса

            if ((args[i] != NULL && strcmp(args[i], "&&") == 0 && WEXITSTATUS(status) != 0) ||
                (args[i] != NULL && strcmp(args[i], "||") == 0 && WEXITSTATUS(status) == 0)) {
                break; // Пропустить выполнение следующей команды при условии
            }

            if (args[i] != NULL && strcmp(args[i], ";") == 0) i++; // Пропустить разделитель ";"
        }
    }
}


void execute_command(char **args) {
    int background = 0;
    if (strcmp(args[0], "cd") == 0) {
        if (args[1] == NULL) {
            const char *home_dir = getenv("HOME");
            if (home_dir == NULL) fprintf(stderr, "Не удалось получить домашний каталог\n");
            
            else if (chdir(home_dir) != 0) perror("Ошибка при переходе в домашний каталог");
        } 
        else {
            if (chdir(args[1]) != 0) perror("Ошибка при смене каталога");
        }
    } 
    else {
    // Проверка на фоновый режим
    for (int i = 0; args[i]; i++) {
        if (strcmp(args[i], "&") == 0) {
            background = 1;
            args[i] = NULL; // Удалить "&" из аргументов
            break;
        }
    }

    pid_t pid = fork();

    if (pid == -1) {
        perror("Ошибка создания процесса");
    } else if (pid == 0) { // Дочерний процесс
        if (background) {
            // Перенаправление ввода на /dev/null
            int dev_null = open("/dev/null", O_RDONLY);
            if (dev_null < 0) {
                perror("Ошибка открытия /dev/null");
                exit(EXIT_FAILURE);
            }
            dup2(dev_null, STDIN_FILENO);
            close(dev_null);

            // Игнорирование SIGINT
            signal(SIGINT, SIG_IGN);
        }

        handle_redirection(args); // Обработка перенаправления ввода-вывода
        execvp(args[0], args); // Выполнение команды
        perror("Ошибка выполнения команды");
        exit(EXIT_FAILURE);
    } else { // Родительский процесс
        if (!background) {
            waitpid(pid, NULL, WUNTRACED); // Ожидание завершения процесса
        } else {
            printf("Фоновый процесс запущен с PID %d\n", pid);
        }
    }
    }
}

void handle_sigchld(int sig) {
    int status;
    pid_t pid;

    // Обрабатываем все завершённые дочерние процессы
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("Фоновый процесс %d завершился с кодом %d\n", pid, WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("Фоновый процесс %d был прерван сигналом %d\n", pid, WTERMSIG(status));
        }
    }
}

// Главная функция программы
int main(int argc, char *argv[]) {
    FILE *input_file = stdin;

    signal(SIGINT, SIG_IGN);
    

    // Обработка ввода из файла
    if (argc == 2) {
        input_file = fopen(argv[1], "r");
        if (input_file == NULL) {
            fprintf(stderr, "Ошибка открытия файла\n");
            return 1;
        }
    }
    if (argc > 2) {
        fprintf(stderr, "Ошибка ввода\n");
        return 1;
    }
    if (argc == 1) printf(">$ ");

    char *line;
    char **words;
    int word_count;
    int in_quotes;

    while ((line = read_line(input_file)) != NULL) {
        
        parse_line(line, &words, &word_count, &in_quotes);

        words[word_count] = NULL; // Завершение массива строк

        // Выполнение команды

        int is_spec = 0;
        int is_pipeline = 0;
        for (int i = 0; i < word_count; i++) {
            if (strcmp(words[i], "|") == 0) {
                is_pipeline = 1;
            }
            if (strcmp(words[i], "||") == 0 || strcmp(words[i], "&&") == 0 || strcmp(words[i], ";") == 0){
                is_spec = 1;
            }
        }
        if (word_count > 0) {
            if (is_pipeline) {
                execute_pipeline(words); // Выполнение конвейера
            } 
            else if (is_spec) {
                execute_conditional_commands(words); // Выполнение команды с условными операциями
            }
            else {
                execute_command(words);
            }
        }
        for (int i = 0; i < word_count; i++) free(words[i]);
        
	    free(words); free(line);

        if (in_quotes) {
            fprintf(stderr, "Неверный ввод ковычек\n");
            if (argc == 1) printf(">$ ");
            continue;
        }
        if (argc == 1) 
            printf(">$ ");      

    }
    if (input_file != stdin) 
        fclose(input_file);

    return 0;
}