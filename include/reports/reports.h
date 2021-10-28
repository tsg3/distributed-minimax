#ifndef REPORTS_H_
#define REPORTS_H_

#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <evaluation/evaluation.h>

void write_report(char*);
cJSON* build_measures_array(Measure*);
void export_report();
cJSON* build_node_info(NodeInfo*);

#endif