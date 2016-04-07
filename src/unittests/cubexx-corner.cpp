


#include <regex>
#include <set>
#include <map>
#include <tuple>

#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include "gtest/gtest.h"
#include "format.h"







struct CUBEXXCornerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};




TEST_F(CUBEXXCornerTest,SIZE)
{
    ASSERT_EQ(8U, cubexx::corner_t::SIZE());
}

TEST_F(CUBEXXCornerTest,index)
{


    ///test corner_t::index() and corner_t::get(index)
    uint32_t index = 0;
    for (auto corner : cubexx::corner_t::all())
    {
        EXPECT_EQ(index, corner.index());
        
        EXPECT_EQ(corner, cubexx::corner_t::get(index));
        
        ++index;
    }
}

TEST_F(CUBEXXCornerTest,index_morton)
{


    ///test corner_t::index() and corner_t::get(index)
    uint32_t index = 0;
    for (auto corner : cubexx::corner_t::all())
    {
        int x = (index >> 0) & 1;
        int y = (index >> 1) & 1;
        int z = (index >> 2) & 1;
        
        ASSERT_EQ(x, corner.ux());
        ASSERT_EQ(y, corner.uy());
        ASSERT_EQ(z, corner.uz());
        
        ++index;
    }
}

TEST_F(CUBEXXCornerTest,unique)
{

    ///test corner indices
    {
        ///mark each corner's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (auto corner : cubexx::corner_t::all())
        {
            bitmask |= (1 << corner.index());
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << cubexx::corner_t::SIZE()) - 1);
    }
    
    
    std::map< std::tuple<int,int,int>, cubexx::corner_t > xyz_values;
    std::map< std::size_t, cubexx::corner_t > index_values;
    for (auto corner : cubexx::corner_t::all())
    {
        int x = corner.x(), y = corner.y(), z = corner.z();
        

        if (xyz_values.count(std::make_tuple(x,y,z)) > 0)
        {
            auto corner0 = xyz_values[std::make_tuple(x,y,z)];
            ASSERT_TRUE( xyz_values.count(std::make_tuple(x,y,z)) == 0 )
                << "corner: " << corner
                << ", (x,y,z): " << "(" << x << "," << y << "," << z << ")"
                << ", corner0: " << corner0;
        }
        xyz_values[std::make_tuple(x,y,z)] = corner;
        
        if (index_values.count(corner.index()) > 0)
        {
            
            auto corner0 = index_values[corner.index()];
            ASSERT_TRUE( index_values.count(corner.index()) == 0 )
                << "corner: " << corner
                << ", (x,y,z): " << "(" << x << "," << y << "," << z << ")"
                << ", corner0: " << corner0;
            
        }
        index_values[corner.index()] = corner;
        
    }
}

TEST_F(CUBEXXCornerTest,opposite)
{

    ///test corner_t::opposite()
    {
        
        
        for (auto corner : cubexx::corner_t::all())
        {
            
            auto opposite_corner = corner.opposite();
            
            
            EXPECT_NE(corner, opposite_corner);
            EXPECT_EQ(corner, opposite_corner.opposite());
            EXPECT_EQ(-opposite_corner.x(), corner.x());
            EXPECT_EQ(-opposite_corner.y(), corner.y());
            EXPECT_EQ(-opposite_corner.z(), corner.z());
            
        }
        
    }
}




TEST_F(CUBEXXCornerTest,get)
{

    ///test corner get
    {
        uint32_t mask = 0;
        
        for (int x = -1; x <= 1; x += 2)
        for (int y = -1; y <= 1; y += 2)
        for (int z = -1; z <= 1; z += 2)
        {
            auto corner = cubexx::corner_t::get(x,y,z);
            
            EXPECT_EQ(corner.x(), x);
            EXPECT_EQ(corner.y(), y);
            EXPECT_EQ(corner.z(), z);
            
            mask |= (1 << corner.index());
        }
        
        EXPECT_EQ(mask, uint32_t(1 << cubexx::corner_t::SIZE()) - 1);

    }
}

TEST_F(CUBEXXCornerTest,adjacent_direction)
{

    ///test corner adjacent(direction_t)
    {
        
        for (auto corner0 : cubexx::corner_t::all())
        {
            for (auto direction : cubexx::direction_t::all())
            {
                auto corner1 = corner0.adjacent(direction);
                
                ASSERT_NE(corner0, corner1) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                
                
                ASSERT_TRUE(corner0.is_adjacent(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                
                
                bool expected_out_of_bounds = false;
                int xyz0[] = {corner0.x(), corner0.y(), corner0.z()};
                int dir[] = {direction.x(), direction.y(), direction.z()};
                int xyz1_expected[] = {0,0,0};
                for (int i = 0; i < 3; ++i) {
                    ///add the direction in
                    xyz1_expected[i] = xyz0[i] + dir[i]*2;
                    ///if it is out of bounds, move it around and back to the opposite side
                    if (xyz1_expected[i] > 1) {
                        xyz1_expected[i] = -1;
                        expected_out_of_bounds = true;
                    } else if (xyz1_expected[i] < -1) {
                        xyz1_expected[i] = 1;
                        expected_out_of_bounds = true;
                    } else {
                        
                    }
                }
                
                
                
                
                int xyz1_actual[] = {corner1.x(), corner1.y(), corner1.z()};
                
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(std::abs(xyz1_actual[i] - xyz0[i]), std::abs(2*dir[i]))
                            << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1
                            << ", i: " << i;
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(xyz1_expected[i], xyz1_actual[i])
                            << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1
                            << ", i: " << i;
                
            }
        }
    }
}


TEST_F(CUBEXXCornerTest,push_direction)
{

    ///test corner push(direction_t)
    {
        
        for (auto corner0 : cubexx::corner_t::all())
        {
            for (auto direction : cubexx::direction_t::all())
            {
                int xyz0[] = {corner0.x(),corner0.y(), corner0.z()};
                int dir[] = {direction.x(),direction.y(), direction.z()};
                int xyz1_expected[] = {0,0,0};
                
                bool expected_out_of_bounds = false;
                for (int i = 0; i < 3; ++i)
                {
                    xyz1_expected[i] = xyz0[i] + 2*dir[i];
                    
                    if (xyz1_expected[i] > 1)
                    {
                        xyz1_expected[i] = 1;
                        expected_out_of_bounds = true;
                    } else if (xyz1_expected[i] < -1)
                    {
                        xyz1_expected[i] = -1;
                        expected_out_of_bounds = true;
                    }
                }
                
                
                auto corner1 = corner0.push(direction);
                int xyz1_actual[] = {corner1.x(),corner1.y(), corner1.z()};
                
                
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(xyz1_expected[i], xyz1_actual[i])
                            << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1
                            << ", i: " << i;
                
                if (corner0 == corner1)
                {
                    
                    
                    continue;
                }
                
                
                ASSERT_TRUE(corner0.is_adjacent(corner1));
                
                
                
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(xyz1_actual[i] - xyz0[i], 2*dir[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(xyz1_actual[i], xyz1_expected[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                
            }
        }
    }
}

TEST_F(CUBEXXCornerTest,move_direction)
{
    for (auto corner : cubexx::corner_t::all())
    {
        auto xyz = corner.xyz();

        for (auto direction : cubexx::direction_t::all())
        {
            auto next_corner = corner.move(direction);


            auto expected_next_xyz = xyz;

            for (std::size_t i = 0; i < 3; ++i)
                expected_next_xyz[i] += direction.xyz()[i]*2;


            bool expected_out_of_bounds = false;
            for (std::size_t i = 0; i < 3; ++i)
                expected_out_of_bounds |= (std::abs(expected_next_xyz[i]) > 1);

            ASSERT_EQ(expected_out_of_bounds, next_corner.is_null())
                        << "expected_out_of_bounds: " << (expected_out_of_bounds ? "true" : "false")
                        << ", corner: " << corner
                        << ", direction: " << direction
                        << ", next_corner: " << next_corner
                        << ", expected_next_xyz: " << (int)expected_next_xyz[0] << ", " << (int)expected_next_xyz[1] << ", " << (int)expected_next_xyz[2]
                        ;

            if (next_corner.is_null())
                continue;

            auto next_xyz = next_corner.xyz();

            ASSERT_EQ(expected_next_xyz, next_xyz);
        }
    }
}

TEST_F(CUBEXXCornerTest,is_adjacent)
{

    ///test corner is_adjacent(corner_t)
    {
        
        for (auto corner0 : cubexx::corner_t::all())
        {
            
            for (auto corner1 : cubexx::corner_t::all())
            {
                
                ASSERT_EQ(corner0.is_adjacent(corner1), corner1.is_adjacent(corner0))
                    << "corner0: " << corner0 << ", corner1: " << corner1;
                
                
                if (corner0 == corner1)
                {
                    ASSERT_FALSE(corner0.is_adjacent(corner1))
                        << "corner0: " << corner0 << ", corner1: " << corner1;
                }
                
                if (corner0.is_adjacent(corner1))
                    ASSERT_TRUE( !corner0.is_adjacent(corner1) || corner0 != corner1 )
                        << "adjacency =implies> corner0 != corner1."
                        << " corner0: " << corner0
                        << ", corner1: " << corner1
                        << ", corner0.is_adjacent(corner1): " << (corner0.is_adjacent(corner1) ? "true" : "false");
                
                int deltas[] = {corner1.x() - corner0.x(), corner1.y() - corner0.y(), corner1.z() - corner0.z()};
                
                int absdeltas[] = {std::abs(deltas[0]), std::abs(deltas[1]), std::abs(deltas[2]) };
                
                
                int absdeltassum = absdeltas[0] + absdeltas[1] + absdeltas[2];
                
                ///iff the sum of all the absolute deltas is 2, then it is adjacent
                ///in other words: if the corners differ from eachother in only one dimension, they are
                /// adjacent.
                ASSERT_EQ(corner0.is_adjacent(corner1), absdeltassum == 2)
                    << "corner0: " << corner0 << ", corner1: " << corner1
                    << ", absdeltassum: " << absdeltassum;
            }
        }
    }
}



TEST_F(CUBEXXCornerTest,adjacents)
{
  uint32_t adjacent_corner_counts[8] = {0};
  for (auto lhs : cubexx::corner_t::all())
  {
    uint32_t rhs_counts[8] = {0};
    for (auto rhs : lhs.adjacents())
    {
      ASSERT_TRUE(lhs.is_adjacent(rhs));
      ASSERT_TRUE(rhs.is_adjacent(lhs));
      
      ASSERT_EQ(0U, rhs_counts[rhs.index()]);
      rhs_counts[rhs.index()]++;
      adjacent_corner_counts[rhs.index()]++;
    }
    
    
    
    ///check the contrapositive
    for (auto rhs : cubexx::corner_t::all())
    {
      
      if (rhs_counts[rhs.index()] != 0)
      {
        ASSERT_EQ(1U, rhs_counts[rhs.index()]);
        ASSERT_TRUE(lhs.is_adjacent(rhs));
        ASSERT_TRUE(rhs.is_adjacent(lhs));
      } else {
        ASSERT_FALSE(lhs.is_adjacent(rhs));
        ASSERT_FALSE(rhs.is_adjacent(lhs));
        
      }
    }
    
  }
  
  ///each corner should be covered 3 times
  for (auto corner : cubexx::corner_t::all())
  {
    ASSERT_EQ(3U, adjacent_corner_counts[corner.index()]);
  }
}

TEST_F(CUBEXXCornerTest,faces)
{

    std::vector<uint32_t> all___face_visits(cubexx::face_t::SIZE(), 0);

    for (auto corner : cubexx::corner_t::all())
    {
        ASSERT_EQ(corner.face_set(), cubexx::face_set_t(corner.faces()));

        std::vector<uint32_t> corner___face_visits(cubexx::face_t::SIZE(), 0);

        for (auto face : corner.faces())
        {
            ASSERT_TRUE(corner.face_set().contains(face));

            ASSERT_TRUE(face.corner_set().contains(corner));
            ASSERT_TRUE(face.is_adjacent(corner));
            ASSERT_TRUE(corner.is_adjacent(face));

            corner___face_visits[face.index()]++;
            all___face_visits[face.index()]++;
        }
        for (auto face : cubexx::face_t::all())
        {
            ASSERT_EQ(face.is_adjacent(corner), corner.is_adjacent(face));

            if (face.is_adjacent(corner))
            {
                ASSERT_EQ(1U, all___face_visits[face.index()]);
            } else {
                ASSERT_EQ(0U, all___face_visits[face.index()]);
            }
        }
    }



    for (auto face : cubexx::face_t::all())
    {
        ASSERT_EQ(4U, all___face_visits[face.index()]);
    }

}

TEST_F(CUBEXXCornerTest,edges)
{
    ASSERT_TRUE(false);
}


TEST_F(CUBEXXCornerTest,edge_via_direction)
{
    for (auto corner : cubexx::corner_t::all())
    {
        for (auto direction : cubexx::direction_t::all())
        {
            auto edge = corner.edge(direction);
            auto lhs_corner = corner.adjacent(direction);
            
            
            ASSERT_EQ(direction.axis(), edge.base_axis());
            ASSERT_TRUE(edge.corner_set().contains(corner));
            ASSERT_TRUE(edge.corner_set().contains(lhs_corner));
            
            ASSERT_TRUE(lhs_corner.is_adjacent(corner));
            ASSERT_TRUE(corner.is_adjacent(lhs_corner));
            
            ASSERT_TRUE(corner.edge_set().contains(edge));
            ASSERT_TRUE(lhs_corner.edge_set().contains(edge));
            
            
            
        }
    }
    
}



TEST_F(CUBEXXCornerTest,edge_via_corner)
{
    ASSERT_TRUE(false);
}


TEST_F(CUBEXXCornerTest,xyz)
{
    for (auto corner : cubexx::corner_t::all()){
        auto xyz = corner.xyz();

        ASSERT_EQ(corner.x(), xyz[0]);
        ASSERT_EQ(corner.y(), xyz[1]);
        ASSERT_EQ(corner.z(), xyz[2]);
    }
}

TEST_F(CUBEXXCornerTest,get_adjacent_direction)
{
    std::vector<uint32_t> rhs_counts(8,0);
    std::vector<uint32_t> direction_counts(6,0);
    for (auto lhs : cubexx::corner_t::all())
    {
        uint32_t rhs_count = 0;
        for (auto rhs : cubexx::corner_t::all())
        {
            if (!lhs.is_adjacent(rhs))
                continue;
            
            auto direction = lhs.get_adjacent_direction(rhs);
            
            ASSERT_EQ(direction.opposite(),rhs.get_adjacent_direction(lhs));
            ASSERT_EQ(direction.axis(),rhs.get_adjacent_direction(lhs).axis());
            
            int lhs_xyz[] = {lhs.x(), lhs.y(), lhs.z()};
            int rhs_xyz[] = {rhs.x(), rhs.y(), rhs.z()};
            
            
            ///the coordinate of the corners in the component of the direction's axis should be flipped
            ASSERT_EQ(-lhs_xyz[direction.axis()], rhs_xyz[direction.axis()]);
            
            std::uint_fast8_t axis2 = (direction.axis() + 1) % 3;
            std::uint_fast8_t axis3 = (direction.axis() + 2) % 3;
            
            ///the other coordinates of the corners should be the same
            ASSERT_EQ(lhs_xyz[axis2], rhs_xyz[axis2]);
            ASSERT_EQ(lhs_xyz[axis3], rhs_xyz[axis3]);
            
            rhs_count++;
            rhs_counts.at(rhs.index())++;
            direction_counts.at(direction.index())++;
            
            
            
        }
        
        ///there should be 3 adjacent corners
        ASSERT_EQ(3U, rhs_count);
    }
    
    ///every corner should be covered 3 times
    for (auto corner : cubexx::corner_t::all())
    {
        ASSERT_EQ(3U, rhs_counts.at(corner.index()));
    }
    
    ///every direction should be covered 4 times, between every set of opposite faces
    for (auto direction : cubexx::direction_t::all())
    {
        ASSERT_EQ(4U, direction_counts.at(direction.index()));
    }
    
}











TEST_F(CUBEXXCornerTest,corner_set)
{
    
    ///test in every metric we can think of, if the bitmask and set are equal
    auto bitmask_equal_corner_set = [](const cubexx::corner_set_t& corner_set, uint32_t bitmask)
    {
        for (auto corner : cubexx::corner_t::all())
        {
            if (corner_set.contains(corner) != ((bitmask >> corner.index()) & 1))
                return false;
        }
        
        for (auto corner : corner_set)
        {
            if (!((bitmask >> corner.index()) & 1))
                return false;
        }
        
        
        for (uint32_t bitmask_tmp = bitmask, index = 0; bitmask_tmp != 0; bitmask_tmp >>= 1, ++index)
        {
            assert(index < cubexx::corner_set_t::value_type::SIZE());
            
            bool bit = bitmask_tmp & 1;
            if (corner_set.contains(index) != bit)
                return false;
        }
        
        
        return true;
    };
    
    const cubexx::corner_set_t empty_set;
    
    ///its an empty set, should not contain anything
    for (auto corner : cubexx::corner_t::all())
    {
        ASSERT_FALSE(empty_set.contains(corner));
        ASSERT_FALSE(empty_set.contains(corner.index()));
    }
    
    std::vector<cubexx::corner_set_t> all_corner_sets;
    
    /// every combination of an 8 corner selection
    for (uint32_t combo = 0; combo < 256; ++combo)
    {
        cubexx::corner_set_t corner_set;
        
        ///insert the corners in this combination
        for (auto corner : cubexx::corner_t::all())
        {
            bool corner_bit = (combo >> corner.index()) & 1;
            if ( corner_bit )
                corner_set |= corner;
            
            ASSERT_EQ(corner_set.contains(corner), corner_bit);
            ASSERT_EQ(corner_set.contains(corner.index()), corner_bit);
        }
        ///(double) check the corners in this combination
        for (auto corner : cubexx::corner_t::all())
        {
            bool corner_bit = (combo >> corner.index()) & 1;
            if ( corner_bit )
                corner_set |= corner;
            
            ASSERT_EQ(corner_bit, corner_set.contains(corner));
            ASSERT_EQ(corner_bit, corner_set.contains(corner.index()));
        }
        
        ///check size
        {
            std::size_t size = 0;
            for (auto corner : cubexx::corner_t::all())
            {
                bool corner_bit = (combo >> corner.index()) & 1;
                if ( corner_bit )
                    ++size;
            }
            
            ASSERT_EQ(size, corner_set.size());
        }
        all_corner_sets.push_back(corner_set);
    }
    
    ///check operator== and operator!=
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        auto corner_set0 = all_corner_sets[combo0];
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            auto corner_set1 = all_corner_sets[combo1];
            
            /// combo0 == combo1 =implies> corner_set0 == corner_set1
            ASSERT_TRUE(combo0 != combo1 || corner_set0 == corner_set1);
            ASSERT_TRUE(combo0 != combo1 || !(corner_set0 != corner_set1));
            /// combo0 != combo1 =implies> corner_set0 != corner_set1
            ASSERT_TRUE(combo0 == combo1 || corner_set0 != corner_set1);
            ASSERT_TRUE(combo0 == combo1 || !(corner_set0 == corner_set1));
        }
    }
    
    
    ///check copy ctor
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto corner_set0 = all_corner_sets[combo0];
        
        cubexx::corner_set_t corner_set( corner_set0 );
        
        ASSERT_EQ(corner_set0, corner_set);
        
        ///check equality more comprehensively
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto corner_set1 = all_corner_sets[combo1];
            
            /// combo0 == combo1 =implies> corner_set == corner_set1
            ASSERT_TRUE(combo0 != combo1 || corner_set == corner_set1);
            /// combo0 != combo1 =implies> corner_set != corner_set1
            ASSERT_TRUE(combo0 == combo1 || corner_set != corner_set1);
            
            
            ///make sure its not equal to all the other corner sets.
            for (uint32_t combo2 = 0; combo2 < 256; ++combo2)
            {
                if (combo2 == combo0)
                    continue;
                const auto other_corner_set = all_corner_sets[combo2];
                
                ASSERT_NE(other_corner_set, corner_set);
            }
        }
    }
    
    
    ///check assignment
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        auto corner_set = all_corner_sets[combo0];
        
        
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto corner_set1 = all_corner_sets[combo1];
            
            corner_set = corner_set1;
            
            ASSERT_EQ(corner_set, corner_set1);
            
            for (uint32_t combo2 = 0; combo2 < 256; ++combo2)
            {
                if (combo2 == combo1)
                    continue;
                const auto other_corner_set = all_corner_sets[combo2];
                
                ASSERT_NE(other_corner_set, corner_set);
            }
        }
        
    }
    
        

    
    
    ///check begin(), end()
    for (uint32_t combo = 0; combo < 256; ++combo)
    {
        ///we are gonna blank out each bit as we get that corner
        uint32_t combo_mask = combo;
        auto corner_set = all_corner_sets[combo];
        
        std::size_t count = 0;
        
        cubexx::corner_set_t::const_iterator w;
        ASSERT_NE(w, w);

        w = corner_set.begin();
        cubexx::corner_set_t::const_iterator w_end = corner_set.end();
        
        ASSERT_EQ(w, w);
        ASSERT_EQ(w, corner_set.begin());
        ASSERT_EQ(w_end, corner_set.end());
        ASSERT_EQ(w_end, w_end);
        
        
        ///iterate through the set
        for (; w != w_end; ++w)
        {
            ASSERT_EQ(w,w);
            ASSERT_NE(w,w_end);
            ASSERT_NE(w,corner_set.end());
            cubexx::corner_t corner = *w;
            ///lets count the corners
            count++;
            
            ///make sure this corner is in the combo_mask
            ASSERT_TRUE( (combo_mask >> corner.index()) & 1 );
            ///sanity
            ASSERT_TRUE( corner_set.contains(corner) );
            
            ///blank this bit
            combo_mask ^= 1 << corner.index();
        }
        
        ASSERT_EQ( corner_set.size(), count );
        ///make sure all the bits are blanked out
        ASSERT_EQ( uint32_t(0), combo_mask );
    }
    
    
    ///check range loop
    for (uint32_t combo = 0; combo < 256; ++combo)
    {
        ///we are gonna blank out each bit as we get that corner
        uint32_t combo_mask = combo;
        auto corner_set = all_corner_sets[combo];
        
        std::size_t count = 0;
        
        ///iterate through the set
        for (auto corner : corner_set)
        {
            ///lets count the corners
            count++;
            
            ///make sure this corner is in the combo_mask
            ASSERT_TRUE( (combo_mask >> corner.index()) & 1 );
            ///sanity
            ASSERT_TRUE( corner_set.contains(corner) );
            
            ///blank this bit
            combo_mask ^= 1 << corner.index();
        }
        
        ASSERT_EQ( corner_set.size(), count );
        ///make sure all the bits are blanked out
        ASSERT_EQ( uint32_t(0), combo_mask );
    }
    
    ///check contains(size_t) and contains(corner_t)
    for (uint32_t combo = 0; combo < 256; ++combo)
    {
        const auto corner_set = all_corner_sets[combo];
        
        
        for (auto corner : corner_set)
        {
            ASSERT_TRUE(corner_set.contains(corner));
            ASSERT_TRUE(corner_set.contains(corner.index()));
        }
        
        for (auto corner : cubexx::corner_t::all())
        {
            bool bit = (combo >> corner.index()) & 1;
            ASSERT_EQ(corner_set.contains(corner), bit);
        }
        for (std::size_t index = 0; index < cubexx::corner_t::SIZE(); ++index)
        {
            bool bit = (combo >> index) & 1;
            ASSERT_EQ(corner_set.contains(index), bit);
        }
    }
    
    
    auto bitmask_to_corner_set = [](uint32_t bitmask)
    {
        cubexx::corner_set_t corner_set;
        for (auto corner : cubexx::corner_t::all())
        {
            if (((bitmask >> corner.index()) & 1))
                corner_set |= corner;
        }
        return corner_set;
    };
    
    ///check ::operator<<(corner_set_t | corner_set_t)
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto corner_set0 = all_corner_sets[combo0];
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto corner_set1 = all_corner_sets[combo1];
            
            std::string result0 = tostr(corner_set0 | corner_set1);
            
            const auto corner_set2 = corner_set0 | corner_set1;
            std::string result1 = tostr(corner_set2);
            
            ASSERT_EQ(result0,result1);
        }
    }
    
    ///check corner_set_t::operator|(corner_set_t) and corner_set_t::operator|=(corner_set_t)
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto corner_set0 = all_corner_sets[combo0];
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto corner_set1 = all_corner_sets[combo1];
            
            uint32_t combo_mask = combo0 | combo1;
            
            ///test operator|
            {
                
                ASSERT_TRUE(bitmask_equal_corner_set( corner_set0 | corner_set1, combo0 | combo1))
                    <<   "corner_set0:                " << corner_set0.bits()
                    << "\n corner_set1:               " << corner_set1.bits()
                    << "\n corner_set0 | corner_set1: " << (corner_set0 | corner_set1).bits()
                    << "\n combo0:          " << combo0
                    << "\n combo1:          " << combo1
                    << "\n combo0 | combo1: " << (combo0 | combo1);
                ASSERT_TRUE(bitmask_equal_corner_set( corner_set0 | corner_set1, combo0 | combo1))
                    <<   "corner_set0:                " << corner_set0.bits()
                    << "\n corner_set1:               " << corner_set1.bits()
                    << "\n corner_set0 | corner_set1: " << (corner_set0 | corner_set1);
                ASSERT_EQ(corner_set0 | corner_set1, bitmask_to_corner_set(combo0 | combo1))
                    <<   "corner_set0:                " << corner_set0.bits()
                    << "\n corner_set1:               " << corner_set1.bits()
                    << "\n corner_set0 | corner_set1: " << (corner_set0 | corner_set1);
            }
            ///test operator|=
            {
                auto corner_set = corner_set1;
                corner_set |= corner_set0;
                
                ASSERT_TRUE(bitmask_equal_corner_set(corner_set, combo_mask));
                ASSERT_EQ(corner_set, bitmask_to_corner_set(combo0 | combo1));
            }
        }
        ASSERT_EQ(corner_set0, bitmask_to_corner_set(combo0));
    }
    
    
    
    
    ///check ::operator<<(corner_set_t - corner_set_t)
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto corner_set0 = all_corner_sets[combo0];
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto corner_set1 = all_corner_sets[combo1];
            
            std::string result0 = tostr(corner_set0 - corner_set1);
            
            const auto corner_set2 = corner_set0 - corner_set1;
            std::string result1 = tostr(corner_set2);
            
            ASSERT_EQ(result0,result1);
        }
    }
    
    ///check corner_set_t::operator-(corner_t) and corner_set_t::operator-=(corner_t)
    for (uint32_t base_combo = 0; base_combo < 256; ++base_combo)
    {
        
        for (auto corner : cubexx::corner_t::all())
        {
            const auto base_corner_set = all_corner_sets[base_combo];
            
            auto index = corner.index();
            
            uint32_t expected_result_combo = base_combo & (~(1 << index));
            
            
            ///test operator-
            {
                
                ASSERT_TRUE(bitmask_equal_corner_set( base_corner_set - corner, expected_result_combo))
                    <<   "base_corner_set:                " << base_corner_set.bits()
                    << "\n corner:                        " << corner
                    << "\n corner.index():                " << index
                    << "\n corner.index():                " << std::bitset<8>(index)
                    << "\n base_corner_set - corner:      " << (base_corner_set - corner).bits()
                    << "\n base_combo:                    " << std::bitset<8>(base_combo)
                    << "\n expected_result_combo:  " << std::bitset<8>(expected_result_combo);
                ASSERT_TRUE(bitmask_equal_corner_set( base_corner_set - corner, expected_result_combo))
                    <<   "base_corner_set:                " << base_corner_set.bits()
                    << "\n corner:                        " << corner
                    << "\n corner.index():                " << index
                    << "\n corner.index():                " << std::bitset<8>(index)
                    << "\n base_corner_set - corner:      " << (base_corner_set - corner).bits()
                    << "\n base_combo:                    " << std::bitset<8>(base_combo)
                    << "\n expected_result_combo:  " << std::bitset<8>(expected_result_combo);
                ASSERT_EQ(base_corner_set - corner, bitmask_to_corner_set(expected_result_combo))
                    <<   "base_corner_set:                " << base_corner_set.bits()
                    << "\n corner:                        " << corner
                    << "\n corner.index():                " << index
                    << "\n corner.index():                " << std::bitset<8>(index)
                    << "\n base_corner_set - corner:      " << (base_corner_set - corner).bits()
                    << "\n base_combo:                    " << std::bitset<8>(base_combo)
                    << "\n expected_result_combo:  " << std::bitset<8>(expected_result_combo);
            }
            
            
            ///test operator-=
            {
                auto corner_set = base_corner_set;
                corner_set -= corner;
                
                ASSERT_TRUE(bitmask_equal_corner_set(corner_set, expected_result_combo));
                ASSERT_EQ(corner_set, bitmask_to_corner_set(expected_result_combo));
            }
            
            
            ASSERT_EQ(base_corner_set, all_corner_sets[base_combo]);
            ASSERT_EQ(base_corner_set, bitmask_to_corner_set(base_combo));
        }
    }
    
    ///check corner_set_t::operator-(corner_set_t) and corner_set_t::operator-=(corner_set_t)
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto left_corner_set = all_corner_sets[combo0];
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto right_corner_set = all_corner_sets[combo1];
            uint32_t expected_result_combo = combo0 & (~combo1);
            
            
            
            ASSERT_TRUE( expected_result_combo < 256 );
            
            ///this has no bits of combo1
            ASSERT_EQ(expected_result_combo & combo1, uint32_t(0));
            
            ///expected_result_combo => combo0
            ASSERT_EQ(uint32_t((~expected_result_combo) | combo0), uint32_t(-1));
            
            
            
            
            ///test operator-
            {
                
                ASSERT_TRUE(bitmask_equal_corner_set( left_corner_set - right_corner_set, expected_result_combo))
                    <<   "left_corner_set:                " << left_corner_set.bits()
                    << "\n right_corner_set:               " << right_corner_set.bits()
                    << "\n left_corner_set - right_corner_set: " << (left_corner_set - right_corner_set).bits()
                    << "\n combo0:          " << combo0
                    << "\n combo1:          " << combo1
                    << "\n expected_result_combo: " << (expected_result_combo);
                ASSERT_TRUE(bitmask_equal_corner_set( left_corner_set - right_corner_set, expected_result_combo))
                    <<   "left_corner_set:                " << left_corner_set.bits()
                    << "\n right_corner_set:               " << right_corner_set.bits()
                    << "\n left_corner_set - right_corner_set: " << (left_corner_set - right_corner_set);
                ASSERT_EQ(left_corner_set - right_corner_set, bitmask_to_corner_set(expected_result_combo))
                    <<   "left_corner_set:                " << left_corner_set.bits()
                    << "\n right_corner_set:               " << right_corner_set.bits()
                    << "\n left_corner_set - right_corner_set: " << (left_corner_set - right_corner_set);
            }
            ///test operator-=
            {
                auto result_corner_set = left_corner_set;
                result_corner_set -= right_corner_set;
                
                ASSERT_TRUE(bitmask_equal_corner_set(result_corner_set, expected_result_combo));
                ASSERT_EQ(result_corner_set, bitmask_to_corner_set(expected_result_combo));
            }
            ASSERT_EQ(left_corner_set, bitmask_to_corner_set(combo0));
            ASSERT_EQ(right_corner_set, bitmask_to_corner_set(combo1));
        }
    }
    
    
    ///check corner_set_t::operator&(corner_set_t) and corner_set_t::operator-=(corner_set_t)
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto left_corner_set = all_corner_sets[combo0];
        for (uint32_t combo1 = 0; combo1 < 256; ++combo1)
        {
            const auto right_corner_set = all_corner_sets[combo1];
            uint32_t expected_result_combo = combo0 & combo1;
            
            
            
            ASSERT_TRUE( expected_result_combo < 256 );
            
            
            ///expected_result_combo => combo0
            ASSERT_EQ(uint32_t((~expected_result_combo) | combo0), uint32_t(-1));
            ///expected_result_combo => combo1
            ASSERT_EQ(uint32_t((~expected_result_combo) | combo0), uint32_t(-1));
            
            
            
            
            ///test operator&
            {
                
                ASSERT_TRUE(bitmask_equal_corner_set( left_corner_set & right_corner_set, expected_result_combo))
                    <<   "left_corner_set:                " << left_corner_set.bits()
                    << "\n right_corner_set:               " << right_corner_set.bits()
                    << "\n left_corner_set & right_corner_set: " << (left_corner_set & right_corner_set).bits()
                    << "\n combo0:          " << combo0
                    << "\n combo1:          " << combo1
                    << "\n expected_result_combo: " << (expected_result_combo);
                ASSERT_TRUE(bitmask_equal_corner_set( left_corner_set & right_corner_set, expected_result_combo))
                    <<   "left_corner_set:                " << left_corner_set.bits()
                    << "\n right_corner_set:               " << right_corner_set.bits()
                    << "\n left_corner_set & right_corner_set: " << (left_corner_set & right_corner_set);
                ASSERT_EQ(left_corner_set & right_corner_set, bitmask_to_corner_set(expected_result_combo))
                    <<   "left_corner_set:                " << left_corner_set.bits()
                    << "\n right_corner_set:               " << right_corner_set.bits()
                    << "\n left_corner_set & right_corner_set: " << (left_corner_set & right_corner_set);
            }
            ///test operator-=
            {
                auto result_corner_set = left_corner_set;
                result_corner_set &= right_corner_set;
                
                ASSERT_TRUE(bitmask_equal_corner_set(result_corner_set, expected_result_combo));
                ASSERT_EQ(result_corner_set, bitmask_to_corner_set(expected_result_combo));
            }
            ASSERT_EQ(left_corner_set, bitmask_to_corner_set(combo0));
            ASSERT_EQ(right_corner_set, bitmask_to_corner_set(combo1));
        }
    }
    
    
    
    
    
    
    ///check ::operator<<
    /*
    for (uint32_t combo0 = 0; combo0 < 256; ++combo0)
    {
        const auto corner_set = all_corner_sets[combo0];
        
        std::string oparen_re = "\\(";
        std::string cparen_re = "\\)";
        std::string olist_re = "\\[";
        std::string clist_re = "\\]";
        std::string space_re = "\\s";
        std::string plus_re = "\\+";
        std::string minus_re = "\\-";
        std::string corner_digit_re = "1";
        std::string corner_num_re = "(({plus}|{minus})?{digit})";
        std::string corner_re = "({oparen}   {num},{space}*   {num}{space}*   ,{num}   {cparen})";
        std::string corner_list_re = "{olist}   ({corner}   (,{space}*   {corner})*)?   {clist}";
        
        corner_num_re = fmt::format(corner_num_re
                                    , fmt::arg("plus", plus_re)
                                    , fmt::arg("minus", minus_re)
                                    , fmt::arg("digit", corner_digit_re));
        
        corner_re = fmt::format(corner_re
                                , fmt::arg("oparen", oparen_re)
                                , fmt::arg("cparen", cparen_re)
                                , fmt::arg("num", corner_num_re)
                                , fmt::arg("space", space_re)
                                );
        corner_list_re = fmt::format(corner_list_re
                                , fmt::arg("olist", olist_re)
                                , fmt::arg("clist", clist_re)
                                , fmt::arg("space", space_re)
                                , fmt::arg("corner", corner_re)
                                );
        
        std::string final_re_str = corner_list_re;
        
        ///remove whitespace from the regex
        std::string::iterator end_pos = std::remove(final_re_str.begin(), final_re_str.end(), ' ');
        final_re_str.erase(end_pos, final_re_str.end());


        std::regex re(final_re_str);
        
        auto formatted = cubexx::tostr(corner_set);
        
        
        
        ASSERT_TRUE(std::regex_match(formatted,re))
            << "re: " << corner_list_re
            << ", re: " << final_re_str
            << ", formatted: " << formatted;
        
    }
    */
    
}





