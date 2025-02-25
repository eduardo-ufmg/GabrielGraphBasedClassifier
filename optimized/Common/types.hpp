#ifndef TYPES_HPP
#define TYPES_HPP

#include <vector>
#include <variant>
#include <string>
#include <map>

class Vertex;
class Cluster;

using VertexID = int;
using Coordinates = std::vector<float>;
using AdjacentVertex = std::pair<const Vertex *, bool>; // second: is support edge
using AdjacencyList = std::vector<AdjacentVertex>;

class Vertex
{
public:
  const VertexID id;
  const Coordinates coordinates;

  const Cluster * cluster;
  AdjacencyList adjacencyList;
  float quality;

  Vertex(const VertexID id, const Coordinates coordinates, const Cluster * const cluster = nullptr);
};

using Vertices = std::vector<Vertex>;

using ClusterID = std::variant<int, const std::string>;

class Cluster
{
public:
  const ClusterID id;

  float sumq;
  int magq;

  float online_avgq;
  float sumDeltaSq;
  float online_stdq;
  float treshold;

  Cluster(const ClusterID id);

  void reset();
  void accumQ_updateStats(const float q);
  void computeTreshold(const float tolerance);
};

using Clusters = std::map<ClusterID, Cluster>;

using Edge = std::pair<const Vertex * const, const Vertex * const>;
using ExpertID = int;
using ExpertDifferences = std::vector<float>;

class Expert
{
private:
  const Coordinates computeMidpoint(const Edge& edge);
  const ExpertDifferences computeDifferences(const Edge& edge);
  float computeBias(const Coordinates& midpoint, const ExpertDifferences& differences);

public:
  const ExpertID id;
  const Edge edge;

  const Coordinates midpoint;
  const ExpertDifferences differences;
  const float bias;

  Expert(const ExpertID id, const Edge edge);
};

using Experts = std::vector<Expert>;

template<typename... Ts>
std::enable_if_t<(sizeof...(Ts) > 0), std::ostream&>
operator<<(std::ostream& os, const std::variant<Ts...>& var) {
  std::visit([&os](const auto& value) { os << value; }, var);
  return os;
}

#endif // TYPES_HPP
