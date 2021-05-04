#ifndef CONVERTER_SIM_CONVERTER_H
#define CONVERTER_SIM_CONVERTER_H

#define MAX_MODEL_LENGTH 100
#define MAX_LINE_LENGTH 1000
#define SCENARIO_NAME "experiment_1"
#define SCENARIO_DATE "15 Dec 2020"

typedef struct model {
    char name[MAX_MODEL_LENGTH];
    struct model *next;
} model_t;

model_t *model_head = NULL;

typedef struct output_port {
    char name[MAX_MODEL_LENGTH];
    struct output_port *next;
} output_port_t;

output_port_t *output_port_head = NULL;

/**
 * @brief Add a new model instance to a linked-list if it does not exist.
 * @param model_id string containing the model name instance
 */
void add_model(char *model_id);

/**
 * @brief Add a new output port to a linked-list if it does not exist.
 * @param output_port_id
 */
void add_output_port(char *output_port_id);

#endif //CONVERTER_SIM_CONVERTER_H
