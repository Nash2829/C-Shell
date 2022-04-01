#include "headers.h"

void present_working_directory() {
    getcwd(current_work_dir, MAX_SZ);
    printf("%s\n", current_work_dir);
}