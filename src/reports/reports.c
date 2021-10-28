#include <reports/reports.h>

void write_report(char* report_path)
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

    fwrite(report_path, 1, strlen(report_path), fd);

    fclose(fd);
}

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

cJSON* build_node_info(NodeInfo* node_info)
{
    cJSON* json_node = cJSON_CreateObject();

    cJSON* json_usage = build_measures_array(node_info->cpu_measures);
    cJSON_AddItemToObject(json_node, "cpu", json_usage);

    json_usage = build_measures_array(node_info->ram_measures);
    cJSON_AddItemToObject(json_node, "ram", json_usage);

    return json_node;
}