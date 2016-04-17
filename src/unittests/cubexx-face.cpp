


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
    
    std::vector<uint32_t> all____edge_counts(cubexx::edge_t::SIZE(), 0);
    
    
    for (auto face : cubexx::face_t::all())
    {
        std::vector<uint32_t> face___edge_counts(cubexx::edge_t::SIZE(), 0);
        for (auto edge : face.edges())
        {
            all____edge_counts[edge.index()]++;
            face___edge_counts[edge.index()]++;
            
            
            ASSERT_TRUE(face.is_adjacent(edge));
            ASSERT_TRUE(edge.is_adjacent(face));
            
            ASSERT_TRUE(face.corner_set().contains(edge.corner0()));
            ASSERT_TRUE(face.corner_set().contains(edge.corner1()));
            ASSERT_TRUE(face.edge_set().contains(edge));
            ASSERT_TRUE(edge.face_set().contains(face));
        }

        ASSERT_EQ(face.edge_set(), cubexx::edge_set_t(face.edges()));

        for (auto edge : cubexx::edge_t::all())
        {
            ASSERT_EQ(face.is_adjacent(edge),edge.is_adjacent(face));
            ASSERT_EQ(face.is_adjacent(edge),face.edge_set().contains(edge));
            ASSERT_EQ(face.is_adjacent(edge),edge.face_set().contains(face));

            ASSERT_EQ(1U == face___edge_counts[edge.index()], edge.is_adjacent(face));
            ASSERT_EQ(0U == face___edge_counts[edge.index()], !edge.is_adjacent(face));
        }
    }


    for (auto edge : cubexx::edge_t::all())
    {
        ///each edge is visited twice in total
        ASSERT_EQ(2U, all____edge_counts[edge.index()]);
    }

}


TEST_F(CUBEXXFaceTest,corners)
{

    std::vector<uint32_t> all____corner_counts(cubexx::corner_t::SIZE(), 0);


    for (auto face : cubexx::face_t::all())
    {
        std::vector<uint32_t> face___corner_counts(cubexx::corner_t::SIZE(), 0);
        for (auto corner : face.corners())
        {
            all____corner_counts[corner.index()]++;
            face___corner_counts[corner.index()]++;
            
            
            ASSERT_TRUE(face.is_adjacent(corner));
            ASSERT_TRUE(corner.is_adjacent(face));
            
            ASSERT_TRUE(face.corner_set().contains(corner));
            ASSERT_TRUE(corner.face_set().contains(face));
        }

        ASSERT_EQ(face.corner_set(), cubexx::corner_set_t(face.corners()));

        for (auto corner : cubexx::corner_t::all())
        {
            ASSERT_EQ(face.is_adjacent(corner),corner.is_adjacent(face));
            ASSERT_EQ(face.is_adjacent(corner),face.corner_set().contains(corner));
            ASSERT_EQ(face.is_adjacent(corner),corner.face_set().contains(face));

            ASSERT_EQ(1U == face___corner_counts[corner.index()], corner.is_adjacent(face))
                << "face: " << face
                << ", corner: " << corner;
            ASSERT_EQ(0U == face___corner_counts[corner.index()], !corner.is_adjacent(face));
        }
    }

    for (auto corner : cubexx::corner_t::all())
    {
        ///each corner is visited thrice in total
        ASSERT_EQ(3U, all____corner_counts[corner.index()]);
    }
}

TEST_F(CUBEXXFaceTest,is_adjacent_to_face)
{
    std::vector<uint32_t> all____face_counts(cubexx::face_t::SIZE(),0);

    for (auto face : cubexx::face_t::all())
    {
        std::vector<uint32_t> face___face_counts(cubexx::face_t::SIZE(),0);

        auto opposite = face.opposite();

        ASSERT_TRUE(!face.is_adjacent(face));
        ASSERT_TRUE(!face.is_adjacent(opposite));
        ASSERT_TRUE(!opposite.is_adjacent(face));

        for (auto rhs : cubexx::face_t::all())
        {
            ASSERT_EQ(rhs.is_adjacent(face),face.is_adjacent(rhs));


            ASSERT_EQ(rhs.is_adjacent(face), face.direction().axis() != rhs.direction().axis());
            ASSERT_EQ(rhs.is_adjacent(face), (face.corner_set() & rhs.corner_set()).size() == 2);


        }


        ///count
        for (auto rhs : cubexx::face_t::all())
        {
            if (!rhs.is_adjacent(face))
                continue;
            all____face_counts[rhs.index()]++;
            face___face_counts[rhs.index()]++;
        }
        ///pidgeonhole
        for (auto rhs : cubexx::face_t::all())
        {
            ASSERT_EQ(1U == face___face_counts[rhs.index()], rhs.is_adjacent(face));
            ASSERT_EQ(0U == face___face_counts[rhs.index()], !rhs.is_adjacent(face));
        }

        ///pidgeonhole
        ASSERT_EQ(4U, std::accumulate(face___face_counts.begin(), face___face_counts.end(), 0U));
    }

    ///pidgeonhole
    for (auto face : cubexx::face_t::all())
    {
        ///each face should be visited 4 times
        ASSERT_EQ(4U, all____face_counts[face.index()]);
    }

    ///pidgeonhole
    ///6 faces, each is visited 4 times
    ASSERT_EQ(6U*4U, std::accumulate(all____face_counts.begin(), all____face_counts.end(), 0U));
}


TEST_F(CUBEXXFaceTest,is_adjacent_to_corner)
{
    std::vector<uint32_t> all____corner_counts(cubexx::corner_t::SIZE(),0);

    for (auto face : cubexx::face_t::all())
    {
        std::vector<uint32_t> face___corner_counts(cubexx::corner_t::SIZE(),0);


        auto direction = face.direction();

        for (auto corner : cubexx::corner_t::all())
        {
            ASSERT_EQ(corner.is_adjacent(face),face.is_adjacent(corner));

            auto direction_xyz = direction.xyz();
            auto corner_xyz = corner.xyz();
            auto axis = direction.axis();

            ASSERT_EQ(corner.is_adjacent(face), direction_xyz[axis] == corner_xyz[axis]);
            ASSERT_EQ(corner.is_adjacent(face), corner.face_set().contains(face));
            ASSERT_EQ(corner.is_adjacent(face), face.corner_set().contains(corner));


        }


        ASSERT_EQ(face.corner_set(), cubexx::corner_set_t(face.corners()));

        
        ///count
        for (auto corner : cubexx::corner_t::all())
        {
            if (!corner.is_adjacent(face))
                continue;
            all____corner_counts[corner.index()]++;
            face___corner_counts[corner.index()]++;
        }
        ///pidgeonhole
        for (auto corner : cubexx::corner_t::all())
        {
            ASSERT_EQ(1U == face___corner_counts[corner.index()], corner.is_adjacent(face));
            ASSERT_EQ(0U == face___corner_counts[corner.index()], !corner.is_adjacent(face));
        }

        ///pidgeonhole
        ASSERT_EQ(4U, std::accumulate(face___corner_counts.begin(), face___corner_counts.end(), 0U));
    }

    ///pidgeonhole
    for (auto corner : cubexx::corner_t::all())
    {
        ///each corner should be visited 3 times
        ASSERT_EQ(3U, all____corner_counts[corner.index()]);
    }

    ///pidgeonhole
    ///8 corners, each is visited 3 times
    ASSERT_EQ(3U*8U, std::accumulate(all____corner_counts.begin(), all____corner_counts.end(), 0U));
    ///6 faces, each visits 4 corners
    ASSERT_EQ(4U*6U, std::accumulate(all____corner_counts.begin(), all____corner_counts.end(), 0U));
}


TEST_F(CUBEXXFaceTest,is_adjacent_to_edge)
{
  
    for (auto face : cubexx::face_t::all())
    {
        uint32_t rhs_counts[12] = {0};

        ///check that face_t::edges() produces adjacent edges
        for (auto rhs : face.edges())
        {
          
            ASSERT_TRUE(face.is_adjacent(rhs))
                << "face: " << face
                << ", edge: " << rhs;
            ASSERT_TRUE(rhs.is_adjacent(face));
            ASSERT_EQ(0U, rhs_counts[rhs.index()]);
            rhs_counts[rhs.index()]++;
        }

        ///check the contrapositive
        for (auto rhs : cubexx::edge_t::all())
        {
            if (rhs_counts[rhs.index()] != 0)
            {
                ASSERT_EQ(1U, rhs_counts[rhs.index()]);
            
                ASSERT_TRUE(face.is_adjacent(rhs));
                ASSERT_TRUE(rhs.is_adjacent(face));
            } else {
            
                ASSERT_FALSE(face.is_adjacent(rhs));
                ASSERT_FALSE(rhs.is_adjacent(face));
            }
        }
    }

    ///check the adjacency based on the axes
    for (auto face : cubexx::face_t::all())
    {
        auto direction = face.direction();

        for (auto edge : cubexx::edge_t::all())
        {
            for (auto corner : edge.corners())
            {
        
        
                int corner_xyz[] = {corner.x(), corner.y(), corner.z()};
                int direction_xyz[] = {direction.x(), direction.y(), direction.z()};
            
              
                if (face.is_adjacent(edge))
                {
                    ASSERT_NE(edge.base_axis(), face.direction().axis());
                
                    ///both corners will be in the same direction as the face
                    ASSERT_EQ(direction_xyz[direction.axis()], corner_xyz[direction.axis()]);
                } else {
                    ///EACH corners will NOT be in the same direction OR the edge will be parallel to the face
                    ASSERT_TRUE((direction_xyz[direction.axis()] != corner_xyz[direction.axis()]) || edge.base_axis() == direction.axis());
                  
                }
            }
        }
    }
    ///check the adjacency based on corner sets
    for (auto face : cubexx::face_t::all())
    {

        for (auto edge : cubexx::face_t::all())
        {
            ///iff the two corner sets have two corners in common, they are adjacent
            ASSERT_EQ(face.is_adjacent(edge), ((edge.corner_set() & face.corner_set()).size() == 2))
                << "face: " << face << ", edge: " << edge
                << ", face.corner_set(): " << face.corner_set()
                << ", edge.corner_set(): " << edge.corner_set()
                << ", (edge.corner_set() & face.corner_set()): " << (edge.corner_set() & face.corner_set())
                ;
          
        }
    }
}


TEST_F(CUBEXXFaceTest,flip)
{
    std::vector<uint32_t> all____face_counts(cubexx::face_t::SIZE(), 0);
    for (auto face : cubexx::face_t::all())
    {
        std::vector<uint32_t> face___face_counts(cubexx::face_t::SIZE(), 0);
        for (auto edge : face.edges()){
            if (!edge.is_adjacent(face))
                continue;

            auto neighbor = face.flip(edge);

            ASSERT_TRUE(face.is_adjacent(neighbor));
            ASSERT_TRUE(neighbor.is_adjacent(face));

            ASSERT_EQ(cubexx::edge_set_t(edge), (face.edge_set() & neighbor.edge_set()));

            face___face_counts[neighbor.index()]++;
            all____face_counts[neighbor.index()]++;
        }

        ///pidgeonhole
        for (auto other : cubexx::face_t::all())
        {
            ASSERT_EQ(1U == face___face_counts[other.index()], face.is_adjacent(other));
            ASSERT_EQ(0U == face___face_counts[other.index()], !face.is_adjacent(other));
        }

        ///pidgeonhole
        ASSERT_EQ(4U, std::accumulate(face___face_counts.begin(), face___face_counts.end(), 0U));
    }


    ///pidgeonhole
    for (auto face : cubexx::face_t::all())
    {
        ASSERT_EQ(4U, all____face_counts[face.index()]);
    }

    ///pidgeonhole
    ASSERT_EQ(6U*4U, std::accumulate(all____face_counts.begin(), all____face_counts.end(), 0U));
}


TEST_F(CUBEXXFaceTest,perpendicular_edges)
{
  ASSERT_TRUE(false);
}

