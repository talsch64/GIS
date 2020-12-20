
#include "gtest/gtest.h"
#include "../GISNamedTypes.h"
#include "../GISProvider.h"
#include "../CoordinatesMath.h"
#include "../navigation/Navigation.h"
#include "../entities/Way.h"

class IsraelMapTest : public ::testing::Test {
protected:
    GIS gis;
    NavigationGIS navGis;
    Navigation navigation;
public:
    IsraelMapTest() : gis(GISProvider::getGIS()), navGis(gis), navigation(navGis) {
    }

    void SetUp() {
        gis.loadMapFile("israel.json");
    }

    void TearDown() {

    }

    std::vector<EntityId> getWaysIds(Route route) {
        std::vector<EntityId> ids;
        if (route.isValid()) {
            for (auto pair: route.getWays()) {
                ids.push_back(pair.first);
            }
        }
        return ids;
    }

    void printRoutes(Routes routes) {
        if (!routes.isValid()) {
            //TODO add invalid messgae of the routes
            std::cout << "invalid routes!!" << std::endl;
            return;
        }
        std::cout << std::endl << "---ShortestDistance Route--- " << std::endl;

        for (const auto &way : routes.shortestDistance().getWays()) {
            std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                      << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
            std::cout << "length: " << (double) gis.getWay(way.first).getLength() << ", time: "
                      << (double) gis.getWay(way.first).getTime() << std::endl;
            std::cout << "==================================" << std::endl;
        }

        std::cout << std::endl << "---ShortestTime Route--- " << std::endl;

        for (const auto &way : routes.shortestTime().getWays()) {
            std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
                      << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
            std::cout << "length: " << (double) gis.getWay(way.first).getLength() << ", time: "
                      << (double) gis.getWay(way.first).getTime() << std::endl;
            std::cout << "==================================" << std::endl;
        }
        std::cout << "ShortestDistanceRoute - distance: " << (double) routes.shortestDistance().totalLength()
                  << ", time: "
                  << (double) routes.shortestDistance().estimatedDuration() << std::endl;
        std::cout << "ShortestTimeRoute - distance: " << (double) routes.shortestTime().totalLength() << ", time: "
                  << (double) routes.shortestTime().estimatedDuration() << std::endl;
    }
};


/**
 * Routes should be the same but reveres
 */
TEST_F(IsraelMapTest, oppositeRoutesByDistance) {
    Coordinates from(Longitude(32.113357), Latitude(34.801290)); // J1001
    Coordinates to(Longitude(32.11265), Latitude(34.79254)); // J1014
    auto routes = navigation.getRoutes(from, to);
    EXPECT_TRUE(routes.isValid());
    auto size1 = routes.shortestDistance().getWays().size();
    EXPECT_EQ(size1, 5);
    auto reverseRoutes = navigation.getRoutes(to, from);
    EXPECT_TRUE(routes.isValid());
    auto size2 = routes.shortestDistance().getWays().size();
    EXPECT_EQ(size2, 5);
    auto from_to_ways = routes.shortestDistance().getWays();
    auto to_from_ways =  reverseRoutes.shortestDistance().getWays();
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(from_to_ways[i].first, to_from_ways[4-i].first);
        EXPECT_NE(from_to_ways[i].second, to_from_ways[4-i].second);
    }
}


/**
 * Routes should be the same but reveres
 */
TEST_F(IsraelMapTest, oppositeRoutesByTime) {
    Coordinates from(Longitude(32.113357), Latitude(34.801290)); // J1001
    Coordinates to(Longitude(32.11265), Latitude(34.79254)); // J1014
    auto routes = navigation.getRoutes(from, to);
    EXPECT_TRUE(routes.isValid());
    auto size1 = routes.shortestTime().getWays().size();
    EXPECT_EQ(size1, 5);
    auto reverseRoutes = navigation.getRoutes(to, from);
    EXPECT_TRUE(routes.isValid());
    auto size2 = routes.shortestTime().getWays().size();
    EXPECT_EQ(size2, 5);

    auto from_to_ways = routes.shortestTime().getWays();
    auto to_from_ways =  reverseRoutes.shortestTime().getWays();
    for (int i = 0; i < 5; i++) {
        EXPECT_EQ(from_to_ways[i].first, to_from_ways[4-i].first);
        EXPECT_NE(from_to_ways[i].second, to_from_ways[4-i].second);
    }
}

TEST_F(IsraelMapTest, niceRoute) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.11181), Latitude(34.79474)); // on W2041, between J1004 and J1014
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(routes.isValid());
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 11);
    EXPECT_EQ(timeRouteSize, 10);
//    printRoutes(routes);
}

/**
 * Starts from a point on a highway - the closest way is W2017 which is a highway
 */
TEST_F(IsraelMapTest, onHighway) {
    Coordinates origin(Longitude(32.50428), Latitude(35.06188)); // on a highway W2017
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(routes.isValid());
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_NE(distRouteSize, timeRouteSize); // routes should be different
    EXPECT_EQ(routes.shortestDistance().getWays().front().first, EntityId("W2017")); //highway
    EXPECT_EQ(routes.shortestTime().getWays().front().first, EntityId("W2017")); //highway
//    printRoutes(routes);
}

/**
 * Closest Way is W2017 highway but is too far so the expected first way id is not W2017
 */
TEST_F(IsraelMapTest, highwayTooFar) {
    Coordinates origin(Longitude(32.4618), Latitude(35.08074)); // highway W2017 is the closest but too far away
    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(routes.isValid());
    EXPECT_NE(routes.shortestDistance().getWays().front().first, EntityId("W2017")); //highway too far
    EXPECT_NE(routes.shortestTime().getWays().front().first, EntityId("W2017")); //highway too far
    auto shortestDistanceWays = getWaysIds(routes.shortestDistance());
    auto id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    // shortestDistanceRoute runs through W2045
    EXPECT_NE(id, shortestDistanceWays.end());
    auto shortestTimeWays = getWaysIds(routes.shortestTime());
    id = std::find(shortestTimeWays.begin(), shortestTimeWays.end(), EntityId("W2045"));
    // shortestTimeRoute does not run through W2045
    EXPECT_EQ(id, shortestTimeWays.end());
//    printRoutes(routes);
}


TEST_F(IsraelMapTest, differentRoutes) {
    Coordinates origin(Longitude(32.50365),
                       Latitude(35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J1020

    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(routes.isValid());
    auto sizeTime = routes.shortestTime().getWays().size();
    auto sizeDistance = routes.shortestDistance().getWays().size();
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 7);
    EXPECT_EQ(timeRouteSize, 5);
//    printRoutes(routes);

}

/**
 * This test tests two opposite route - from J1020 to J1026 and vice-versa.
 * The shortest distance from J1026 to J1020 runs through unidirectional way so the routes should differ.
 * Way W2045 is the unidirectional way
 */
TEST_F(IsraelMapTest, differentRoutesOpposite) {
    Coordinates destination(Longitude(32.50365),
                            Latitude(
                                    35.06183)); // near J1026, closestWayPoint is on a highway (less then 3 meters away)
    Coordinates origin(Longitude(32.10885), Latitude(34.85451)); // J1020
    // from J1026 to J1020:
    auto routes = navigation.getRoutes(destination, origin);
    EXPECT_TRUE(routes.isValid());
    auto shortestDistanceWays = getWaysIds(routes.shortestDistance());
    auto id = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    EXPECT_NE(id, shortestDistanceWays.end());

    // from J1020 to J1026:
    auto reverseRoute = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(reverseRoute.isValid());
    shortestDistanceWays = getWaysIds(reverseRoute.shortestDistance());
    auto invalidId = std::find(shortestDistanceWays.begin(), shortestDistanceWays.end(), EntityId("W2045"));
    // The reverse route from J2020 to J1026 is not the same because there is a unidirectional way.
    EXPECT_EQ(invalidId, shortestDistanceWays.end());
//    printRoutes(routes);

}

TEST_F(IsraelMapTest, invalidRoutesUnreachable) {
    Coordinates origin(Longitude(32.31719),
                       Latitude(35.18944)); // point on W2047 which is isolated
    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J1020
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(!routes.isValid());
    EXPECT_EQ(routes.getErrorMessage(), "Routes not found!");
}

TEST_F(IsraelMapTest, unidirectionalSingleWayInvalid) {
//    W2047 is unidirectional
    Coordinates origin(Longitude(32.31719),
                       Latitude(35.18944)); // point on W2047 which is isolated
    Coordinates destination(Longitude(32.33931), Latitude(35.19085)); // J1033 which is on W2047
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(!routes.isValid());
    EXPECT_EQ(routes.getErrorMessage(), "Routes contain only one unidirectional way!");
}

TEST_F(IsraelMapTest, bidirectionalSingleWay) {
    Coordinates destination(Longitude(32.34981),
                            Latitude(35.22814)); // J1038
    Coordinates origin(Longitude(32.25985), Latitude(35.22334)); // J1039
    auto routes = navigation.getRoutes(origin, destination);
    auto way = routes.shortestDistance().getWays().front();
    EXPECT_TRUE(routes.isValid());
//  size should be 1 but it makes automatic U-turn
//  EXPECT_EQ(routes.shortestDistance().getWays().size(), 1);
    EXPECT_EQ(way.first, EntityId("W2051"));
    EXPECT_EQ(gis.getWay(way.first).getLength(), routes.shortestDistance().totalLength());
    EXPECT_EQ(gis.getWay(way.first).getTime(), routes.shortestDistance().estimatedDuration());
//    printRoutes(routes);
}

TEST_F(IsraelMapTest, singleSlowWayVsFastWay) {
    Coordinates origin(Longitude(32.49647),
                       Latitude(35.03114)); // near J1026 on W2052
    Coordinates destination(Longitude(32.37961), Latitude(34.97144)); // J1029
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(routes.isValid());
    auto shortestDistanceRoute = routes.shortestDistance();
    auto shortestTimeRoute = routes.shortestTime();
    EXPECT_LT(shortestDistanceRoute.totalLength(), shortestTimeRoute.totalLength());
    EXPECT_LT(shortestTimeRoute.estimatedDuration(), shortestDistanceRoute.estimatedDuration());
//    printRoutes(routes);
}
/**
 * This test tests that the route found is indeed the shortest route when the final way is bidirectional.
 * The final Way is W2050 and connect the junctions J1035 and J1036
 * the origin point is J1037 which is connected to both J1035 and J1036 but is closer to J1035
 * The destination point is placed on W2050 very close to J1036
 * The expected Route is J1037-->J1036-->destination.
 */
TEST_F(IsraelMapTest, finalBidirectionalWay) {
    Coordinates origin(Longitude(32.15044), Latitude(34.85382)); // origin is J1037
    Coordinates destination(Longitude(32.18378), Latitude(34.82216)); // near J1036, on way W2050
    auto routes = navigation.getRoutes(origin, destination);
    EXPECT_TRUE(routes.isValid());
    auto sizeTime = routes.shortestTime().getWays().size();
    auto sizeDistance = routes.shortestDistance().getWays().size();
    auto distRouteSize = routes.shortestDistance().getWays().size();
    auto timeRouteSize = routes.shortestTime().getWays().size();
    EXPECT_EQ(distRouteSize, 2);
    EXPECT_EQ(timeRouteSize, 2);
    auto ways = routes.shortestDistance().getWays();
    EXPECT_EQ(ways.front().first, EntityId("W2049"));
    EXPECT_EQ(ways.back().first, EntityId("W2050"));
//    printRoutes(routes);
}

//TODO delete code!!!!
//TEST(GISBasic, routeToFrom) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates from(Longitude(32.113357), Latitude(34.801290));
//    Coordinates to(Longitude(32.111320), Latitude(34.796037));
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(from, to);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(size, 3);
//}
//
//
//TEST(GISBasic, routeFromTo) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates to(Longitude(32.113357), Latitude(34.801290));
//    Coordinates from(Longitude(32.11181), Latitude(34.79474));
//    gis.loadMapFile("route_test.json");
//    auto routes = nav.getRoutes(from, to);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(size, 3);
//}
//
//TEST(GISBasic, niceRoute) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.50365),
//                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
//    Coordinates destination(Longitude(32.11181), Latitude(34.79474)); // on W2041, between J1004 and J1014
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    std::cout << "============================== test 3 ==============================" << std::endl << std::endl;
//    std::cout << "============================== Shortest Distance: ==============================" << std::endl
//              << std::endl;
//    for (const auto &way : routes.shortestDistance().getWays()) {
//        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
//                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
//        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
//                  << (double) navGis.getWay(way.first).getTime() << std::endl;
//        std::cout << "==================================" << std::endl;
//
//    }
//
//    std::cout << "--------------------------------------------------------" << std::endl;
//    std::cout << "============================== Shortest Time: ==============================" << std::endl
//              << std::endl;
//
//    for (const auto &way : routes.shortestTime().getWays()) {
//        std::cout << "ID: " << static_cast<std::string>(way.first) << " Direction: "
//                  << ((way.second == Direction::A_to_B) ? "A_B" : "B_A") << std::endl;
//        std::cout << "length: " << (double) navGis.getWay(way.first).getLength() << ", time: "
//                  << (double) navGis.getWay(way.first).getTime() << std::endl;
//        std::cout << "==================================" << std::endl;
//
//    }
//    std::cout << "shortestDistanceRoute: distance: " << (double) routes.shortestDistance().totalLength() << ", time: "
//              << (double) routes.shortestDistance().estimatedDuration() << std::endl;
//    std::cout << "shortestTimeRoute: distance: " << (double) routes.shortestTime().totalLength() << ", time: "
//              << (double) routes.shortestTime().estimatedDuration() << std::endl << std::endl;
//    EXPECT_EQ(size, 8);
//}
//
//TEST(GISBasic, onHighway) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.50428), Latitude(35.06188)); // on a highway
//    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(std::string(routes.shortestDistance().getWays().begin()->first), "W2017");
//    EXPECT_EQ(size, 6);
//}
//
//
//TEST(GISBasic, highwayTooFar) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.4618), Latitude(35.08074)); // highway is the closest but too far away
//    Coordinates destination(Longitude(32.057), Latitude(34.86717)); // on W2023, between J1022 and J1023
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(std::string(routes.shortestDistance().getWays().begin()->first), "W2036"); // not an highway
//    EXPECT_EQ(size, 6);
//}
//
//
//TEST(GISBasic, differentRoutes) {
//    GIS gis = GISProvider::getGIS();
//    NavigationGIS navGis(gis);
//    Navigation nav(navGis);
//    Coordinates origin(Longitude(32.50365),
//                       Latitude(35.06183)); // J1026, closestWayPoint is on a highway (less then 3 meters away)
//    Coordinates destination(Longitude(32.10885), Latitude(34.85451)); // J2020
//    gis.loadMapFile("israel.json");
//    auto routes = nav.getRoutes(origin, destination);
//    auto size = routes.shortestDistance().getWays().size();
//    EXPECT_EQ(size, 6);
//}


