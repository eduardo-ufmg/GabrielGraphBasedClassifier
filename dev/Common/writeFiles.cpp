#include "writeFiles.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <variant>
#include <string>
#include <type_traits>
#include <algorithm>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"

using namespace std;

/*

  output format:

  vertex0_id, |, feature0_0, feature0_1, ..., feature0_n, |, cluster0_id, |, adjacent_vertex0_0 - isSupportEdge, adjacent_vertex0_1 - isSupportEdge, ..., adjacent_vertex0_m - isSupportEdge
  vertex1_id, |, feature1_0, feature1_1, ..., feature1_n, |, cluster1_id, |, adjacent_vertex1_0 - isSupportEdge, adjacent_vertex1_1 - isSupportEdge, ..., adjacent_vertex1_m - isSupportEdge
  ...
  vertexk_id, |, featurek_0, featurek_1, ..., featurek_n, |, clusterk_id, |, adjacent_vertexk_0 - isSupportEdge, adjacent_vertexk_1 - isSupportEdge, ..., adjacent_vertexk_m - isSupportEdge

*/

int writeGabrielGraphToFile(const ClusterMap& clusters, const std::string& file_name_with_path)
{
  ofstream outfile(file_name_with_path);

  if (!outfile.is_open()) {
    cerr << "Error: Unable to open file " << file_name_with_path << " for writing." << endl;
    return -1;
  }
  
  // Iterate over each cluster.
  for (const auto& clusterPair : clusters) {
    const ClassType& clusterKey = clusterPair.first;
    const Cluster& cluster = clusterPair.second;
    
    // Iterate over each vertex in the cluster.
    for (const auto& [vertexID, vertexptr] : cluster.vertices) {
      // Write vertex id.
      outfile << vertexID << ", |";
      
      // Write features separated by comma.
      for (const double& f : vertexptr->features) {
        outfile << ", " << f;
      }
      
      // Write cluster id.
      outfile << ", |, " << clusterKey << ", |";
      
      // Write adjacent vertices.
      for (const auto& adj : vertexptr->adjacents) {
        outfile << ", " << adj.first << " - " << adj.second;
      }
      
      outfile << "\n";
    }
  }
  
  outfile.close();
  return 0;
}

/*

output format:

feature0_0, feature0_1, ..., feature0_n, cluster0_id
feature1_0, feature1_1, ..., feature1_n, cluster1_id
...
featurek_0, featurek_1, ..., featurek_n, clusterk_id

*/

int writeVerticesToFile(const ClusterMap& clusters, const std::string& file_name_with_path)
{
  ofstream outfile(file_name_with_path);

  if (!outfile.is_open()) {
    cerr << "Error: Unable to open file " << file_name_with_path << " for writing." << endl;
    return -1;
  }
  
  // Iterate over each cluster.
  for (const auto& clusterPair : clusters) {
    const ClassType& clusterKey = clusterPair.first;
    const Cluster& cluster = clusterPair.second;
    
    // Iterate over each vertex in the cluster.
    for (const auto& [vertexID, vertexptr] : cluster.vertices) {
      // Write features separated by comma.
      for (const double& f : vertexptr->features) {
        outfile << f << ", ";
      }
      
      // Write cluster id.
      outfile << clusterKey << "\n";
    }
  }
  
  outfile.close();
  return 0;
}
