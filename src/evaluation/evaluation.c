/**
 * @file evaluation.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Evaluation module source file.
 * @version 0.1
 * @date 2021-10-31
 */

#include <evaluation/evaluation.h>

/**
 * Evaluation thread execution loop. 
 * 
 * First, it initialize all variables needed for the evaluation module. Then, it
 * continuously captures CPU and RAM usage until minimax execution finishes.
 */
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
    }

    end_time();

    pthread_exit(NULL);
}

/**
 * Initializes variables.
 */
void init_evaluation_module()
{
    node_measures = (NodeInfo*)malloc(sizeof(NodeInfo));
    node_measures->node_id = node;
    node_measures->cpu_measures = NULL;
    node_measures->ram_measures = NULL;

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
    strcpy(stat_path + strlen(prefix) + strlen(pid_str), suffix_1);

    statm_path = (char*)malloc(length + 1);
    strcpy(statm_path, prefix);
    strcpy(statm_path + strlen(prefix), pid_str);
    strcpy(statm_path + strlen(prefix) + strlen(pid_str), suffix_2);

    free(pid_str);

    eval_thread_exec = true;
    
    last_cpu_usage_times = 0;
    hertz = sysconf(_SC_CLK_TCK);
    num_processors = sysconf(_SC_NPROCESSORS_ONLN);
    page_size = sysconf(_SC_PAGESIZE) / 1024;

    start_time();
}

/**
 * Initializes the start time.
 */
void start_time()
{
    first_time = time(NULL);
    last_time = first_time;
}

/**
 * Retrieves current time in seconds.
 */
time_t get_current_time()
{
    return time(NULL);
}

/**
 * Sets the elapsed time (only used by this module).
 */
void end_time()
{
    time_elapsed = time(NULL) - first_time;
}

/**
 * Opens a file at a specific path and returns its file descriptor.
 */
FILE* open_file(char* path)
{
    FILE* fd = fopen(path, "r");
    if (fd == NULL)
    {
        perror("Error opening the file");
    }
    return fd;
}

/**
 * Captures the current CPU usage.
 * 
 * After opening the file, it looks for the cpu time consumed on the last period of time.
 * Then, it computes the percentage of CPU usage, and adds it to the node's CPU measures
 * list. 
 */
void get_cpu_usage(time_t current_time)
{
    FILE* fd = open_file(stat_path);
    if (fd == NULL)
    {
        return;
    }

    unsigned long utime, stime;
    long cutime, cstime;

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
        usage = 100 * (((double) (total - last_cpu_usage_times) 
            / hertz) / period);
    }

    last_cpu_usage_times = total;

    fclose(fd);

    add_measure(&(node_measures->cpu_measures), 
        usage, (double)(current_time - first_time));
}

/**
 * Captures the current RAM usage.
 * 
 * After opening the file, it looks for the resident set size (Rss) reported in pages.
 * Then, it converts this value to kilobytes (KB), and adds it to the node's RAM measures
 * list. 
 */
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

    add_measure(&(node_measures->ram_measures), 
        resident, (double)(current_time - first_time));
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

/**
 * Frees the memory allocated for the evaluation module.
 * 
 * It deallocates the RAM and CPU lists, the NodeInfo struct, and the char arrays
 * 'stat_path' and 'statm_path' for the files.
 */
void free_node_info()
{
    Measure* temp = node_measures->cpu_measures;
    while (node_measures->cpu_measures != NULL)
    {
        temp = node_measures->cpu_measures;
        node_measures->cpu_measures = temp->next;
        free(temp);
    }

    temp = node_measures->ram_measures;
    while (node_measures->ram_measures != NULL)
    {
        temp = node_measures->ram_measures;
        node_measures->ram_measures = temp->next;
        free(temp);
    }

    free(node_measures);

    if (stat_path != NULL)
    {
        free(stat_path);
    }

    if (statm_path != NULL)
    {
        free(statm_path);
    }
}