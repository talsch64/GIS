#ifndef EX1_ENTITYJSONSERIALIZER_H
#define EX1_ENTITYJSONSERIALIZER_H

#include <rapidjson/document.h>
#include <memory>
#include "GeometryJsonSerializer.h"
#include "../../Junction.h"

class POI;

class Entity;

class Geometry;

class EntityJsonSerializer {
    GeometryJsonSerializer geometryJsonSerializer;
    CoordinatesJsonSerializer coordinatesJsonSerializer;

public:
    rapidjson::Value toJson(POI *entity, rapidjson::Document::AllocatorType &allocator);
    rapidjson::Value toJson(Junction *entity, rapidjson::Document::AllocatorType &allocator);

private:
    rapidjson::Value entityToJson(Entity *entity, rapidjson::Document::AllocatorType &allocator);

    rapidjson::Value setGeometry(rapidjson::Value entityJson, Geometry *geometry,
                                 rapidjson::Document::AllocatorType &allocator);
};

#endif //EX1_ENTITYJSONSERIALIZER_H
