#include <configuration.h>

Configuration* configure_minimax()
{
    char* filename = "/conf_minimax";
    char* path = (char*)malloc(strlen(getenv("HOME")) + strlen(filename) + 1);
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
    Configuration* conf = (Configuration*)malloc(sizeof(Configuration));
    
    const cJSON* topology = cJSON_GetObjectItemCaseSensitive(json, "topologia");
    if (!check_topology(topology, &(conf->topology)))
    {
        goto parsing_error;
    }

    const cJSON* turn = cJSON_GetObjectItemCaseSensitive(json, "turno");
    if (!check_turn(turn, &(conf->turn)))
    {
        goto parsing_error;
    }

    const cJSON* iters = cJSON_GetObjectItemCaseSensitive(json, "iteraciones");
    if (!check_iters(iters, &(conf->iters)))
    {
        goto parsing_error;
    }

    const cJSON* nodes = cJSON_GetObjectItemCaseSensitive(json, "nodos");
    if (!check_nodes(nodes, conf))
    {
        goto parsing_error;
    }

    conf->initial_state = create_state(conf->turn, 7, 7, NULL, false);
    const cJSON* chess_table = cJSON_GetObjectItemCaseSensitive(json, "tablero");
    if (!check_pieces(chess_table, conf->initial_state))
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
    if (conf->initial_state != NULL)
    {
        delete_state(conf->initial_state);
    }
    free(conf);
    return NULL;
}

bool check_topology(const cJSON* topology, char* conf)
{
    if (topology == NULL)
    {
        return false;
    }

    else if (cJSON_IsString(topology))
    {
        if (strcmp(topology->valuestring, "Completamente enlazada") == 0)
        {
            *conf = '0';
            return true;
        }
        else if (strcmp(topology->valuestring, "Anillo") == 0)
        {
            *conf = '1';
            return true;
        }
        else if (strcmp(topology->valuestring, "Estrella") == 0)
        {
            *conf = '2';
            return true;
        }
        return false;
    }

    return false;
}

bool check_turn(const cJSON* turn, bool* conf)
{
    if (turn == NULL)
    {
        return false;
    }

    return cJSON_bool_to_bool(turn, conf);
}

bool check_iters(const cJSON* iters, int* conf)
{
    if (iters == NULL)
    {
        return false;
    }

    else if (cJSON_IsNumber(iters))
    {
        *conf = iters->valueint;
        return true;
    }

    return false;
}

bool check_nodes(const cJSON* nodes, Configuration* conf)
{
    if (nodes == NULL)
    {
        return false;
    }

    else if (cJSON_IsArray(nodes))
    {
        int size = 1; 
        bool res;
        cJSON* temp = nodes->child;

        while (temp != NULL)
        {
            if (size == 1)
            {
                res = cJSON_bool_to_bool(temp, &(conf->node_1));
            }
            else if (size == 2)
            {
                res = cJSON_bool_to_bool(temp, &(conf->node_2));
            }
            else if (size == 3)
            {
                res = cJSON_bool_to_bool(temp, &(conf->node_3));
            }
            else if (size == 4)
            {
                res = cJSON_bool_to_bool(temp, &(conf->node_4));
            }
            else if (size > 4)
            {
                return false;
            }

            if (!res)
            {
                return false;
            }

            temp = temp->next;
            size++;
        }

        return true;
    }

    return false;
}

bool check_pieces(const cJSON* item, State* state)
{
    if (item == NULL || state == NULL)
    {
        return false;
    }

    else if (cJSON_IsArray(item))
    {
        cJSON* temp = item->child;
        cJSON *type, *x, *y, *player;
        Piece* new_piece;
        bool bool_value;

        while (temp != NULL)
        {
            if (!cJSON_IsObject(temp))
            {
                return false;
            }

            type = cJSON_GetObjectItemCaseSensitive(temp, "tipo");
            x = cJSON_GetObjectItemCaseSensitive(temp, "posicionX");
            y = cJSON_GetObjectItemCaseSensitive(temp, "posicionY");
            player = cJSON_GetObjectItemCaseSensitive(temp, "jugador");

            if (type == NULL || x == NULL 
                || y == NULL || player == NULL)
            {
                return false;
            }

            if (!(cJSON_IsString(type) && cJSON_IsNumber(x) 
                && cJSON_IsNumber(y) && cJSON_IsBool(player)))
            {
                return false;
            }

            cJSON_bool_to_bool(player, &bool_value);
            new_piece = create_piece(type->valuestring[0], x->valueint, y->valueint);
            add_piece(state, new_piece, bool_value);

            temp = temp->next;
        }

        return true;
    }

    return false;
}

void add_piece(State* state, Piece* piece, bool player)
{
    if (player)
    {
        if (state->whitePieces == NULL)
        {
            state->whitePieces = piece;
        }

        else
        {
            Piece* temp = state->whitePieces;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = piece;
        }
    }

    else
    {
        if (state->blackPieces == NULL)
        {
            state->blackPieces = piece;
        }
        
        else
        {
            Piece* temp = state->blackPieces;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = piece;
        }
    }
}

bool cJSON_bool_to_bool(const cJSON* item, bool* var)
{
    if (cJSON_IsTrue(item))
    {
        *var = true;
        return true;
    }

    else if (cJSON_IsFalse(item))
    {
        *var = false;
        return true;
    }

    return false;
}

void print_configuration(Configuration* conf)
{
    if (conf == NULL)
    {
        perror("Configuration struct is 'NULL'!");
        return;
    }

    printf(" << Configuration struct: >> \n"
        " | Topología : %c\n"
        " | Nodo 1 : %d\n"
        " | Nodo 2 : %d\n"
        " | Nodo 3 : %d\n"
        " | Nodo 4 : %d\n"
        " | Turno : %d \n"
        " | Iteraciones: %d\n"
        " | Tablero: \n",
        conf->topology, conf->node_1, conf->node_2, conf->node_3,
        conf->node_4, conf->turn, conf->iters);
    
    Piece* temp = conf->initial_state->whitePieces;
    printf(" |   Piezas blancas: \n");
    while (temp != NULL)
    {
        printf(" |     %c : (%d, %d) \n", 
            temp->type, temp->posX, temp->posY);
        temp = temp->next;
    }
    
    temp = conf->initial_state->blackPieces;
    printf(" |   Piezas negras: \n");
    while (temp != NULL)
    {
        printf(" |     %c : (%d, %d) \n", 
            temp->type, temp->posX, temp->posY);
        temp = temp->next;
    }
}