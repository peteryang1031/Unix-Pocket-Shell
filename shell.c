#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
char *Get_Command(char *shell)
{
        int i = 0;
        if (shell[0] == ' ')
        { //去除命令前的空格
                i++;
        }
        char *command = (char *)malloc(5 * sizeof(char));

        int j = 0;

        while (shell[i] != ' ' && shell[i] != '\n')
        {
                command[j] = shell[i];
                i++;
                j++;
        }
        command[j] = '\0';
        return command;
}
void ls_command(char *shell)
{
        int paracnt = 1;
        char *para[5];
        for (int u = 0; u < 5; u++)
        {
                para[u] = (char *)malloc(10 * sizeof(char));
        }
        para[0] = "ls";
        char *pos = NULL;
        while (((pos = strchr(shell, '-')) != NULL))
        {
                para[paracnt][0] = '-';
                int temp = 1;
                while (isalpha(*(pos + temp))||*(pos+temp)=='-')
                {
                        para[paracnt][temp] = *(pos + temp);
                        temp++;
                }
                shell = pos + temp;
                paracnt++;
        }

        para[paracnt] = NULL;
        execvp("ls", para);
}
int main(void)
{
        pid_t pid;
        char *shell;
        shell = (char *)malloc(30 * sizeof(char));
        printf("$");
        while (fgets(shell, 30, stdin))
        {
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
                        if (strcmp(command, "ls") == 0)
                        {
                                ls_command(shell);
                        }
                }
                else {
                        waitpid(pid,NULL,0);
                        printf("$");
                }
        }
}
// int main(void)
// {
//         char answer[100];
//         char *p;
//         printf("Type something:\n");
//         fgets(answer, sizeof answer, stdin);
//         if ((p = strchr(answer, '\n')) != NULL) /*fgets不会像gets那样自动地去掉结尾的\n，所以程序中手动将\n位置处的值变为\0,代表输入的结束。*/
//         {
//                 *p = '\0';
//         }
//         printf("You typed \"%s\"\n", answer);
//         return 0;
// }
