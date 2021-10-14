#include <configuration.h>

configuration* configure_minimax()
{
    char* filename = "/conf_minimax";
    char* path = (char*)malloc(strlen(getenv("HOME") + strlen(filename) + 1));
    strcpy(path, getenv("HOME"));
    strcat(path, filename);

    FILE* fd = fopen(path, "r");
    if (fd == NULL)
    {
        perror("Error opening the file");
        free(path);
        return NULL;
    }
    free(path);

    fseek(fd, 0, SEEK_END);
    long fsize = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    char* buffer = (char*)malloc(fsize + 1);
    fread(buffer, 1, fsize, fd);

    cJSON* json = cJSON_Parse(buffer);
    free(buffer);
    configuration* conf = (configuration*)malloc(sizeof(configuration));
    
    const cJSON* topology = cJSON_GetObjectItemCaseSensitive(json, "topologia");
    if (!check_topology(topology, conf))
    {
        goto parsing_error;
    }

    const cJSON* turn = cJSON_GetObjectItemCaseSensitive(json, "turno");
    if (!check_turn(turn, conf))
    {
        goto parsing_error;
    }

    const cJSON* iters = cJSON_GetObjectItemCaseSensitive(json, "iteraciones");
    if (!check_iters(iters, conf))
    {
        goto parsing_error;
    }

    cJSON_Delete(json);

    int cl = fclose(fd);
    if (cl == EOF)
    {
        perror("Error closing the file");
        free(conf);
        return NULL;
    }

    return conf;

parsing_error:
    cJSON_Delete(json);
    perror("Error parsing the configuration file");
    free(conf);
    return NULL;
}

bool check_topology(const cJSON* topology, configuration* conf)
{
    if (topology == NULL)
    {
        return false;
    }
    else if (cJSON_IsString(topology))
    {
        if (strcmp(topology->valuestring, "Completamente enlazada") == 0)
        {
            conf->topology = '0';
            return true;
        }
        else if (strcmp(topology->valuestring, "Anillo") == 0)
        {
            conf->topology = '1';
            return true;
        }
        else if (strcmp(topology->valuestring, "Estrella") == 0)
        {
            conf->topology = '2';
            return true;
        }
        return false;
    }
    return false;
}

bool check_turn(const cJSON* turn, configuration* conf)
{
    if (turn == NULL)
    {
        return false;
    }
    else if (cJSON_IsTrue(turn))
    {
        conf->turn = true;
        return true;
    }
    else if (cJSON_IsFalse(turn))
    {
        conf->turn = false;
        return true;
    }
    else
    {
        return false;
    }
}

bool check_iters(const cJSON* iters, configuration* conf)
{
    if (iters == NULL)
    {
        return false;
    }
    else if (cJSON_IsNumber(iters))
    {
        conf->iters = iters->valueint;
        return true;
    }
    else
    {
        return false;
    }
}