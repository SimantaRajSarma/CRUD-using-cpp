#include "Controller.h"

int main() {
    Controller controller("http://localhost:8080");
    controller.start();
    return 0;
}
