
#include "POI.h"
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

#include <utility>

POI::POI(const EntityId &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, std::vector<std::string> accessibility,
         std::unique_ptr<Circle> geometry) : Entity(id, name, description, categoryTags, "POI"),
                                             geometry(std::move(geometry)), accessibility(std::move(accessibility)) {}

const std::vector<std::string> &POI::getAccessibility() const {
    return accessibility;
}

rapidjson::Value POI::toJson(rapidjson::Document::AllocatorType &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const std::unique_ptr<Geometry> &POI::getGeometry() const {
    return (const std::unique_ptr<Geometry> &) geometry;
}
