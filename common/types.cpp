#include "types.hpp"

#include <cmath>
#include <algorithm>
#include <numeric>

using namespace std;

BaseVertex::BaseVertex(const VertexID id, const Coordinates coordinates)
  : id(id), coordinates(coordinates)
{}

Vertex::Vertex(const VertexID id, const Coordinates coordinates, std::shared_ptr<Cluster> cluster)
  : BaseVertex(id, coordinates), cluster(cluster), quality(0.0f)
{}

Cluster::Cluster(const ClusterID id)
  : id(id), sumq(0.0f), magq(0), online_avgq(0.0f), sumDeltaSq(0.0f), online_stdq(0.0f), treshold(0.0f)
{}

void Cluster::reset()
{
  sumq = 0.0f;
  magq = 0;
  online_avgq = 0.0f;
  sumDeltaSq = 0.0f;
  online_stdq = 0.0f;
  treshold = 0.0f;
}

void Cluster::accumQ_updateStats(const float q)
{
  sumq += q;
  ++ magq;

  const float delta = q - online_avgq;

  online_avgq += delta / magq;

  const float delta2 = delta - online_avgq;

  sumDeltaSq += delta * delta2;

  if (magq > 1) {
    online_stdq = sqrt(sumDeltaSq / (magq - 1));
  }
}

void Cluster::computeTreshold(const float tolerance)
{
  treshold = online_avgq - tolerance * online_stdq;
}

const Coordinates Expert::computeMidpoint(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  Coordinates midpoint(c1.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            midpoint.begin(),
            [](const float x, const float y) {
              return (x + y) / 2.0f;
            });

  return midpoint;
}

const NormalVector Expert::computeNormal(const Edge& edge)
{
  const auto& [v1, v2] = edge;
  const auto& c1 = v1->coordinates;
  const auto& c2 = v2->coordinates;

  NormalVector normal(midpoint.size());

  transform(c1.begin(), c1.end(),
            c2.begin(),
            normal.begin(),
            minus<float>());

  return normal;
}

float Expert::computeBias(const Coordinates& midpoint, const NormalVector& normal)
{
  return inner_product(midpoint.begin(), midpoint.end(),
                       normal.begin(),
                       0.0f);
}

Expert::Expert(const ExpertID id, const Edge edge)
  : id(id), edge(edge),
  midpoint(computeMidpoint(edge)),
  normal(computeNormal(edge)),
  bias(computeBias(midpoint, normal))
{}

Expert::Expert(const ExpertID id, const Coordinates midpoint, const NormalVector normal, const float bias)
  : id(id), edge({nullptr, nullptr}), midpoint(midpoint), normal(normal), bias(bias)
{}

SupportVertex::SupportVertex(const VertexID id, const Coordinates coordinates, const ClusterID clusterid)
  : BaseVertex(id, coordinates), clusterid(clusterid)
{}

VertexToLabel::VertexToLabel(const VertexID id, const Coordinates coordinates, const ClusterID expectedclusterid)
  : BaseVertex(id, coordinates), expectedclusterid(expectedclusterid)
{}

LabeledVertex::LabeledVertex(const VertexID id, const Coordinates coordinates, const ClusterID clusterid)
  : BaseVertex(id, coordinates), clusterid(clusterid)
{}

void chipIDbimap::insert(const ClusterID& cid, const int chip)
{
  cidtochip.emplace(cid, chip);
  chiptocid.emplace(chip, cid);
}

int chipIDbimap::getchip(const ClusterID& cid) const
{
  return cidtochip.at(cid);
}

const ClusterID& chipIDbimap::getcid(const int chip) const
{
  return chiptocid.at(chip);
}

const cIDtochipMap& chipIDbimap::getcidtochip() const
{
  return cidtochip;
}

const chiptocIDMap& chipIDbimap::getchiptocid() const
{
  return chiptocid;
}
