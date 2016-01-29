


#include "cubelib/cubelib.h"
#include "cubelib/formatters.hpp"

#include "gtest/gtest.h"



#include <vector>
#include <tuple>




struct CubelibDirectionTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};





TEST_F(CubelibDirectionTest,direction_indices)
{
    ///test direction indices
    {
        ///mark each direction's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (auto direction : all_directions)
        {
            bitmask |= (1 << get_direction_index(direction));
        }

        EXPECT_EQ(bitmask, uint32_t(1 << 6) - 1);



        ///test get_direction_by_index()
        uint32_t index = 0;
        for (auto direction : all_directions)
        {
            EXPECT_EQ(index, get_direction_index(direction));

            EXPECT_TRUE(is_direction_equal(direction, get_direction_by_index(index)));
            EXPECT_FALSE(is_direction_null(direction));
            EXPECT_TRUE(is_direction_valid(direction));

            ++index;
        }

    }
}

TEST_F(CubelibDirectionTest,null_direction)
{

    
    ///test null direction
    {
        EXPECT_TRUE(is_direction_valid(null_direction));
        EXPECT_TRUE(is_direction_null(null_direction));
        
        for (auto direction : all_directions)
        {
            EXPECT_FALSE(is_direction_null(direction));
            EXPECT_TRUE(is_direction_valid(direction));
        }
    }

}

TEST_F(CubelibDirectionTest,get_direction_by_int3)
{

    
    ///test get_direction_by_int3()
    {
        ///make sure every direction is tested, use a bitmask to mark each direction
        uint32_t bitmask = 0;
        for (auto direction : all_directions)
        {
            bitmask |= (1 << get_direction_index(direction));
        }
        
        for (int dim = 0; dim < 3; ++dim)
        for (int pos = -1; pos < 2; pos += 2)
        {
            int xyz[] = {0,0,0};
            xyz[dim] = pos;
            
            int x = xyz[0], y = xyz[1], z = xyz[2];
            
            direction_t direction = get_direction_by_int3(x,y,z);
            
            EXPECT_NE( (bitmask & ( 1 << get_direction_index(direction))), uint32_t(0) );
            bitmask ^= ( 1 << get_direction_index(direction));
            
            EXPECT_EQ(get_direction_x(direction), x);
            EXPECT_EQ(get_direction_y(direction), y);
            EXPECT_EQ(get_direction_z(direction), z);
            
            for (int i = 0; i < 3; ++i)
                EXPECT_EQ(get_direction_i(direction, i), xyz[i]);
            
        }
        
        EXPECT_EQ(bitmask, uint32_t(0));
    }
    
}






TEST_F(CubelibDirectionTest,direction_formatters)
{

    
    std::vector< std::tuple<direction_t, std::string> > tests;
    
    tests.push_back( std::make_tuple(get_direction_by_int3(1,0,0), "(1,0,0)") );
    tests.push_back( std::make_tuple(get_direction_by_int3(-1,0,0), "(-1,0,0)") );
    tests.push_back( std::make_tuple(get_direction_by_int3(0,1,0), "(0,1,0)") );
    tests.push_back( std::make_tuple(get_direction_by_int3(0,-1,0), "(0,-1,0)") );
    tests.push_back( std::make_tuple(get_direction_by_int3(0,0,1), "(0,0,1)") );
    tests.push_back( std::make_tuple(get_direction_by_int3(0,0,-1), "(0,0,-1)") );
    
    
    for (auto test : tests)
    {
        auto direction = std::get<0>(test);
        
        auto expected = std::get<1>(test);
        
        auto actual = tostr(direction);
        
        EXPECT_EQ(expected, actual);
    }
    
    
    
}







