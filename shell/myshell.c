#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>

typedef struct {
    char **args;     
    char *input; 
    char *output;    
    int append;      
    int background;  
} Command;

int flag_eof = 0;
int flag_quot = 0;
int kol = 0;
int quot_pair = 0;
int new_line = 0;
int errors = 0;
int child_exit_status = 0;
int flag_not_file;
int flag_home;

const char single_symbols[5] = ";()<";
const char pair_symbols[4] = "&|>";

void handle_sigchld(int sig) {
    int status;
    pid_t pid;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        if (WIFEXITED(status)) {
            printf("\nProcess %d exited with code %d\n", pid, WEXITSTATUS(status));
            fflush(stdout);
            child_exit_status = WEXITSTATUS(status);
        } 
        else if (WIFSIGNALED(status)) {
            printf("\nProcess %d aborted by signal %d\n", pid, WTERMSIG(status));
        }
    }
}

int is_pipeline(char **array) {
    if (!array) return 0;

    int i = 0;
    while (array[i] != NULL) {
        if (strcmp(array[i], "|") == 0)
            return 1;
        i++;
    }
    
    return 0;
}

int is_background(char **array) {
    if (!array) return 0;

    int i = 0;
    while (array[i] != NULL) {
        if (strcmp(array[i], "&") == 0)
            return 1;
        i++;
    }

    return 0;
}

Command parse_command(char **array) {
    Command cmd = {NULL, NULL, NULL, 0, 0};
    int i, j = 0;
    char **new_args = malloc(sizeof(char*));
    
    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], "<") == 0 && array[i + 1]) {
            cmd.input = strdup(array[++i]);
        }
        else if (strcmp(array[i], ">") == 0 && array[i + 1]) {
            cmd.output = strdup(array[++i]);
            cmd.append = 0;
        }
        else if (strcmp(array[i], ">>") == 0 && array[i + 1]) {
            cmd.output = strdup(array[++i]);
            cmd.append = 1;
        }
        else if (strcmp(array[i], "&") == 0) {
            cmd.background = 1;
        }
        else {
            new_args = realloc(new_args, (j + 2) * sizeof(char*));
            new_args[j++] = strdup(array[i]);
        }
    }
    new_args[j] = NULL;
    cmd.args = new_args;
    
    return cmd;
}

char ***split_pipeline(char **array) {
    char ***pipeline = malloc(sizeof(char**));
    int cmd_count = 0;
    int start = 0;
    int i, j;

    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], "|") == 0 || array[i + 1] == NULL) {
            int end = (array[i + 1] == NULL && strcmp(array[i], "|") != 0) ? i + 1 : i;
            pipeline = realloc(pipeline, (cmd_count + 2) * sizeof(char**));
            pipeline[cmd_count] = malloc(sizeof(char*) * (end - start + 1));
            
            for (j = 0; j < end - start; j++) {
                pipeline[cmd_count][j] = strdup(array[start + j]);
            }
            pipeline[cmd_count][j] = NULL;
            cmd_count++;
            start = i + 1;
        }
    }
    pipeline[cmd_count] = NULL;
    return pipeline;
}

void free_command(Command *cmd) {
    if (!cmd) return;
    if (cmd->args) {
        for (int i = 0; cmd->args[i]; i++)
            free(cmd->args[i]);
        
        free(cmd->args);
    }
    free(cmd->input);
    free(cmd->output);
}

void free_pipeline(char ***pipeline) {
    if (!pipeline) 
        return;

    for (int i = 0; pipeline[i]; i++) {
        for (int j = 0; pipeline[i][j]; j++) {
            free(pipeline[i][j]);
        }
        free(pipeline[i]);
    }
    free(pipeline);
}

int count_commands(char ***pipeline) {
    int count = 0;
    while (pipeline[count]) count++;
    return count;
}

char *read_word(FILE *file_in) {
    int is_word = 0;
    int is_spec_sym = 0;
    char c = 0;
    int iword = 0;
    char *word = (char *)malloc(sizeof(char));
    word[iword] = 0;

    while ((c = getc(file_in)) != EOF) {
        if (flag_quot) {
            if (c == '\n') {
                new_line = 1;
                flag_quot = 0;
                fprintf(stderr,"Неправильное количество кавычек\n");
                errors = 1;
                free(word);
                return NULL;
            } 
            if (c == '"') {
                flag_quot = 0;
            } 
            else {
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[iword] = c;
                iword++;
            }
        } 
        else if (is_spec_sym) {
            is_spec_sym = 0;
            if (c == word[iword - 1]) {
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[iword] = c;
                iword++;
                word[iword] = 0;
                return word;
            } 
            else {
                ungetc(c, file_in);
                word[iword] = 0;
                return word;
            }
        } 
        else if (isspace(c)) {
            if (is_word) { 
                if (c == '\n') {
                    new_line = 1;
                }
                word[iword] = 0;
                return word;
            } 
            else { 
                if (c == '\n') {
                    new_line = 1;
                    free(word);
                    word = NULL;
                    return NULL;
                }
                continue; 
            }
        } 
        else if (c == '"') {
            is_word = 1;
            flag_quot = 1;
        }
        else if (strchr(pair_symbols, c)) {
            if (is_word) {
                ungetc(c, file_in);
                word[iword] = 0;
                return word;
            } 
            else {
                is_spec_sym = 1;
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[iword] = c;
                iword++;
            }
        } 
        else if (strchr(single_symbols, c)) {
            if (is_word) {
                ungetc(c, file_in);
                word[iword] = 0;
                return word;
            } 
            else {
                word = (char *)realloc(word, (iword + 2) * sizeof(char));
                word[0] = c;
                word[1]= '\0';
                return word;
            }
        } 
        else {               
            is_word = 1;
            word = (char *)realloc(word, (iword + 2) * sizeof(char));
            word[iword] = c;
            iword++;
        }
    }
    flag_eof = 1;
    if (word != NULL) {
        free(word);
        return NULL;
    } else {
        word[iword] = 0;
        return word;
    }
}

char **add_to_array (char **array, char *word) {
    array = realloc(array, (kol + 2) * sizeof(char*));
    array[kol] = word;
    kol ++;
    array[kol] = NULL;
    return array;
}

void free_array(char** array) {
    while(*array != NULL) {
        free(*array);
        array++; 
    }
}

void print_array(char **array) {

    char** newarray = array;

    while (*newarray != NULL) {
        printf("%s\n",*newarray);
        newarray++;
    }
}

void check_directory() {
    char *home_dir = getenv("HOME");  
    char current_dir[1024];

    if (getcwd(current_dir, sizeof(current_dir)) != NULL) {
        if (strcmp(current_dir, home_dir) == 0) 
            flag_home = 1;
        else 
            flag_home = 0;
        
    } 
    else {
        fprintf(stderr, "Ошибка при выполнении getcwd\n");
        flag_home = 0;
    }
}

int is_cd(char *array[]) {
    if (strcmp(array[0], "cd") != 0)
        return 0;

    for (int i = 0; array[i] != NULL; i++) {
        if (strchr("|<>&", array[i][0]) != NULL) {
            fprintf(stderr, "Команда cd не может использоваться со специальными символами\n");
            return -1;
        }
    }

    if (array[1] == NULL) {
        if (chdir(getenv("HOME")) != 0) {
            fprintf(stderr, "Ошибка с cd\n");
            return -1;
        }
    } else if (array[2] != NULL) {
        fprintf(stderr, "cd: слишком много аргументов\n");
        return -1;
    } else if (chdir(array[1]) != 0) {
        fprintf(stderr, "Ошибка с cd\n");
        return -1;
    }

    return 1;
}

int execute_pipeline(char ***commands, int cmd_count) {
    int i;
    int in = 0;
    int fd[2];
    pid_t pid;
    int status;
    int last_status = 0;

    signal(SIGCHLD, SIG_DFL);

    for (i = 0; i < cmd_count; i++) {
        pipe(fd);
        
        if ((pid = fork()) == 0) {
            // Дочерний процесс
            if (in != 0) {
                dup2(in, 0);
                close(in);
            }
            
            if (i < cmd_count - 1) 
                dup2(fd[1], 1);
            
            close(fd[0]);
            execvp(commands[i][0], commands[i]);
            exit(1);
        }
        
        // Родительский процесс
        if (in != 0) 
            close(in);
        
        close(fd[1]);
        in = fd[0];
        
        if (i == cmd_count - 1) {
            waitpid(pid, &status, 0);
            last_status = WEXITSTATUS(status);
        }
    }
    
    return last_status;
}

int execute_command(Command *cmd) {
    pid_t pid;
    int status;
    
    if ((pid = fork()) == 0) {
        if (cmd->input) {
            int fd = open(cmd->input, O_RDONLY);
            if (fd == -1) {
                fprintf(stderr, "Ошибка ввода");
                exit(1);
            }
            dup2(fd, 0);
            close(fd);
        }
        
        if (cmd->output) {
            int flags = O_WRONLY | O_CREAT;
            flags |= cmd->append ? O_APPEND : O_TRUNC;
            int fd = open(cmd->output, flags, 0644);
            if (fd == -1) {
                fprintf(stderr, "Ошибка вывода");
                exit(1);
            }
            dup2(fd, 1);
            close(fd);
        }
        
        execvp(cmd->args[0], cmd->args);
        fprintf(stderr, "Ошибка с execvp");
        exit(1);
    }
    
    if (!cmd->background) {
        waitpid(pid, &status, 0);
        return WEXITSTATUS(status);
    }
    
    printf("[%d] Started in background\n", pid);
    return 0;
}

char ***split_commands(char **array, int *cmd_count) {
    char ***commands = malloc(sizeof(char**));
    int count = 0;
    int start = 0;
    int i, j;

    for (i = 0; array[i] != NULL; i++) {
        if (strcmp(array[i], ";") == 0 || array[i + 1] == NULL) {
            int end = array[i + 1] == NULL && strcmp(array[i], ";") != 0 ? i + 1 : i;
            commands = realloc(commands, (count + 2) * sizeof(char**));
            commands[count] = malloc(sizeof(char*) * (end - start + 1));
            
            for (j = 0; j < end - start; j++)
                commands[count][j] = strdup(array[start + j]);

            commands[count][j] = NULL;
            count++;
            start = i + 1;
        }
    }
    
    commands[count] = NULL;
    *cmd_count = count;

    return commands;
}

int main(int argc, char ** argv) {
    char *word;
    char **array = NULL;
    FILE *file_in;
    Command cmd;

    if (argc == 2) {
        file_in = fopen(argv[1], "r");
        flag_not_file = 0;

        if (!file_in) {
            fprintf(stderr, "Ошибка открытия файла\n");
            return 1;
        }

    } else {
        file_in = stdin;
        flag_not_file = 1;
    }

    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGCHLD, handle_sigchld);

    while(!flag_eof) {
        kol = 0;
        array = malloc(sizeof(char*));
        array[0] = NULL;
        new_line = 0;
        errors = 0;

        check_directory();

        if (flag_not_file && flag_home == 1)
            printf("~$ ");
        else if (flag_not_file && (flag_home == 0))
            printf("!$ ");
        
        while(!flag_eof && !new_line && !errors) {
            word = read_word(file_in);

            if (word != NULL)
                array = add_to_array(array, word);
        }

        if (array[0] != NULL && !errors) {
            int cmd_count;
            char ***commands = split_commands(array, &cmd_count);
            
            for (int i = 0; i < cmd_count; i++) {
                int cd_status = is_cd(commands[i]);
                
                if (cd_status == -1)
                    continue;
                else if (cd_status == 1)
                    continue;
                
                if (is_pipeline(commands[i])) {
                    char ***pipeline = split_pipeline(commands[i]);
                    int pipe_count = count_commands(pipeline);
                    execute_pipeline(pipeline, pipe_count);
                    free_pipeline(pipeline);
                } else {
                    cmd = parse_command(commands[i]);
                    execute_command(&cmd);
                    free_command(&cmd);
                }
            }
            
            for (int i = 0; commands[i] != NULL; i++) {
                for (int j = 0; commands[i][j] != NULL; j++) 
                    free(commands[i][j]);
                free(commands[i]);
            }
            free(commands);
        }

        if (array != NULL) {
            for (int i = 0; array[i] != NULL; i++) 
                free(array[i]);
            free(array);
        }
    }
    if (argc == 2)
        fclose(file_in);

    if (flag_not_file)
        printf("\nTHANK YOU FOR USING MY PROGRAM!!\n");

    return 0;
}
