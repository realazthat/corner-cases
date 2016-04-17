


#include "cubelib/cubelib.h"
#include "cubelib/formatters.hpp"

#include "gtest/gtest.h"



#include <vector>
#include <tuple>


struct CubelibEdgeTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};



TEST_F(CubelibEdgeTest,null)
{

    
    ///test null edge
    {
        EXPECT_TRUE(cubelib_is_edge_valid(cubelib_null_edge));
        EXPECT_TRUE(cubelib_is_edge_null(cubelib_null_edge));
        EXPECT_TRUE(cubelib_is_edge_equal(cubelib_null_edge, cubelib_null_edge));
    }
    
    ///make sure all the regular edges are not cubelib_null_edge
    for (auto edge : cubelib_all_edges)
    {
        EXPECT_FALSE(cubelib_is_edge_null(edge));
        EXPECT_TRUE(cubelib_is_edge_valid(edge));
        EXPECT_FALSE(cubelib_is_edge_equal(edge, cubelib_null_edge));
    }

}



TEST_F(CubelibEdgeTest,indices)
{

    ///test edge indices
    {
        ///mark each edge's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (cubelib_edge_t edge : cubelib_all_edges)
        {
            bitmask |= (1 << cubelib_get_edge_index(edge));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << 12) - 1);
        
        
        
        ///test cubelib_get_edge_by_index()
        uint32_t index = 0;
        for (cubelib_edge_t edge : cubelib_all_edges)
        {
            EXPECT_EQ(index, cubelib_get_edge_index(edge));
            
            EXPECT_TRUE(cubelib_is_edge_equal(edge, cubelib_get_edge_by_index(index)));
            EXPECT_FALSE(cubelib_is_edge_null(edge));
            EXPECT_TRUE(cubelib_is_edge_valid(edge));
            
            ++index;
        }
        
    }
    

}



TEST_F(CubelibEdgeTest,unique)
{
    
    ///test edge indices
    {
        ///mark each edge's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (auto edge : cubelib_all_edges)
        {
            bitmask |= (1 << cubelib_get_edge_index(edge));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << CUBELIB_EDGES_SIZE) - 1);
    }
    
    
    std::map< std::tuple<uint8_t,bool,bool>, cubelib_edge_t > base_projection_values;
    std::map< std::size_t, cubelib_edge_t > index_values;
    for (auto edge : cubelib_all_edges)
    {
        auto base_axis = cubelib_get_edge_base_axis(edge);
        auto project_secondary = cubelib_is_edge_projected_secondary(edge);
        auto project_tertiary = cubelib_is_edge_projected_tertiary(edge);
        
        
        auto base_projection_key = std::make_tuple(base_axis,project_secondary,project_tertiary);
        if (base_projection_values.count(base_projection_key) > 0)
        {
            auto edge0 = base_projection_values[base_projection_key];
            ASSERT_TRUE( base_projection_values.count(base_projection_key) == 0 )
                << "edge: " << edge
                << ", (base_axis,project_secondary,project_tertiary): "
                    << "("
                    << (int)base_axis
                    << "," << (project_secondary ? "true" : "false")
                    << "," << (project_tertiary ? "true" : "false")
                    << ")"
                << ", edge0: " << edge0;
        }
        base_projection_values[base_projection_key] = edge;
        
        if (index_values.count(cubelib_get_edge_index(edge)) > 0)
        {
            
            auto edge0 = index_values[cubelib_get_edge_index(edge)];
            ASSERT_TRUE( index_values.count(cubelib_get_edge_index(edge)) == 0 )
                << "edge: " << edge
                << ", (base_axis,project_secondary,project_tertiary): "
                    << "("
                    << (int)base_axis
                    << "," << (project_secondary ? "true" : "false")
                    << "," << (project_tertiary ? "true" : "false")
                    << ")"
                << ", edge0: " << edge0;
            
        }
        index_values[cubelib_get_edge_index(edge)] = edge;
        
    }
}



TEST_F(CubelibEdgeTest,get_edge_by_axis)
{
    
    uint32_t mask = 0;
    
    for (uint_fast8_t base_axis = 0; base_axis < 3; ++base_axis)
    for (bool project_secondary : {false, true})
    for (bool project_tertiary : {false, true})
    {
        auto edge = cubelib_get_edge_by_axis(base_axis,project_secondary,project_tertiary);
        
        EXPECT_EQ(cubelib_get_edge_base_axis(edge), base_axis);
        EXPECT_EQ(cubelib_is_edge_projected_secondary(edge), project_secondary);
        EXPECT_EQ(cubelib_is_edge_projected_tertiary(edge), project_tertiary);
        
        ASSERT_EQ(0U, mask & (1 << cubelib_get_edge_index(edge)));
        mask |= (1 << cubelib_get_edge_index(edge));
    }
    
    EXPECT_EQ(mask, uint32_t(1 << CUBELIB_EDGES_SIZE) - 1);

}


TEST_F(CubelibEdgeTest,get_edge_corner)
{
    for (auto edge0 : cubelib_all_edges)
    {
        auto corner0 = cubelib_get_edge_corner0(edge0);
        auto corner1 = cubelib_get_edge_corner1(edge0);
        
        
        
        auto edge1 = cubelib_get_edge_by_corners(corner0, corner1);
        
        ASSERT_TRUE(cubelib_is_edge_equal(edge1,edge0));
        auto edge2 = cubelib_get_edge_by_corners(corner1, corner0);
        
        ASSERT_TRUE(cubelib_is_edge_equal(edge2,edge0));
    }
    
    ///for every pair of adjacent corners, check the edge
    for (auto corner0 : cubelib_all_corners){
        for (auto direction : cubelib_all_directions){
            auto corner1 = cubelib_get_adjacent_corner(corner0, direction);
            ASSERT_TRUE(!cubelib_is_corner_null(corner1));
            ASSERT_TRUE(cubelib_is_corner_adjacent_corner(corner0, corner1));
            
            auto edge = cubelib_get_edge_by_corners(corner0, corner1);
            
            ///the edge's corners are equal to one of corner0, corner1
            ASSERT_TRUE(cubelib_is_corner_equal(cubelib_get_edge_corner0(edge), corner0) || cubelib_is_corner_equal(cubelib_get_edge_corner0(edge), corner1));
            ASSERT_TRUE(cubelib_is_corner_equal(cubelib_get_edge_corner1(edge), corner0) || cubelib_is_corner_equal(cubelib_get_edge_corner1(edge), corner1));
            
            ///the edge did not return two equal corners
            ASSERT_FALSE(cubelib_is_corner_equal(cubelib_get_edge_corner0(edge), cubelib_get_edge_corner1(edge)));
        }
    }
    
    
    
}











TEST_F(CubelibEdgeTest,get_edge_by_corners)
{

    int edge_corner_count = 0;
    
    ///test cubelib_get_edge_by_corners() with two adjacent corners
    {
        for (cubelib_corner_t corner0 : cubelib_all_corners)
        {
            for (cubelib_corner_t corner1 : cubelib_all_corners)
            {
                if (!cubelib_is_corner_adjacent_corner(corner0,corner1))
                    continue;
                edge_corner_count++;
                
                cubelib_edge_t edge = cubelib_get_edge_by_corners(corner0,corner1);
                
                
                
                auto edge_corner0 = cubelib_get_edge_corner0(edge);
                auto edge_corner1 = cubelib_get_edge_corner1(edge);
                
                ///the edge's endpoints should match one of corner0, or corner1
                ASSERT_TRUE(cubelib_is_corner_equal(corner0, edge_corner0) || cubelib_is_corner_equal(corner1, edge_corner0))
                    << "corner0: " << corner0 << ", edge: " <<  edge << ", corner1: " << corner1;
                
                ///the edge's endpoints should match one of corner0, or corner1
                ASSERT_TRUE(cubelib_is_corner_equal(corner0, edge_corner1) || cubelib_is_corner_equal(corner1, edge_corner1))
                    << "corner0: " << corner0 << ", edge: " <<  edge << ", corner1: " << corner1;
                
                
                
                
                
                
            }
        }
    }
    
    ///8 corners, each has 3 adjacent corners
    ASSERT_EQ(edge_corner_count, 8*3);

}

TEST_F(CubelibEdgeTest,get_edge_by_corner_direction)
{
    int count = 0;
    
    for (auto corner : cubelib_all_corners)
    {
        for (auto direction : cubelib_all_directions)
        {
            auto next_corner = cubelib_corner_move(corner,direction);
            if (cubelib_is_corner_null(next_corner))
                continue;
            
            ++count;
            
            auto edge = cubelib_get_edge_by_corner_direction(corner,direction);
            
            ASSERT_TRUE(cubelib_is_corner_equal(cubelib_get_edge_corner0(edge), corner)
                        ||
                        cubelib_is_corner_equal(cubelib_get_edge_corner1(edge), corner));
            ASSERT_TRUE(cubelib_is_corner_equal(cubelib_get_edge_corner0(edge), next_corner)
                        ||
                        cubelib_is_corner_equal(cubelib_get_edge_corner1(edge), next_corner));
            
            
            ASSERT_TRUE(cubelib_is_corner_adjacent_corner(cubelib_get_edge_corner0(edge), cubelib_get_edge_corner1(edge)));
            
        }
    }
    
    ASSERT_EQ(8*3,count);
}




TEST_F(CubelibEdgeTest,get_opposite_edge)
{
    for (auto edge : cubelib_all_edges)
    {
        auto corner0 = cubelib_get_edge_corner0(edge);
        auto corner1 = cubelib_get_edge_corner1(edge);
        ASSERT_TRUE(cubelib_is_corner_adjacent_corner(corner0, corner1));
        
        auto opposite_edge = cubelib_get_opposite_edge(edge);
        
        auto opposite_corner0 = cubelib_get_edge_corner0(opposite_edge);
        auto opposite_corner1 = cubelib_get_edge_corner1(opposite_edge);
        
        ASSERT_TRUE(cubelib_is_corner_adjacent_corner(corner0, corner1));
        
        
        ASSERT_TRUE(cubelib_is_corner_equal(cubelib_get_opposite_corner(corner0), opposite_corner1));
        ASSERT_TRUE(cubelib_is_corner_equal(cubelib_get_opposite_corner(corner1), opposite_corner0));
    }
    
    
}


TEST_F(CubelibEdgeTest,face_on_edge)
{
    
    
    uint32_t face_counts[CUBELIB_FACES_SIZE] = {0};
    
    for (auto edge : cubelib_all_edges)
    {
        uint32_t per_edge_face_counts[CUBELIB_EDGES_SIZE] = {0};
        for (int i = 0; i < 2; ++i)
        {
            ///simple sanity
            ASSERT_LT(cubelib_get_edge_index(edge), CUBELIB_EDGES_SIZE);
            
            cubelib_face_t face_get = cubelib_get_face_on_edge(edge, i);
            cubelib_face_t face_calc = cubelib_calc_face_on_edge(edge, i);
            cubelib_face_t face_const = cubelib_faces_on_edge[cubelib_get_edge_index(edge)][i];
            
            ASSERT_TRUE(cubelib_is_face_equal(face_get, face_calc));
            ASSERT_TRUE(cubelib_is_face_equal(face_get, face_const));
            
            
            ASSERT_TRUE(!cubelib_is_face_null(face_get));
            ASSERT_TRUE(cubelib_is_face_on_edge(face_get,edge));
            ASSERT_TRUE(cubelib_is_edge_on_face(edge,face_get));
            
            face_counts[cubelib_get_face_index(face_get)]++;
            per_edge_face_counts[cubelib_get_face_index(face_get)]++;
        }
        
        int unique_faces_in_this_edge = 0;
        for (auto face : cubelib_all_faces)
        {
            ///each edge should have been passed over at MOST 1 times
            ASSERT_LE(per_edge_face_counts[ cubelib_get_face_index(face) ], 1U);
            
            unique_faces_in_this_edge += per_edge_face_counts[ cubelib_get_face_index(face) ];
        }
        ///there should be 2 unique faces in this edge
        ASSERT_EQ(2, unique_faces_in_this_edge);
    }
    
    
    for (auto face : cubelib_all_faces)
    {
        ///each face should have been passed over 4 times
        ASSERT_EQ(4U, face_counts[ cubelib_get_face_index(face) ]);
    }
}
