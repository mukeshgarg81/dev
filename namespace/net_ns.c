#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>


static char child_stack[1048576];

static int show_pid() {
  printf("PID: %ld\n", (long)getpid());
}


static int show_iface() {
  printf("New `net` Namespace:\n");
  system("ip link");
  printf("\n\n");
}

static int set_ip() {
  system("ifconfig veth1 192.168.20.100");
  system("ip link");
  printf("\n\n");
}


static int set_new_net_ns()
{
	unshare(CLONE_NEWNET);
}


static int child_cli()
{
        char input[] = " ";
        char *argument;
        while(strcmp(input, "quit")!= 0) {
                printf("$");
                scanf ("%s", input);

                if(strcmp(input,"clear") == 0) {
                        printf("\e[1;1H\e[2J");
                }

                else if(strcmp(input,"show_iface") == 0) {
			show_iface();
                }
                
		else if(strcmp(input,"show_pid") == 0) {
			show_pid();
                }
		
		else if(strcmp(input,"set_new_net_ns") == 0) {
			set_new_net_ns();
                }
		
                else if(strcmp(input, "exit") == 0) {
			printf("Exiting form child\n");
			exit(1);
		}
                else if(strcmp(input, "help") == 0) {
			printf("show_iface	-	Display network interfaces\n");
			printf("show_pid	-	Display pid\n");
			printf("set_new_net_ns	-	set new network namespace\n");	
		}
        }
}


int main() {
  printf("Original `net` Namespace:\n");
  system("ip link");
  printf("\n\n");

  pid_t child_pid = clone(child_cli, child_stack+1048576, CLONE_NEWPID| SIGCHLD, NULL);
  printf("child pid = %d\n",child_pid);

  waitpid(child_pid, NULL, 0);
  return 0;
}
