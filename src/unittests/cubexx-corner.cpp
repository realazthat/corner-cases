


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




TEST_F(CUBEXXCornerTest,index)
{


    ///test corner_t::index() and corner_t::index(index)
    uint32_t index = 0;
    for (auto corner : cubexx::corner_t::all())
    {
        EXPECT_EQ(index, corner.index());
        
        EXPECT_EQ(corner, cubexx::corner_t::index(index));
        
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
        
        EXPECT_EQ(bitmask, uint32_t(1 << 8) - 1);
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

    ///test corner_t::opposite_corner()
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
        uint8_t mask = 0;
        
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
        
        EXPECT_EQ(mask, uint32_t(1 << 8) - 1);

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
            assert(index < cubexx::corner_set_t::value_type::SIZE);
            
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
        for (std::size_t index = 0; index < cubexx::corner_t::SIZE; ++index)
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
    
    ///check corner_set_t::operator|(corner_set_t) and corner_set_t::operator!=(corner_set_t)
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




