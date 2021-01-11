

#ifndef GIS_CPP_TASKRESULT_H
#define GIS_CPP_TASKRESULT_H


#include <memory>
#include "../Common/AbstractRoutes.h"


/**
 * class TaskResult represent the result of a task (Navigation x GIS x request)
 * Stores the output Routes of the task, a flag that indicates whether the routes is valid abd the gis usage count.
 */
class TaskResult {
    std::unique_ptr<AbstractRoutes> routes = nullptr;
    bool shortestDistanceValid;
    bool shortestTimeValid;
    std::size_t gisUsageCount;

public:
    void setRoutes(std::unique_ptr<AbstractRoutes> routes);

    void setGisUsageCount(size_t gisUsage);

    const std::unique_ptr<AbstractRoutes> &getRoutes() const;

    size_t getGisUsageCount() const;

    bool isShortestDistanceValid() const;

    void setShortestDistanceValid(bool shortestDistanceValid);

    bool isShortestTimeValid() const;

    void setShortestTimeValid(bool shortestTimeValid);
};


#endif //GIS_CPP_TASKRESULT_H