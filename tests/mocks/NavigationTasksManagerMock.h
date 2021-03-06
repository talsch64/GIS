#ifndef GIS_CPP_NAVIGATIONTASKSMANAGERMOCK_H
#define GIS_CPP_NAVIGATIONTASKSMANAGERMOCK_H


#include "../../Simulator/NavigationTasksManager.h"

class NavigationTasksManagerMock: public NavigationTasksManager {
    std::unique_ptr<std::unique_ptr<TaskResult>[]> fakeResults;

public:
    NavigationTasksManagerMock(int gisAmount, int navigationsAmount, int requestsAmount);

    std::unique_ptr<TaskResult> &getResult(int gisIndex, int navigationIndex, int requestIndex);

    void setResult(int gisIndex, int navigationIndex, int requestIndex, std::unique_ptr<TaskResult> result);

protected:
    std::unique_ptr<NavigationTask>
    createTask(std::unique_ptr<GISContainer> &gisContainer, std::unique_ptr<NavigationContainer> &navigationContainer,
               const NavigationRequest &request, int gisIndex, int navigationIndex, int requestIndex) override;

};


#endif //GIS_CPP_NAVIGATIONTASKSMANAGERMOCK_H
