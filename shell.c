#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
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
void ls_subprint(char c)
{
        char name[60];
        char path[60];
        if (getcwd(path, sizeof(path)) == NULL)
        {
                perror("getcwd");
                exit(1);
        }
        struct dirent *dp;
        DIR *dfd;
        if ((dfd = opendir(path)) == NULL)
        {
                perror("opendir");
                exit(1);
        }
        while ((dp = readdir(dfd)) != NULL)
        {
                if ((c == 0 || c == 'A') && (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0))
                {
                        continue;
                }
                if(c==0&&dp->d_name[0]=='.'){
                        continue;
                }
                printf("%s ",dp->d_name);
        }
        printf("\n");
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
                while (isalpha(*(pos + temp)) || *(pos + temp) == '-')
                {
                        para[paracnt][temp] = *(pos + temp);
                        temp++;
                }
                para[paracnt][temp] = '\0';
                shell = pos + temp;
                paracnt++;
        }
        para[paracnt] = NULL;
        if (paracnt == 1)
        { //case1:ls无参数(不显示隐藏文件)
                ls_subprint(0);  
        }
        else
        {
                for (int i = 1; i < paracnt; i++)
                {
                        if (strcmp(para[i], "-a")==0)
                        {
                                ls_subprint('a');
                                break;
                        }
                        if(strcmp(para[i],"-A")==0){
                                ls_subprint('A');
                        }
                }
        }
        //execvp("ls", para);
        //perror("ls command");
        exit(1);
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
void cat_command(char *shell)
{
        int fd;
        int paracnt = 1;
        char *para[5];
        char buf[1024];
        for (int u = 0; u < 5; u++)
        {
                para[u] = (char *)malloc(10 * sizeof(char));
        }
        para[0] = "cat";
        int temp = 3;
        int i = 0;
        while (shell[temp] == ' ')
        {
                temp++;
        }
        while (shell[temp] != ' ' && shell[temp] != '\n')
        {
                para[1][i++] = shell[temp++];
        }
        para[1][i] = '\0';
        //
        if (para[1][0] == '\0') //case1:没有任何参数，输入到标准输入输出到标准输出
        {
                printf(">");
                fflush(stdout);
                while (fgets(buf, sizeof(buf), stdin))
                {
                        printf("%s", buf);
                        printf(">");
                        fflush(stdout);
                }
        }
        else if ((strstr(shell, "<<") != NULL) && (strstr(shell, "EOF") != NULL))
        {
                temp = 3;
                i = 0;
                while (isalnum(shell[temp]) == 0)
                {
                        temp++;
                }
                while (isalnum(shell[temp]))
                {
                        para[1][i++] = shell[temp++];
                }
                para[1][i] = '\0';
                if (strstr(shell, ">>") == NULL) //case 2:创建新文件
                {

                        int fd = open(para[1], O_RDWR | O_CREAT, 0644);
                        if (fd < 0)
                        {
                                perror("Create");
                                exit(1);
                        }
                        int read_byte;
                        printf(">");
                        fflush(stdout);
                        while ((read_byte = read(STDIN_FILENO, buf, 1024)))
                        {
                                int write_byte = write(fd, buf, read_byte);
                                if (write_byte < 0)
                                {
                                        perror("Write");
                                        exit(1);
                                }
                                printf(">");
                                fflush(stdout);
                        }
                }
                else //case3:追加文件内容
                {
                        int fd = open(para[1], O_RDWR | O_APPEND);
                        if (fd < 0)
                        {
                                perror("Append");
                                exit(1);
                        }
                        int read_byte;
                        printf(">");
                        fflush(stdout);
                        while ((read_byte = read(STDIN_FILENO, buf, 1024)))
                        {
                                int write_byte = write(fd, buf, read_byte);
                                if (write_byte < 0)
                                {
                                        perror("Write");
                                        exit(1);
                                }
                                printf(">");
                                fflush(stdout);
                        }
                }
        }
        else //case4:把已存在文件内容读到标准输出
        {

                fd = open(para[1], O_RDONLY);
                int read_byte = read(fd, buf, sizeof(buf));
                if (read_byte < 0)
                {
                        perror("read");
                        exit(1);
                }
                while (read_byte != 0)
                {
                        int write_byte = write(STDOUT_FILENO, buf, read_byte);
                        if (write_byte < 0)
                        {
                                perror("write");
                                exit(1);
                        }
                        read_byte = read(fd, buf, sizeof(buf));
                        if (read_byte < 0)
                        {
                                perror("write");
                                exit(1);
                        }
                }
                close(fd);
                // para[2]=NULL;
                // execvp("cat",para);
                // perror("cat command");
        }
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
