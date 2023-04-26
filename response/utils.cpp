// // #include "response.hpp"
// #include <unistd.h>
// #include <stdlib.h>
// #include <string>

// int main(int ac, char **av, char **env)
// {

//     char *argv[] = {(char*)"/bin/echo", (char*)"index.php", NULL};

//     pid_t pid;
//     int t_fd[2];
//     pipe(t_fd);
//     if((pid = fork()) == 0)
//     {
//         close(t_fd[1]);
//         dup2(t_fd[1], 1);
//         close (t_fd[1]);
//         execve(argv[0], argv, env);
//         exit(1);
//     }
//     else{

//         close( t_fd[1]);
//         char buf[1024];
//         read(t_fd[0], buf, sizeof(buf));
//         write(1, "here ", 5);
//         write(1, buf, strlen(buf));
//         write(1, "\n", 1);
//         close( t_fd[0]);
//         int tt;
//         waitpid(pid, &tt, 0);
//     }
//     return 0;
// }

 