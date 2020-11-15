#ifndef EX1_ENTITYJSONPARSER_H
#define EX1_ENTITYJSONPARSER_H

#include <rapidjson/document.h>
#include "POI.h"
#include "Junction.h"
#include "geometry/GeometryJsonParser.h"
#include "Way.h"
#include<memory>


class EntityJsonParser {
    GeometryJsonParser geometryJsonParser;

public:
    std::unique_ptr<Entity> parse(rapidjson::Value &doc);

private:

    std::unique_ptr<POI> parsePoi(rapidjson::Value &doc);

    std::unique_ptr<Junction> parseJunction(rapidjson::Value &doc);

    std::string parseEntityId(rapidjson::Value &doc);

    std::string parseName(rapidjson::Value &doc);

    std::string parseDescription(rapidjson::Value &doc);

    std::vector<std::string> parseCategoryTags(rapidjson::Value &doc);

    std::vector<std::string> parseAccessibility(rapidjson::Value &doc);

    std::unique_ptr<Way> parseWay(rapidjson::Value &doc);

    std::string parseDirection(rapidjson::Value &doc);

    int parseSpeedLimit(rapidjson::Value &doc);

    bool parseTollRoad(rapidjson::Value &doc);

    std::vector<std::string> parseRestricted(rapidjson::Value &doc);

    std::string parseJunctionId(rapidjson::Value &doc, const char *direction);

    std::vector<Coordinates> parseCurves(rapidjson::Value &doc);

    std::vector<Coordinates> parseCoordinates(rapidjson::Value &doc);
};

#endif //EX1_ENTITYJSONPARSER_H
