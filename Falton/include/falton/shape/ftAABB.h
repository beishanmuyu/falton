//
// Created by Kevin Yu on 2/22/16.
//

#ifndef FALTON_FTAABB_H
#define FALTON_FTAABB_H

#include <falton/math.h>

struct ftAABB {
    ftVector2 min;
    ftVector2 max;

    bool overlap(const ftAABB& b) const {
        if (this->max.x < b.min.x || b.max.x < this->min.x) return false;
        if (this->max.y< b.min.y || b.max.y < this->min.y) return false;
        return true;
    }

    void extend(real extension) {
        min.x -= extension;
        min.y -= extension;
        max.x += extension;
        max.y += extension;
    }

    real getPerimeter() {
        return 2 * ((max.x - min.x) + (max.y - min.y));
    }

    bool isContain(const ftAABB& b) {
        return (min.x < b.min.x && min.y < b.min.y && max.x > b.max.x && max.y > b.max.y);
    }

    real getArea() const {
        return (max.x - min.x) * (max.y - min.y);
    }

    static ftAABB combine(const ftAABB&a, const ftAABB&b) {
        ftAABB aabb;
        aabb.min.x = ftMin(a.min.x, b.min.x);
        aabb.min.y = ftMin(a.min.y, b.min.y);
        aabb.max.x = ftMax(a.max.x, b.max.x);
        aabb.max.y = ftMax(a.max.y, b.max.y);
        return aabb;
    }

    static ftAABB Create(ftVector2 p1, ftVector2 p2) {
        ftAABB aabb;
        
        aabb.min.x = p1.x;
        aabb.max.x = p2.x;
        if (p1.x > p2.x) {
            aabb.min.x = p2.x;
            aabb.max.x = p1.x;
        }

        aabb.min.y = p1.y;
        aabb.max.y = p2.y;
        if (p1.y > p2.y) {
            aabb.min.y = p2.y;
            aabb.max.y = p1.y;
        }

        return aabb;
    }
};




#endif //FALTON_FTAABB_H
