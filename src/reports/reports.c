/**
 * @file reports.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Reports module source file.
 * @version 0.1
 * @date 2021-10-31
 */

#include <reports/reports.h>

/**
 * Writes a JSON-formatted report file in '~/report.json'.
 */
void write_report(char* report)
{
    char* filename = "/report.json";
    char* path = (char*)malloc(strlen(getenv("HOME")) + strlen(filename) + 1);
    strcpy(path, getenv("HOME"));
    strcat(path, filename);

    FILE* fd = fopen(path, "w");
    if (fd == NULL)
    {
        perror("Error opening the file");
        return;
    }

    free(path);

    fwrite(report, 1, strlen(report), fd);

    fclose(fd);
}

/**
 * Create a cJSON array from a series of measures. 
 * 
 * Each one of this measures will be represented as a cJSON object, with two double
 * values: 'porcentaje' and 'time'. Finally, it returns the cJSON array.
 */
cJSON* build_measures_array(Measure* measures)
{
    cJSON* json_usage = cJSON_CreateArray();
    cJSON *json_measure, *json_measure_value, *json_measure_time;
    Measure* temp = measures;

    while (temp != NULL)
    {
        json_measure = cJSON_CreateObject();

        json_measure_value = cJSON_CreateNumber(temp->value);
        cJSON_AddItemToObject(json_measure, "porcentaje", json_measure_value);

        json_measure_time = cJSON_CreateNumber(temp->time);
        cJSON_AddItemToObject(json_measure, "tiempo", json_measure_time);

        cJSON_AddItemToArray(json_usage, json_measure);

        temp = temp->next;
    }

    return json_usage;
}

/**
 * Compiles all captured info from the evaluation module into a report.
 */
void export_report()
{
    NodeInfo *node_2 = NULL, *node_3 = NULL, *node_4 = NULL; // All nodes support

    cJSON* json_object = cJSON_CreateObject();

    cJSON* json_error_found = cJSON_CreateFalse(); // Suppose error_found = false
    cJSON_AddItemToObject(json_object, "error_encontrado", json_error_found);

    cJSON* json_error = cJSON_CreateString(""); // Suppose error = ""
    cJSON_AddItemToObject(json_object, "error", json_error);

    cJSON* json_wall_time = cJSON_CreateNumber(wall_seconds);
    cJSON_AddItemToObject(json_object, "tiempo_real", json_wall_time);

    cJSON* json_cpu_time = cJSON_CreateNumber(((double)cpu_time_elapsed) / CLOCKS_PER_SEC);
    cJSON_AddItemToObject(json_object, "tiempo_cpu", json_cpu_time);

    cJSON* json_nodes = cJSON_CreateObject();

    cJSON* json_node_1 = build_node_info(node_measures);
    cJSON_AddItemToObject(json_nodes, "nodo_1", json_node_1);

    cJSON* json_node_temp;
    if (node_2 != NULL)
    {
        json_node_temp = build_node_info(node_2);
        cJSON_AddItemToObject(json_nodes, "nodo_2", json_node_temp);
    }
    if (node_3 != NULL)
    {
        json_node_temp = build_node_info(node_3);
        cJSON_AddItemToObject(json_nodes, "nodo_3", json_node_temp);
    }
    if (node_4 != NULL)
    {
        json_node_temp = build_node_info(node_4);
        cJSON_AddItemToObject(json_nodes, "nodo_4", json_node_temp);
    }

    cJSON_AddItemToObject(json_object, "nodos", json_nodes);

    char* json_string = cJSON_Print(json_object);

    write_report(json_string);

    cJSON_Delete(json_object);
    free(json_string);
}

/**
 * Creates a cJSON object for the information captured in a node.
 * 
 * It returns a cJSON object containing all the data captured from the NodeInfo struct
 * parameter.
 */
cJSON* build_node_info(NodeInfo* node_info)
{
    cJSON* json_node = cJSON_CreateObject();

    cJSON* json_usage = build_measures_array(node_info->cpu_measures);
    cJSON_AddItemToObject(json_node, "cpu", json_usage);

    json_usage = build_measures_array(node_info->ram_measures);
    cJSON_AddItemToObject(json_node, "ram", json_usage);

    return json_node;
}