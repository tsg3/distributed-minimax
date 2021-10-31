/**
 * @file reports.h
 * @author Esteban Campos Granados (este0111@hotmail.com)
 * @brief Reports module header.
 * @version 0.1
 * @date 2021-10-31
 */
#ifndef REPORTS_H_
#define REPORTS_H_

#include <cJSON.h>
#include <stdio.h>
#include <stdlib.h>
#include <evaluation/evaluation.h>

/** @fn write_report
 * @brief Writes a JSON-formatted report file.
 * 
 * @param report    String containing the JSON-formatted report.
 * @return void
 */
void write_report(char*);

/** @fn build_measures_array
 * @brief Creates a cJSON array from a series of measures.
 * 
 * @param measures  Pointer to a list of measures
 * @return cJSON array containing all the measures listed.
 */
cJSON* build_measures_array(Measure*);

/** @fn export_report
 * @brief Compiles all captured info from the evaluation module into a report.
 * 
 * @return void
 */
void export_report();

/** @fn build_node_info
 * @brief Creates a cJSON object for the information captured in a node.
 * 
 * @param node_info NodeInfo struct with the data captured from a specific node.
 * @return cJSON object containing all the data from the NodeInfo struct.
 */
cJSON* build_node_info(NodeInfo*);

#endif