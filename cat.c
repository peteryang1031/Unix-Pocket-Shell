#include"Header.h"
void cat_command(char*shell);
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