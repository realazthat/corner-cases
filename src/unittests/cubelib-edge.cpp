


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
        EXPECT_TRUE(is_edge_valid(null_edge));
        EXPECT_TRUE(is_edge_null(null_edge));
        EXPECT_TRUE(is_edge_equal(null_edge, null_edge));
    }
    
    ///make sure all the regular edges are not null_edge
    for (auto edge : all_edges)
    {
        EXPECT_FALSE(is_edge_null(edge));
        EXPECT_TRUE(is_edge_valid(edge));
        EXPECT_FALSE(is_edge_equal(edge, null_edge));
    }

}



TEST_F(CubelibEdgeTest,indices)
{

    ///test edge indices
    {
        ///mark each edge's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (edge_t edge : all_edges)
        {
            bitmask |= (1 << get_edge_index(edge));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << 12) - 1);
        
        
        
        ///test get_edge_by_index()
        uint32_t index = 0;
        for (edge_t edge : all_edges)
        {
            EXPECT_EQ(index, get_edge_index(edge));
            
            EXPECT_TRUE(is_edge_equal(edge, get_edge_by_index(index)));
            EXPECT_FALSE(is_edge_null(edge));
            EXPECT_TRUE(is_edge_valid(edge));
            
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
        for (auto edge : all_edges)
        {
            bitmask |= (1 << get_edge_index(edge));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << EDGES_SIZE) - 1);
    }
    
    
    std::map< std::tuple<uint8_t,bool,bool>, edge_t > base_projection_values;
    std::map< std::size_t, edge_t > index_values;
    for (auto edge : all_edges)
    {
        auto base_axis = get_edge_base_axis(edge);
        auto project_secondary = is_edge_projected_secondary(edge);
        auto project_tertiary = is_edge_projected_tertiary(edge);
        
        
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
        
        if (index_values.count(get_edge_index(edge)) > 0)
        {
            
            auto edge0 = index_values[get_edge_index(edge)];
            ASSERT_TRUE( index_values.count(get_edge_index(edge)) == 0 )
                << "edge: " << edge
                << ", (base_axis,project_secondary,project_tertiary): "
                    << "("
                    << (int)base_axis
                    << "," << (project_secondary ? "true" : "false")
                    << "," << (project_tertiary ? "true" : "false")
                    << ")"
                << ", edge0: " << edge0;
            
        }
        index_values[get_edge_index(edge)] = edge;
        
    }
}



TEST_F(CubelibEdgeTest,get_edge_corner)
{
    for (auto edge0 : all_edges)
    {
        auto corner0 = get_edge_corner0(edge0);
        auto corner1 = get_edge_corner1(edge0);
        
        
        
        auto edge1 = get_edge_by_corners(corner0, corner1);
        
        ASSERT_TRUE(is_edge_equal(edge1,edge0));
        auto edge2 = get_edge_by_corners(corner1, corner0);
        
        ASSERT_TRUE(is_edge_equal(edge2,edge0));
    }
    
    ///for every pair of adjacent corners, check the edge
    for (auto corner0 : all_corners){
        for (auto direction : all_directions){
            auto corner1 = get_adjacent_corner(corner0, direction);
            ASSERT_TRUE(!is_corner_null(corner1));
            ASSERT_TRUE(is_corner_adjacent_corner(corner0, corner1));
            
            auto edge = get_edge_by_corners(corner0, corner1);
            
            ///the edge's corners are equal to one of corner0, corner1
            ASSERT_TRUE(is_corner_equal(get_edge_corner0(edge), corner0) || is_corner_equal(get_edge_corner0(edge), corner1));
            ASSERT_TRUE(is_corner_equal(get_edge_corner1(edge), corner0) || is_corner_equal(get_edge_corner1(edge), corner1));
            
            ///the edge did not return two equal corners
            ASSERT_FALSE(is_corner_equal(get_edge_corner0(edge), get_edge_corner1(edge)));
        }
    }
    
    
    
}











TEST_F(CubelibEdgeTest,get_edge_by_corners)
{

    int edge_corner_count = 0;
    
    ///test get_edge_by_corners() with two adjacent corners
    {
        for (corner_t corner0 : all_corners)
        {
            for (corner_t corner1 : all_corners)
            {
                if (!is_corner_adjacent_corner(corner0,corner1))
                    continue;
                edge_corner_count++;
                
                edge_t edge = get_edge_by_corners(corner0,corner1);
                
                
                
                auto edge_corner0 = get_edge_corner0(edge);
                auto edge_corner1 = get_edge_corner1(edge);
                
                ///the edge's endpoints should match one of corner0, or corner1
                ASSERT_TRUE(is_corner_equal(corner0, edge_corner0) || is_corner_equal(corner1, edge_corner0))
                    << "corner0: " << corner0 << ", edge: " <<  edge << ", corner1: " << corner1;
                
                ///the edge's endpoints should match one of corner0, or corner1
                ASSERT_TRUE(is_corner_equal(corner0, edge_corner1) || is_corner_equal(corner1, edge_corner1))
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
    
    for (auto corner : all_corners)
    {
        for (auto direction : all_directions)
        {
            auto next_corner = corner_move(corner,direction);
            if (is_corner_null(next_corner))
                continue;
            
            ++count;
            
            auto edge = get_edge_by_corner_direction(corner,direction);
            
            ASSERT_TRUE(is_corner_equal(get_edge_corner0(edge), corner)
                        ||
                        is_corner_equal(get_edge_corner1(edge), corner));
            ASSERT_TRUE(is_corner_equal(get_edge_corner0(edge), next_corner)
                        ||
                        is_corner_equal(get_edge_corner1(edge), next_corner));
            
            
            ASSERT_TRUE(is_corner_adjacent_corner(get_edge_corner0(edge), get_edge_corner1(edge)));
            
        }
    }
    
    ASSERT_EQ(8*3,count);
}




TEST_F(CubelibEdgeTest,get_opposite_edge)
{
    for (auto edge : all_edges)
    {
        auto corner0 = get_edge_corner0(edge);
        auto corner1 = get_edge_corner1(edge);
        ASSERT_TRUE(is_corner_adjacent_corner(corner0, corner1));
        
        auto opposite_edge = get_opposite_edge(edge);
        
        auto opposite_corner0 = get_edge_corner0(opposite_edge);
        auto opposite_corner1 = get_edge_corner1(opposite_edge);
        
        ASSERT_TRUE(is_corner_adjacent_corner(corner0, corner1));
        
        
        ASSERT_TRUE(is_corner_equal(get_opposite_corner(corner0), opposite_corner1));
        ASSERT_TRUE(is_corner_equal(get_opposite_corner(corner1), opposite_corner0));
    }
    
    
}


