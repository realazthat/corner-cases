


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



TEST_F(CUBEXXEdgeTest,opposite_over_face)
{
    
    for (auto edge : cubexx::edge_t::all())
    {
        for (auto face : edge.faces())
        {
            ASSERT_TRUE(face.is_adjacent(edge));
            ASSERT_TRUE(edge.is_adjacent(face));
            
            //edge is one of the edges of the face
            ASSERT_TRUE(face.edge_set().contains(edge));
            
            auto opposite_edge = edge.opposite(face);
            
            //the two edges are not equal
            ASSERT_NE(edge, opposite_edge);
            
            //opposite edge is one of the edges of the face
            ASSERT_TRUE(face.edge_set().contains(opposite_edge));
            //edge and the opposite edge do not share corners
            ASSERT_TRUE((edge.corner_set() & opposite_edge.corner_set()).size() == 0);
            //the two edges cover all of the faces corners
            ASSERT_TRUE((face.corner_set() - edge.corner_set() - opposite_edge.corner_set()).size() == 0);
        }
    }
}

TEST_F(CUBEXXEdgeTest,get_by_axis)
{

    ///test edge get(base_axis,project_secondary,project_tertiary)
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
            
            ASSERT_EQ(0U, mask & (1 << edge.index()));
            mask |= (1 << edge.index());
        }
        
        EXPECT_EQ(mask, uint32_t(1 << cubexx::edge_t::SIZE()) - 1);

    }
}


TEST_F(CUBEXXEdgeTest,get_by_corners)
{

    ///test edge get(corner0, corner1)
    {
        std::vector<int> edge_counts(cubexx::edge_t::SIZE(), 0);
        
        for (auto base_corner : cubexx::corner_t::all())
        for (auto direction : cubexx::direction_t::all())
        {
            auto adjacent_corner = base_corner.adjacent(direction);
            
            auto edge = cubexx::edge_t::get(base_corner, adjacent_corner);
            
            ASSERT_TRUE(edge.corner_set().contains(base_corner));
            ASSERT_TRUE(edge.corner_set().contains(adjacent_corner));
            ASSERT_TRUE(edge.corner0() == base_corner || edge.corner0() == adjacent_corner);
            ASSERT_TRUE(edge.corner1() == base_corner || edge.corner1() == adjacent_corner);
            
            edge_counts.at(edge.index())++;
        }
        
        
        ///every edge should be covered twice
        for (auto edge : cubexx::edge_t::all())
        {
            ASSERT_EQ(2, edge_counts.at(edge.index()));
        }

    }
}


TEST_F(CUBEXXEdgeTest,faces)
{

    ///test edge_t::faces()
    {
        std::vector<int> face_counts(cubexx::face_t::SIZE(), 0);
        
        
        for (auto edge : cubexx::edge_t::all())
        {
            for (auto face : edge.faces())
            {
                face_counts[face.index()]++;
                
                
                ASSERT_TRUE(face.is_adjacent(edge));
                ASSERT_TRUE(edge.is_adjacent(face));
                
                ASSERT_TRUE(face.corner_set().contains(edge.corner0()));
                ASSERT_TRUE(face.corner_set().contains(edge.corner1()));
            }
        }

    }
}


TEST_F(CUBEXXEdgeTest,adjacents)
{

    
    uint32_t adjacent_edge_counts[12] = {0};
    
    for (auto edge : cubexx::edge_t::all())
    {
        for (auto adj_edge : edge.adjacent_edges())
        {
            ASSERT_TRUE(edge.is_adjacent(adj_edge));
            ASSERT_TRUE(adj_edge.is_adjacent(edge));
            
            adjacent_edge_counts[adj_edge.index()]++;
        }
        
        ASSERT_EQ(cubexx::edge_set_t(edge.adjacent_edges()), edge.adjacent_edge_set());
    }
    
    for (auto edge : cubexx::edge_t::all())
    {
        ///each edge should be covered 4 times
        ASSERT_EQ(4U, adjacent_edge_counts[edge.index()]);
    }
}


TEST_F(CUBEXXEdgeTest,adjacent_to_corner)
{

    ASSERT_TRUE(false);
}

TEST_F(CUBEXXEdgeTest,corners)
{

    ASSERT_TRUE(false);
}

TEST_F(CUBEXXEdgeTest,end_faces)
{

    ASSERT_TRUE(false);
}

TEST_F(CUBEXXEdgeTest,is_adjacent_to_edge)
{

    for (auto edge : cubexx::edge_t::all())
    {
        for (auto adj_edge : edge.adjacent_edges())
        {
            ASSERT_TRUE(edge.is_adjacent(adj_edge));
            ASSERT_TRUE(adj_edge.is_adjacent(edge));
        }
        for (auto adj_edge : edge.adjacent_edge_set())
        {
            ASSERT_TRUE(edge.is_adjacent(adj_edge));
            ASSERT_TRUE(adj_edge.is_adjacent(edge));
        }
        
        for (auto other_edge : cubexx::edge_t::all())
        {
            if (other_edge.is_adjacent(edge))
            {
                ASSERT_TRUE(edge.is_adjacent(other_edge));
            } else {
                ASSERT_FALSE(edge.is_adjacent(other_edge));
            }
        }
    }
}

TEST_F(CUBEXXEdgeTest,is_adjacent_to_face)
{

    ASSERT_TRUE(false);
}


TEST_F(CUBEXXEdgeTest,is_adjacent_to_corner)
{

    ASSERT_TRUE(false);
}
