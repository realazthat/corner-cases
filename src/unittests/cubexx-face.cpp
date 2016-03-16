


#include <regex>
#include <set>
#include <map>
#include <tuple>

#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include "gtest/gtest.h"
#include "format.h"







struct CUBEXXFaceTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};



TEST_F(CUBEXXFaceTest,SIZE)
{
    ASSERT_EQ(6U, cubexx::face_t::SIZE());
}

TEST_F(CUBEXXFaceTest,index)
{


    ///test face_t_t::index() and face_t_t::get(index)
    uint32_t index = 0;
    for (auto face : cubexx::face_t::all())
    {
        EXPECT_EQ(index, face.index());
        
        EXPECT_EQ(face, cubexx::face_t::get(index));
        
        ++index;
    }
}


TEST_F(CUBEXXFaceTest,is_null)
{
    for (auto face : cubexx::face_t::all())
    {
        ASSERT_FALSE(face.is_null());
    }
    
    
    ASSERT_TRUE(cubexx::face_t().is_null());
    ASSERT_TRUE(cubexx::face_t::null_face().is_null());
}

TEST_F(CUBEXXFaceTest,is_sane)
{
    for (auto face : cubexx::face_t::all())
    {
        ASSERT_TRUE(face.is_sane());
    }
    ASSERT_TRUE(cubexx::face_t().is_sane());
    ASSERT_TRUE(cubexx::face_t::null_face().is_sane());
}

TEST_F(CUBEXXFaceTest,unique)
{
    
    ///test face indices
    {
        ///mark each face's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (auto face : cubexx::face_t::all())
        {
            bitmask |= (1 << face.index());
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << cubexx::face_t::SIZE()) - 1);
    }
    
    
    std::map< std::tuple<int,int,int>, cubexx::face_t > key2value;
    std::map< std::size_t, cubexx::face_t > index_values;
    for (auto face : cubexx::face_t::all())
    {
        int x = face.direction().x();
        int y = face.direction().y();
        int z = face.direction().z();
        
        
        auto key = std::make_tuple(x,y,z);
        if (key2value.count(key) > 0)
        {
            auto face0 = key2value[key];
            ASSERT_TRUE( key2value.count(key) == 0 )
                << "face: " << face
                << ", (x,y,z): "
                    << "("
                    << (int)x
                    << "," << (int)y
                    << "," << (int)z
                    << ")"
                << ", face0: " << face0;
        }
        key2value[key] = face;
        
        if (index_values.count(face.index()) > 0)
        {
            
            auto face0 = index_values[face.index()];
            ASSERT_TRUE( index_values.count(face.index()) == 0 )
                << "face: " << face
                << ", (x,y,z): "
                    << "("
                    << (int)x
                    << "," << (int)y
                    << "," << (int)z
                    << ")"
                << ", face0: " << face0;
            
        }
        index_values[face.index()] = face;
        
    }
}

TEST_F(CUBEXXFaceTest,opposite)
{

    ///test face_t::opposite()
    {
        
        
        for (auto face : cubexx::face_t::all())
        {
            
            auto opposite_face = face.opposite();
            
            
            EXPECT_NE(face, opposite_face);
            EXPECT_EQ(face, opposite_face.opposite());
            //the axis remains the same
            EXPECT_EQ(face.direction().axis(), opposite_face.direction().axis());
            EXPECT_NE(face.direction().positive(), opposite_face.direction().positive());
        }
        
    }
}




TEST_F(CUBEXXFaceTest,direction)
{
    ASSERT_EQ(cubexx::direction_t::SIZE(), cubexx::face_t::SIZE());
    
    for (auto face : cubexx::face_t::all())
    {
        auto face_direction = face.direction();
        
        ASSERT_EQ(face_direction, face.direction());
        ASSERT_EQ(face, face_direction.face());
        
        ///pidgeonhole it:
        ///1. count the number of directions that match the face
        ///2. make sure all the directions either match or do not match
        ///3. and finally make sure that there was only one match
        uint32_t equal_direction_count = 0;
        for (auto direction : cubexx::direction_t::all())
        {
            if (direction == face_direction)
            {
                equal_direction_count++;
                
                ASSERT_EQ(direction.face(), face);
                ASSERT_EQ(direction, face.direction());
            } else {
                ASSERT_NE(direction.face(), face);
            }
        }
        ASSERT_EQ(1U, equal_direction_count);
    }
}



TEST_F(CUBEXXFaceTest,edges)
{

    ASSERT_TRUE(false);
    
    ///test face_t::edges()
    // {
        // std::vector<int> face_counts(cubexx::face_t::SIZE(), 0);
        
        
        // for (auto edge : cubexx::edge_t::all())
        // {
            // for (auto face : edge.faces())
            // {
                // face_counts[face.index()]++;
                
                
                // ASSERT_TRUE(face.is_adjacent(edge));
                // ASSERT_TRUE(edge.is_adjacent(face));
                
                // ASSERT_TRUE(face.corner_set().contains(edge.corner0()));
                // ASSERT_TRUE(face.corner_set().contains(edge.corner1()));
            // }
        // }

    // }
}


TEST_F(CUBEXXFaceTest,corners)
{

    ASSERT_TRUE(false);
    
    ///test face_t::edges()
    // {
        // std::vector<int> face_counts(cubexx::face_t::SIZE(), 0);
        
        
        // for (auto edge : cubexx::edge_t::all())
        // {
            // for (auto face : edge.faces())
            // {
                // face_counts[face.index()]++;
                
                
                // ASSERT_TRUE(face.is_adjacent(edge));
                // ASSERT_TRUE(edge.is_adjacent(face));
                
                // ASSERT_TRUE(face.corner_set().contains(edge.corner0()));
                // ASSERT_TRUE(face.corner_set().contains(edge.corner1()));
            // }
        // }

    // }
}

TEST_F(CUBEXXFaceTest,is_adjacent_face)
{
  ASSERT_TRUE(false);
}


TEST_F(CUBEXXFaceTest,is_adjacent_corner)
{
  ASSERT_TRUE(false);
}


TEST_F(CUBEXXFaceTest,is_adjacent_edge)
{
  ASSERT_TRUE(false);
}


TEST_F(CUBEXXFaceTest,flip)
{
  ASSERT_TRUE(false);
}


TEST_F(CUBEXXFaceTest,perpendicular_edges)
{
  ASSERT_TRUE(false);
}

