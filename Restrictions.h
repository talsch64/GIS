#ifndef GIS_CPP_RESTRICTIONS_H
#define GIS_CPP_RESTRICTIONS_H


#include <unordered_set>
#include <string>
#include <vector>

/**
 * @brief Restrictions class
 *
 */
class Restrictions {
    std::vector<std::string> restrictions;
public:
    /**
     * @brief Construct a new Restrictions object
     *
     * @param restrictions
     */
    Restrictions(const char* restrictions);
    Restrictions();

    const std::vector<std::string> &getRestrictions() const;

};

#endif //GIS_CPP_RESTRICTIONS_H
