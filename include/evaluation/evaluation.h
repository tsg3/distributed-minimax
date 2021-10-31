/**
 * @file evaluation.h
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Evaluation module header.
 * @version 0.1
 * @date 2021-10-31
 */
#ifndef EVALUATION_H_
#define EVALUATION_H_

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>

/** @typedef struct Measure
 * @brief Contains a double value captured at a certain time.
 */
typedef struct Measure
{
    double value;           /**< Value captured. */
    double time;            /**< Time of measurement. */
    struct Measure* next;   /**< Next measure. */
} Measure;

/** @typedef struct NodeInfo 
 * @brief Contains a list of RAM and CPU measures for an specific node identifier.
 */
typedef struct NodeInfo
{
    char node_id;           /**< Node identifier */
    Measure* cpu_measures;  /**< CPU measures' list. */
    Measure* ram_measures;  /**< RAM measures' list. */
} NodeInfo;

/** @var char node
 * @brief Current node's identifier.
 */

/** @var NodeInfo* node_measures
 * @brief List of node's info structs.
 */
char node;
NodeInfo* node_measures;

/** @var double wall_seconds
 * @brief Wall time elapsed. 
 */

/** @var time_t first_time
 * @brief Start time.
 */

/** @var time_t time_elapsed
 * @brief Time elapsed (not precise; used only by this module).
 */

/** @var time_t last_time
 * @brief Last time measure made.
 */
double wall_seconds;
time_t first_time;
time_t time_elapsed;
time_t last_time;

/** @var unsigned long last_cpu_usage_times
 * @brief Last CPU usage measure made.
 */

/** @var unsigned long hertz
 * @brief Number of clock ticks per second.
 */

/** @var unsigned long num_processors
 * @brief System's number of CPUs. 
 */

/** @var unsigned long page_size
 * @brief System's current page size.
 */
unsigned long last_cpu_usage_times;
unsigned long hertz;
unsigned long num_processors;
unsigned long page_size;

/** @var char* stat_path
 * @brief Path to '/proc/[pid]/stat'.
 */

/** @var char* statm_path
 * @brief Path to '/proc/[pid]/statm'.
 */
char* stat_path;
char* statm_path;

/** @var clock_t cpu_time_elapsed
 * @brief Clock (CPU) time elapsed.
 */
clock_t cpu_time_elapsed;

/** @var pthread_t eval_thread_id
 * @brief Thread identifier.
 */

/** @var bool eval_thread_exec
 * @brief Indicates that minimax execution hasn't finished (when true).
 */
pthread_t eval_thread_id;
bool eval_thread_exec;

/** @fn eval_main
 * @brief Evaluation thread execution loop. 
 * 
 * @return void* (NULL)
 */
void* eval_main();

/** @fn init_evaluation_module
 * @brief Initializes variables.
 * 
 * @return void
 */
void init_evaluation_module();

/** @fn start_time
 * @brief Initializes the start time.
 * 
 * @return void
 */

/** @fn get_current_time
 * @brief Retrieves current time.
 * 
 * @return Current time in seconds.
 */

/** @fn end_time
 * @brief Sets the elapsed time (only used by this module).
 * 
 * @return void
 */
void start_time();
time_t get_current_time();
void end_time();

/** @fn open_file
 * @brief Opens a file.
 * 
 * @param path  File's absolute path.
 * @return Pointer to file descriptor.
 */
FILE* open_file(char*);

/** @fn get_cpu_usage
 * @brief Captures the current CPU usage.
 * 
 * @return void
 */

/** @fn get_ram_usage
 * @brief Captures the current RAM usage.
 * 
 * @return void
 */
void get_cpu_usage(time_t);
void get_ram_usage(time_t);

/** @fn add_measure
 * @brief Adds a measure to a list of measures.
 * 
 * @param list  List of measures.
 * @param value Double value of the new measure.
 * @param time  Time of measurement.
 * @return void
 */
void add_measure(Measure**, double, double);

/** @fn free_node_info
 * @brief Frees the memory allocated for the evaluation module.
 * 
 * @return void
 */
void free_node_info();

#endif