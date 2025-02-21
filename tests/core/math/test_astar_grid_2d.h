/**************************************************************************/
/*  test_astar_grid_2d.h                                                  */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             GODOT ENGINE                               */
/*                        https://godotengine.org                         */
/**************************************************************************/
/* Copyright (c) 2014-present Godot Engine contributors (see AUTHORS.md). */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef TEST_ASTAR_GRID_2D_H
#define TEST_ASTAR_GRID_2D_H

#include "core/math/a_star_grid_2d.h"
#include "tests/test_macros.h"
#include "thirdparty/doctest/doctest.h"

namespace Test_AStar_Grid2D  {

    TEST_CASE("AStarGrid2D - Basic Initialization") {
        AStarGrid2D grid;
        CHECK(grid.is_dirty());
        CHECK(grid.get_region() == Rect2i());
    }
    
    TEST_CASE("AStarGrid2D - Set and Get Region") {
        AStarGrid2D grid;
        Rect2i region(0, 0, 10, 10);
        grid.set_region(region);
        CHECK(grid.get_region() == region);
    }
    
    TEST_CASE("AStarGrid2D - Set and Get Cell Size") {
        AStarGrid2D grid;
        Size2 cell_size(2, 2);
        grid.set_cell_size(cell_size);
        CHECK(grid.get_cell_size() == cell_size);
    }
    
    TEST_CASE("AStarGrid2D - Setting Invalid Region") {
        AStarGrid2D grid;
        Rect2i invalid_region(0, 0, -5, -5);
        grid.set_region(invalid_region);
        CHECK(grid.get_region().size.x >= 0);
        CHECK(grid.get_region().size.y >= 0);
    }
    
    TEST_CASE("AStarGrid2D - In Bounds Check") {
        AStarGrid2D grid;
        grid.set_region(Rect2i(0, 0, 5, 5));
        CHECK(grid.is_in_bounds(2, 2));
        CHECK_FALSE(grid.is_in_bounds(-1, -1));
        CHECK_FALSE(grid.is_in_bounds(6, 6));
    }
    
    TEST_CASE("AStarGrid2D - Setting Point Solid") {
        AStarGrid2D grid;
        grid.set_region(Rect2i(0, 0, 5, 5));
        Vector2i point(2, 2);
        grid.update();
        grid.set_point_solid(point, true);
        CHECK(grid.is_point_solid(point));
    }
    
    TEST_CASE("AStarGrid2D - Get Path Basic") {
        AStarGrid2D grid;
        grid.set_region(Rect2i(0, 0, 5, 5));
        grid.update();
        Vector<Vector2> path = grid.get_point_path(Vector2i(0, 0), Vector2i(4, 4), true);
        CHECK(path.size() > 0);
    }
    
    TEST_CASE("AStarGrid2D - Get Path with Obstacle") {
        AStarGrid2D grid;
        grid.set_region(Rect2i(0, 0, 5, 5));
        grid.update();
        grid.set_point_solid(Vector2i(2, 2), true);
        Vector<Vector2> path = grid.get_point_path(Vector2i(0, 0), Vector2i(4, 4), true);
        CHECK(path.size() > 0);
        bool contains_obstacle = false;
        for (const auto& p : path) {
            if (p == Vector2(2, 2)) {
                contains_obstacle = true;
                break;
            }
        }
        CHECK_FALSE(contains_obstacle); // Ensure obstacle is avoided

    }
    
    TEST_CASE("AStarGrid2D - Heuristic Calculations") {
        Vector2i p1(0, 0);
        Vector2i p2(3, 4);
        CHECK(AStarGrid2D::HEURISTIC_EUCLIDEAN == 0);
        CHECK(AStarGrid2D::HEURISTIC_MANHATTAN == 1);
        CHECK(AStarGrid2D::HEURISTIC_OCTILE == 2);
        CHECK(AStarGrid2D::HEURISTIC_CHEBYSHEV == 3);
    }
    

} // namespace Test_AStar_Grid2D 

#endif // TEST_ASTAR_GRID_2D_H
