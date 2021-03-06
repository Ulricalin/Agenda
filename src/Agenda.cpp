#include "AgendaUI.hpp"
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>

void SignHandler(int Signal) {
	std::cout << std::endl;
	exit(0);
}

AgendaUI aui;

int main() {
	signal(SIGINT, SignHandler);
    aui.OperationLoop();
    return 0;
}
