#include <signal.h>
#include <sys/signal.h>

int main(){

	// using SIG_IGN
	signal(SIGINT, SIG_IGN);

	sleep(10);
	signal(SIGINT, SIG_DFL);
	while(1);
}

