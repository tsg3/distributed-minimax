#include <evaluation/evaluation.h>

void* eval_main()
{
    init_evaluation_module();

    time_t current_time;

    while (eval_thread_exec)
    {
        sleep(5); // This is the time between CPU usage and RAM usage measures

        current_time = (double) get_current_time();
        get_cpu_usage(current_time);
        get_ram_usage(current_time);
        last_time = current_time;
        // last_time = current_time;
    }

    end_time();

    pthread_exit(NULL);
}


void init_evaluation_module()
{
    CPU_list = NULL;

    char* prefix = "/proc/";
    pid_t pid = getpid();
    char* pid_str = (char*)malloc(sizeof(int) * 6);
    sprintf(pid_str, "%d", pid);
    char* suffix_1 = "/stat";
    char* suffix_2 = "/statm";

    int length = strlen(prefix) + strlen(pid_str) + strlen(suffix_1) + 1;

    stat_path = (char*)malloc(length);
    strcpy(stat_path, prefix);
    strcpy(stat_path + strlen(prefix), pid_str);
    strcat(stat_path + strlen(prefix) + strlen(pid_str), suffix_1);

    statm_path = (char*)malloc(length + 1);
    strcpy(statm_path, prefix);
    strcpy(statm_path + strlen(prefix), pid_str);
    strcat(statm_path + strlen(prefix) + strlen(pid_str), suffix_2);

    free(pid_str);

    eval_thread_exec = true;
    
    last_cpu_usage_times = 0;
    hertz = sysconf(_SC_CLK_TCK);
    num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    page_size = sysconf(_SC_PAGESIZE) / 1024;

    start_time();
}

void start_time()
{
    first_time = time(NULL);
    last_time = first_time;
    // last_time = first_time;
}

time_t get_current_time()
{
    return time(NULL);
}

void end_time()
{
    time_elapsed = time(NULL) - first_time;
}

FILE* open_file(char* path)
{
    FILE* fd = fopen(path, "r");
    if (fd == NULL)
    {
        perror("Error opening the file");
    }
    return fd;
}

void get_cpu_usage(time_t current_time)
{
    FILE* fd = open_file(stat_path);
    if (fd == NULL)
    {
        return;
    }

    unsigned long utime, stime;
    long cutime, cstime;

    // double period = (double)(current_time - last_time);
    double period = (double)(current_time - last_time);

    fscanf(fd, "%*d %*s %*c %*d %*d %*d %*d %*d %*u %*u %*u %*u %*u %lu %lu %ld %ld", 
        &utime, &stime, &cutime, &cstime);

    double usage;
    unsigned long total = utime + stime + cutime + cstime;

    if (period == 0.0)
    {
        usage = 0.0;
    }
    else
    {
        usage = (100 / num_processors) * ((
            (double) (total - last_cpu_usage_times) 
            / hertz) / period);
    }

    last_cpu_usage_times = total;

    fclose(fd);

    add_measure(&CPU_list, usage, (double)(current_time - first_time));
}

void get_ram_usage(time_t current_time)
{
    FILE* fd = open_file(statm_path);
    if (fd == NULL)
    {
        return;
    }

    double resident;

    fscanf(fd, "%*d %lf %*d %*d %*d %*d %*d", &resident);

    resident *= page_size; // pages to KB
    double moment = (double) current_time;

    fclose(fd);

    add_measure(&RAM_list, resident, (double)(current_time - first_time));
}

void add_measure(Measure** list, double value, double time)
{
    Measure* new_measure = (Measure*)malloc(sizeof(Measure));
    new_measure->value = value;
    new_measure->time = time;
    new_measure->next = NULL;

    if (*list != NULL)
    {
        Measure* temp = *list;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = new_measure;
    }
    
    else
    {
        *list = new_measure;
    }
}

void free_lists()
{
    Measure* temp = CPU_list;
    while (CPU_list != NULL)
    {
        temp = CPU_list;
        CPU_list = temp->next;
        free(temp);
    }

    temp = RAM_list;
    while (RAM_list != NULL)
    {
        temp = RAM_list;
        RAM_list = temp->next;
        free(temp);
    }

    if (stat_path != NULL)
    {
        free(stat_path);
    }

    if (statm_path != NULL)
    {
        free(statm_path);
    }
}