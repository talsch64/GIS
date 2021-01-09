#ifndef GIS_CPP_SIMULATION_H
#define GIS_CPP_SIMULATION_H

#include <vector>
#include <filesystem>
#include "../Common/AbstractGIS.h"
#include "../Common/AbstractNavigation.h"
#include "GISContainer.h"
#include "NavigationContainer.h"
#include "NavigationTask.h"

/**
 * @brief class Simulation simulates the various navigation and gis .so files on a given map file
 * and navigation requests and analyzes the results.
 * Implemented as a Singleton.
 */
class Simulation {
    using navigationRequest = std::pair<Coordinates, Coordinates>;

private:
    std::vector<std::unique_ptr<GISContainer>> gisContainers;
    std::vector<std::unique_ptr<NavigationContainer>> navigationContainers;
    std::string nextName;
    std::vector<navigationRequest> requests;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<int>>> results;

    class NavigationTasksManager {
        static int nextGisIndex;
        static int nextNavigationIndex;
        static int nextRequestIndex;

//        NavigationTasksManager();
//        NavigationTask createNextTask();

    };


    Simulation() = default;

public:
    static Simulation &getInstance() {
        static Simulation instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }

    Simulation(Simulation const &) = delete;

    void operator=(Simulation const &) = delete;

    void addGisFactory(std::function<std::unique_ptr<AbstractGIS>()> gisFactory);

    void
    addNavigationFactory(std::function<std::unique_ptr<AbstractNavigation>(const NavigationGIS &)> navigationFactory);

    void setNextName(std::string name);

    //TODO implement
    void loadNavigationRequests(std::filesystem::path requests_path);
};


#endif //GIS_CPP_SIMULATION_H
