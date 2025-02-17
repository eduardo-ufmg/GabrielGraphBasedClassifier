#ifndef GRAPH_TYPES_HPP
#define GRAPH_TYPES_HPP

#include <vector>
#include <string>
#include <map>
#include <variant>
#include <memory>

typedef unsigned VertexID_t;

class Vertex
{
public:
  std::vector<double> features;
  std::vector<VertexID_t> adjacents;
  double q;
};

typedef std::map<VertexID_t, std::shared_ptr<Vertex> > VertexMap;

class QualityMeasure
{
public:
  double sum_q;
  double magnitude;
};

class Cluster
{
public:
  VertexMap vertices;
  QualityMeasure Q;
  double threshold;
  double averageQuality;
  double stdDeviation;
};

using ClassType = std::variant<unsigned, std::string>;
typedef std::map<ClassType, Cluster> ClusterMap;

template<typename... Ts>
std::enable_if_t<(sizeof...(Ts) > 0), std::ostream&>
operator<<(std::ostream& os, const std::variant<Ts...>& var) {
  std::visit([&os](const auto& value) { os << value; }, var);
  return os;
}

#endif