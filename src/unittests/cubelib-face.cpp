

#include "cubelib/cubelib.h"
#include "cubelib/formatters.hpp"

#include "gtest/gtest.h"



#include <vector>
#include <tuple>




struct CubelibFaceTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};



TEST_F(CubelibFaceTest,corner_on_face)
{
    uint32_t corner_counts[CUBELIB_CORNERS_SIZE] = {0};
    
    for (auto face : cubelib_all_faces)
    {
        uint32_t per_face_corner_counts[CUBELIB_CORNERS_SIZE] = {0};
        for (int i = 0; i < 4; ++i)
        {
            ASSERT_LT(cubelib_get_face_index(face), CUBELIB_CORNERS_SIZE);
            
            cubelib_corner_t corner_get = cubelib_get_corner_on_face(face, i);
            cubelib_corner_t corner_calc = cubelib_calc_corner_on_face(face, i);
            cubelib_corner_t corner_const = cubelib_corners_on_face[cubelib_get_face_index(face)][i];
            
            ASSERT_TRUE(cubelib_is_corner_equal(corner_get, corner_calc));
            ASSERT_TRUE(cubelib_is_corner_equal(corner_get, corner_const));
            
            
            ASSERT_TRUE(!cubelib_is_corner_null(corner_get));
            ASSERT_TRUE(cubelib_is_corner_on_face(corner_get,face));
            
            corner_counts[cubelib_get_corner_index(corner_get)]++;
            per_face_corner_counts[cubelib_get_corner_index(corner_get)]++;
        }
        
        int unique_corners_in_this_face = 0;
        for (auto corner : cubelib_all_corners)
        {
            ///each corner should have been passed over at MOST 1 times
            ASSERT_LE(per_face_corner_counts[ cubelib_get_corner_index(corner) ], 1U);
            
            unique_corners_in_this_face += per_face_corner_counts[ cubelib_get_corner_index(corner) ];
        }
        ///there should be 4 unique corners in this face
        ASSERT_EQ(4, unique_corners_in_this_face);
    }
    
    
    for (auto corner : cubelib_all_corners)
    {
        ///each corner should have been passed over 3 times
        ASSERT_EQ(3U, corner_counts[ cubelib_get_corner_index(corner) ]);
    }
  
}




TEST_F(CubelibFaceTest,edge_on_face)
{
    uint32_t edge_counts[CUBELIB_EDGES_SIZE] = {0};
    
    for (auto face : cubelib_all_faces)
    {
        uint32_t per_face_edge_counts[CUBELIB_EDGES_SIZE] = {0};
        for (int i = 0; i < 4; ++i)
        {
            ///sanity
            ASSERT_LT(cubelib_get_face_index(face), CUBELIB_FACES_SIZE);
            
            cubelib_edge_t edge_get = cubelib_get_edge_on_face(face, i);
            cubelib_edge_t edge_calc = cubelib_calc_edge_on_face(face, i);
            cubelib_edge_t edge_const = cubelib_edges_on_face[cubelib_get_face_index(face)][i];
            
            ASSERT_TRUE(cubelib_is_edge_equal(edge_get, edge_calc));
            ASSERT_TRUE(cubelib_is_edge_equal(edge_get, edge_const));
            
            
            ASSERT_TRUE(!cubelib_is_edge_null(edge_get));
            ASSERT_TRUE(cubelib_is_edge_on_face(edge_get,face));
            
            edge_counts[cubelib_get_edge_index(edge_get)]++;
            per_face_edge_counts[cubelib_get_edge_index(edge_get)]++;
        }
        
        int unique_edges_in_this_face = 0;
        for (auto edge : cubelib_all_edges)
        {
            ///each edge should have been passed over at MOST 1 times
            ASSERT_LE(per_face_edge_counts[ cubelib_get_edge_index(edge) ], 1U);
            
            unique_edges_in_this_face += per_face_edge_counts[ cubelib_get_edge_index(edge) ];
        }
        ///there should be 4 unique edges in this face
        ASSERT_EQ(4, unique_edges_in_this_face);
    }
    
    
    for (auto edge : cubelib_all_edges)
    {
        ///each edge should have been passed over 2 times
        ASSERT_EQ(2U, edge_counts[ cubelib_get_edge_index(edge) ]);
    }
}









