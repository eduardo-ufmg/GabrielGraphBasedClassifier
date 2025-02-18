#include "computeGabrielGraph.hpp"

#include <cmath>
#include <vector>
#include <limits>

#include "graphTypes.hpp"

using namespace std;

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b);

void computeGabrielGraph(ClusterMap& clusters)
{
  // Collect all vertices from all clusters.
  VertexMap allVertices;

  for (auto& cluster : clusters) {
    Cluster& c = cluster.second;

    for (auto& vertex : c.vertices) {
      allVertices.insert(vertex);
    }

  }
  
  size_t N = allVertices.size();

  // For each pair of vertices (pi, pj)
  for (size_t i = 0; i < N; ++i) {

    if (allVertices.find(i) == allVertices.end()) {
      continue;
    }

    auto& pi = allVertices[i];

    for (size_t j = i + 1; j < N; ++j) {

      if (allVertices.find(j) == allVertices.end()) {
        continue;
      }

      auto& pj = allVertices[j];

      double d2 = squaredDistance(pi->features, pj->features);
      bool validEdge = true;

      // Check the Gabriel condition against every other vertex z.
      for (size_t k = 0; k < N; ++k) {
        if (k == i || k == j) {
          continue;
        }

        auto& pk = allVertices[k];
        
        // If for any other vertex the sum of squared distances is less than d2,
        // then the sphere with (pi, pj) as diameter contains pk.
        if (squaredDistance(pi->features, pk->features) + 
          squaredDistance(pj->features, pk->features) < d2) {
          validEdge = false;
          break;
        }

      }

      // If the edge passes the test, add each vertex to the other's adjacency list.
      if (validEdge) {
        bool isSE = pi->cluster != pj->cluster;

        pi->adjacents.push_back({j, isSE});
        pj->adjacents.push_back({i, isSE});
      }

    }

  }
}

double squaredDistance(const std::vector<double>& a, const std::vector<double>& b)
{
  double sum = 0.0;
  size_t n = a.size();

  if (n != b.size()) {
   return numeric_limits<double>::infinity();
  }

  for (size_t i = 0; i < n; ++i) {
    double diff = a[i] - b[i];
    sum += diff * diff;
  }

  return sum;
}