#include <evaluation/evaluation.h>

void init_evaluation_module()
{
    CPU_list = NULL;
}

void start_time()
{
    time_elapsed = time(NULL);
}

time_t get_current_time()
{
    return time(NULL);
}

void end_time()
{
    time_elapsed = time(NULL) - time_elapsed;
}

double get_time_in_double()
{
    return (double) time_elapsed;
}

void get_cpu_usage()
{
    char* prefix = "/proc/";
    char* suffix = "/stat";

    pid_t pid = getpid();

    char* pid_str = (char*)malloc(sizeof(int) * 6);
    sprintf(pid_str, "%d", pid);

    char* path = (char*)malloc(strlen(prefix) + strlen(pid_str) + strlen(suffix) + 1);
    strcpy(path, prefix);
    strcpy(path + strlen(prefix), pid_str);
    strcat(path + strlen(prefix) + strlen(pid_str), suffix);

    FILE* fd = fopen(path, "r");
    if (fd == NULL)
    {
        perror("Error opening the file");
        free(path);
        return;
    }

    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    unsigned long utime, stime;
    long cutime, cstime;

    double start_period = get_time_in_double();
    double end_period = (double) get_current_time();
    double period = end_period - start_period;

    long hertz = sysconf(_SC_CLK_TCK);

    fscanf(fd, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld", 
        &utime, &stime, &cutime, &cstime);

    double usage;
    if (period == 0.0)
    {
        usage = 100.0;
    }
    else
    {
        usage = 100 * (((double) (utime + stime + cutime + cstime) / hertz) / period);
    }

    // printf("Hello world from %d!\n%lu %lu %ld %ld %f %f %ld %f %f\n", 
    //     pid, utime, stime, cutime, cstime, start_period, end_period, hertz, 
    //     period, usage);

    fclose(fd);
    free(path);
    free(pid_str);

    add_cpu_measure(usage, end_period);
}

void add_cpu_measure(double cpu, double time)
{
    CPU_measure* new_measure = (CPU_measure*)malloc(sizeof(CPU_measure));
    new_measure->usage = cpu;
    new_measure->time = time;
    new_measure->next = NULL;

    if (CPU_list != NULL)
    {
        CPU_measure* temp = CPU_list;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_measure;
    }
    
    else
    {
        CPU_list = new_measure;
    }
}

void free_cpu_list()
{
    CPU_measure* temp = CPU_list;
    while (CPU_list != NULL)
    {
        temp = CPU_list;
        CPU_list = temp->next;
        free(temp);
    }
}