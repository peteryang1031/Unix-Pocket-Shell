#include"Header.h"
#include"ls.h"
#include"cat.h"
int Command_transfer(char *command)
{
        if (strcmp(command, "ls") == 0)
        {
                return 0;
        }
        else if (strcmp(command, "cd") == 0)
        {
                return 1;
        }
        else if (strcmp(command, "pwd") == 0)
        {
                return 2;
        }
        else if (strcmp(command, "cat") == 0)
        {
                return 3;
        }
}
char *Get_Command(char *shell)
{
        int i = 0;
        if (shell[0] == ' ')
        { //去除命令前的空格
                i++;
        }
        char *command = (char *)malloc(5 * sizeof(char));

        int j = 0;
        int x = 1;
        while (shell[i] != ' ' && shell[i] != '\n')
        {
                command[j] = shell[i];
                i++;
                j++;
        }
        command[j] = '\0';
        return command;
}

void cd_command(char *shell)
{
        int paracnt = 1;
        char *para[5];
        for (int u = 0; u < 5; u++)
        {
                para[u] = (char *)malloc(10 * sizeof(char));
        }
        para[0] = "cd";
        int temp = 2;
        while (shell[temp] == ' ')
        {
                temp++;
        }
        int i = 0;
        while (shell[temp] != ' ' && shell[temp] != '\n')
        {
                para[1][i++] = shell[temp++];
        }
        para[1][i] = '\0';
        para[2] = NULL;
        if (para[1][0] == '\0' || para[1][0] == '~')
        { //cd 主目录
                para[1] = getenv("HOME");
        }
        if (chdir(para[1]) < 0)
        {
                perror("chdir");
        }
        //execvp("cd",para);
        //perror("cd command");
        //execl("./go.sh","go",NULL);
        //exit(0);
}
void pwd_command(char *shell)
{
        execlp("pwd", "pwd", NULL);
        perror("pwd command");
        exit(1);
}

int main(int argc, char *argv[])
{
        pid_t pid;
        char *shell;
        shell = (char *)malloc(30 * sizeof(char));
        printf("shell>");
        while (fgets(shell, 30, stdin))
        {
                if(shell[0]=='\n')
                {
                        printf("shell>");
                        continue;
                }
                pid = fork();
                if (pid < 0)
                {
                        perror("Create Process");
                        exit(1);
                }
                else if (pid == 0)
                {
                        //execlp("ls","ls",NULL);
                        char *command = (char *)malloc(5);
                        command = Get_Command(shell);
                        switch (Command_transfer(command))
                        {
                        case 0:
                                ls_command(shell);
                                break;
                        case 1:
                                //cd_command(shell);
                                exit(1);
                                break;
                        case 2:
                                pwd_command(shell);
                                break;
                        case 3:
                                cat_command(shell);
                                break;
                        default:
                                printf("%s", "This command is out of version\n");
                                exit(1);
                                break;
                        }
                }
                else
                {
                        char *command = (char *)malloc(5);
                        if (Command_transfer(Get_Command(shell)) == 1)
                        { //cd 命令
                                cd_command(shell);
                        }
                        waitpid(pid, NULL, 0);
                        printf("shell>");
                        fflush(stdout);
                }
        }
        exit(1);
}
// int main(void)
// {
//         char answer[100];
//         char *p;
//         execlp("cd","cd","..",NULL);
//         return 0;
// }
