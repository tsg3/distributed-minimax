/**
 * @file configuration.c
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Minimax configuration module source file.
 * @version 0.1
 * @date 2021-10-31
 */

#include <configuration.h>

/**
 * Executes the configuration of the minimax algorithm.
 * 
 * First, it opens and reads the content of '~/conf_minimax', which is the path of the
 * configuration file.
 * Then, it parses each of the attributes in the JSON-formatted file, and invokes the
 * parsing methods to check if all the data is valid.
 * 
 * In that case, the function returns a pointer to the minimax configuration struct. 
 * If not, it returns NULL.
 */
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
        delete_configuration(conf);
        return NULL;
    }

    return conf;

parsing_error:
    cJSON_Delete(json);
    perror("Error parsing the configuration file");
    delete_configuration(conf);
    return NULL;
}

/**
 * Checks the topology in the configuration file and sets it to a char variable.
 * 
 * The function returns true if the parsing succeds. If not, it returns false.
 */
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

/**
 * Checks the player in the configuration file and sets it to a bool variable.
 * 
 * The function returns true if the parsing succeds. If not, it returns false.
 */
bool check_turn(const cJSON* turn, bool* conf)
{
    if (turn == NULL)
    {
        return false;
    }

    return cJSON_bool_to_bool(turn, conf);
}

/**
 * Checks the iterations in the configuration file and sets them to a int variable.
 * 
 * The function returns true if the parsing succeds. If not, it returns false.
 */
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

/**
 * Checks the nodes selected in the configuration file and sets them to the configuration 
 * struct.
 * 
 * First, it checks if the cJSON item is valid.
 * 
 * Then parses each of the four nodes' boolean value specified in the cJSON array object,
 * and sets this values in the configuration struct.
 * 
 * The function returns true if the parsing succeds. If not, or if the array doesn't have
 * four values, it returns false.
 */
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

        if (size != 5)
        {
            return false;
        }

        return true;
    }

    return false;
}

/**
 * Checks the pieces indicated in the configuration file and sets it to a state
 * configuration struct.
 * 
 * First, it checks if the cJSON item is valid. In that case, the function returns 'true';
 * otherwise, it returns 'false'.
 * 
 * Then parses each of the pieces objects specified in the cJSON array object, checking
 * the validity of each of its attributes (X and Y positions, type and player). If the 
 * parsing succeds, adds that piece to the state struct referenced by the 'state' pointer.
 */
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
        bool player_value;
        char piece_value;

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

            cJSON_bool_to_bool(player, &player_value);
            piece_value = type->valuestring[0];

            if (!(piece_value == 'K' || piece_value == 'Q' || piece_value == 'B'
                || piece_value == 'N' || piece_value == 'R' || piece_value == 'P')
                || x->valueint < 0 || x->valueint > 7
                || y->valueint < 0 || y->valueint > 7)
            {
                return false;
            }

            new_piece = create_piece(piece_value, x->valueint, y->valueint);
            add_piece(state, new_piece, player_value);

            temp = temp->next;
        }

        return true;
    }

    return false;
}

/**
 * Adds a piece in the list of pieces of a player in a chess state struct.
 * 
 * It checks the 'player' boolean parameter to checked to which player have the piece:
 *  true:   White pieces player
 *  false:  Black pieces player
 * 
 * Then, adds the piece to the corresponding list in the chess state struct.
 */
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

/**
 * Sets the value of a bool variable according to the boolean value of a cJSON_bool
 * object.
 * 
 * It also checks if the cJSON item is valid. In that case, the function returns 'true'
 * (boolean value); otherwise, it returns 'false'.
 */
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

/**
 * Prints the variables' values of a configuration struct.
 */
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

/**
 * Frees the memory allocated for a configuration struct variable.
 * 
 * First, frees the state struct allocated in the configuration struct. Then, it proceeds
 * to free the actual configuration struct memory.
 */
void delete_configuration(Configuration* conf)
{
    if (conf->initial_state != NULL)
    {
        delete_state(conf->initial_state);
    }
    free(conf);
}