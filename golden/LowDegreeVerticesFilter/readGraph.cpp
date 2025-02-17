#include "readGraph.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

#include "graphTypes.hpp"
#include "stringHelpers.hpp"

using namespace std;

int readGraph(ClusterMap& clusters, const std::string& input_file_name_with_path)
{
  ifstream input_file(input_file_name_with_path.c_str());

  if (!input_file.is_open()) {
    cerr << "Error: could not open file " << input_file_name_with_path << endl;
    return 1;
  }
  
  string line;
  // Process each line of the file.
  while(getline(input_file, line)) {
    // Skip empty lines.
    if (line.empty()) {
      continue;
    }
    
    // Split the line using the literal delimiter ", |, "
    // Expected parts:
    //   0: vertex id
    //   1: features (comma-separated list)
    //   2: cluster id
    //   3: adjacent vertices (comma-separated list)
    vector<string> parts = split(line, ", |, ");

    if (parts.size() != 4) {
      cerr << "Warning: unexpected format in line: " << line << endl;
      continue;
    }
    
    // --- Parse Vertex ID ---
    string vertexIdStr = trim(parts[0]);
    VertexID_t vertexId;

    try {
      vertexId = static_cast<VertexID_t>(stoul(vertexIdStr));
    } catch (const std::exception& e) {
      cerr << "Error converting vertex id '" << vertexIdStr << "' to unsigned: " << e.what() << endl;
      return 1;
    }
    
    // --- Parse Features ---
    string featuresStr = trim(parts[1]);
    vector<string> featureTokens = split(featuresStr, ",");
    vector<double> features;

    for (const auto& token : featureTokens) {
      string trimmedToken = trim(token);

      if (!trimmedToken.empty()) {
        try {
          features.push_back(stod(trimmedToken));
        } catch (const std::exception& e) {
          cerr << "Error converting feature '" << trimmedToken << "' to double in line: " << line << endl;
          return 1;
        }
      }

    }
    
    // --- Parse Cluster ID ---
    string clusterIdStr = trim(parts[2]);
    // Determine whether to treat the cluster id as an unsigned or string.
    bool isClusterIdUnsigned = !clusterIdStr.empty() && all_of(clusterIdStr.begin(), clusterIdStr.end(), ::isdigit);

    ClassType clusterKey;

    if (isClusterIdUnsigned) {
      try {
        clusterKey = static_cast<unsigned>(stoul(clusterIdStr));
      } catch (const std::exception& e) {
        cerr << "Error converting cluster id '" << clusterIdStr << "' to unsigned in line: " << line << endl;
        return 1;
      }
    } else {
      clusterKey = clusterIdStr;
    }
    
    // --- Parse Adjacent Vertices ---
    string adjacentsStr = trim(parts[3]);
    vector<string> adjacentTokens = split(adjacentsStr, ",");
    vector<VertexID_t> adjacents;

    for (const auto& token : adjacentTokens) {
      string trimmedToken = trim(token);

      if (!trimmedToken.empty()) {
        try {
          adjacents.push_back(static_cast<VertexID_t>(stoul(trimmedToken)));
        } catch (const std::exception& e) {
          cerr << "Error converting adjacent vertex id '" << trimmedToken << "' to unsigned in line: " << line << endl;
          return 1;
        }
      }

    }
    
    // --- Create and Insert Vertex ---

    // Insert the vertex into the appropriate cluster.
    clusters.emplace(clusterKey, Cluster());

    clusters[clusterKey].vertices.emplace(vertexId, make_shared<Vertex>(Vertex()));

    clusters[clusterKey].vertices[vertexId]->features = features;
    clusters[clusterKey].vertices[vertexId]->adjacents = adjacents;

  }
  
  input_file.close();

  return 0;
}
