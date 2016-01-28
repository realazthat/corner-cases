


#include <regex>
#include <set>
#include <map>
#include <tuple>

#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include "gtest/gtest.h"
#include "format.h"







struct CUBEXXEdgeTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};



TEST_F(CUBEXXEdgeTest,SIZE)
{
    ASSERT_EQ(12U, cubexx::edge_t::SIZE());
}

TEST_F(CUBEXXEdgeTest,index)
{


    ///test edge_t_t::index() and edge_t::get(index)
    uint32_t index = 0;
    for (auto edge : cubexx::edge_t::all())
    {
        EXPECT_EQ(index, edge.index());
        
        EXPECT_EQ(edge, cubexx::edge_t::get(index));
        
        ++index;
    }
}


TEST_F(CUBEXXEdgeTest,is_null)
{
    for (auto edge : cubexx::edge_t::all())
    {
        ASSERT_FALSE(edge.is_null());
    }
    
    
    ASSERT_TRUE(cubexx::edge_t().is_null());
    ASSERT_TRUE(cubexx::edge_t::null_edge().is_null());
}

TEST_F(CUBEXXEdgeTest,is_sane)
{
    for (auto edge : cubexx::edge_t::all())
    {
        ASSERT_TRUE(edge.is_sane());
    }
    ASSERT_TRUE(cubexx::edge_t().is_sane());
    ASSERT_TRUE(cubexx::edge_t::null_edge().is_sane());
}

TEST_F(CUBEXXEdgeTest,unique)
{
    
    ///test edge indices
    {
        ///mark each edge's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (auto edge : cubexx::edge_t::all())
        {
            bitmask |= (1 << edge.index());
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << cubexx::edge_t::SIZE()) - 1);
    }
    
    
    std::map< std::tuple<uint8_t,bool,bool>, cubexx::edge_t > base_projection_values;
    std::map< std::size_t, cubexx::edge_t > index_values;
    for (auto edge : cubexx::edge_t::all())
    {
        auto base_axis = edge.base_axis();
        auto project_secondary = edge.project_secondary();
        auto project_tertiary = edge.project_tertiary();
        
        
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
        
        if (index_values.count(edge.index()) > 0)
        {
            
            auto edge0 = index_values[edge.index()];
            ASSERT_TRUE( index_values.count(edge.index()) == 0 )
                << "edge: " << edge
                << ", (base_axis,project_secondary,project_tertiary): "
                    << "("
                    << (int)base_axis
                    << "," << (project_secondary ? "true" : "false")
                    << "," << (project_tertiary ? "true" : "false")
                    << ")"
                << ", edge0: " << edge0;
            
        }
        index_values[edge.index()] = edge;
        
    }
}

TEST_F(CUBEXXEdgeTest,opposite)
{

    ///test edge_t::opposite()
    {
        
        
        for (auto edge : cubexx::edge_t::all())
        {
            
            auto opposite_edge = edge.opposite();
            
            
            EXPECT_NE(edge, opposite_edge);
            EXPECT_EQ(edge, opposite_edge.opposite());
            //the base axis remains the same
            EXPECT_EQ(opposite_edge.base_axis(), edge.base_axis());
            //but the axis projection across the cube is reversed
            EXPECT_NE(opposite_edge.project_secondary(), edge.project_secondary());
            EXPECT_NE(opposite_edge.project_tertiary(), edge.project_tertiary());
            
        }
        
    }
}




TEST_F(CUBEXXEdgeTest,get)
{

    ///test edge get
    {
        uint32_t mask = 0;
        
        for (int base_axis = 0; base_axis < 3; ++base_axis)
        for (bool project_secondary : {false, true})
        for (bool project_tertiary : {false, true})
        {
            auto edge = cubexx::edge_t::get(base_axis,project_secondary,project_tertiary);
            
            EXPECT_EQ(edge.base_axis(), base_axis);
            EXPECT_EQ(edge.project_secondary(), project_secondary);
            EXPECT_EQ(edge.project_tertiary(), project_tertiary);
            
            mask |= (1 << edge.index());
        }
        
        EXPECT_EQ(mask, uint32_t(1 << cubexx::edge_t::SIZE()) - 1);

    }
}

