


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
        for (auto direction : cubelib_all_directions)
        {
            bitmask |= (1 << cubelib_get_direction_index(direction));
        }

        EXPECT_EQ(bitmask, uint32_t(1 << 6) - 1);



        ///test cubelib_get_direction_by_index()
        uint32_t index = 0;
        for (auto direction : cubelib_all_directions)
        {
            EXPECT_EQ(index, cubelib_get_direction_index(direction));

            EXPECT_TRUE(cubelib_is_direction_equal(direction, cubelib_get_direction_by_index(index)));
            EXPECT_FALSE(cubelib_is_direction_null(direction));
            EXPECT_TRUE(cubelib_is_direction_valid(direction));

            ++index;
        }

    }
}

TEST_F(CubelibDirectionTest,null)
{

    
    ///test null direction
    {
        EXPECT_TRUE(cubelib_is_direction_valid(cubelib_null_direction));
        EXPECT_TRUE(cubelib_is_direction_null(cubelib_null_direction));
        EXPECT_TRUE(cubelib_is_direction_equal(cubelib_null_direction, cubelib_null_direction));
    }
    
    ///make sure all the regular corners are not null_corner
    for (auto direction : cubelib_all_directions)
    {
        EXPECT_FALSE(cubelib_is_direction_null(direction));
        EXPECT_TRUE(cubelib_is_direction_valid(direction));
        EXPECT_FALSE(cubelib_is_direction_equal(direction, cubelib_null_direction));
    }

}

TEST_F(CubelibDirectionTest,get_direction_by_int3)
{

    
    ///test cubelib_get_direction_by_int3()
    {
        ///make sure every direction is tested, use a bitmask to mark each direction
        uint32_t bitmask = 0;
        for (auto direction : cubelib_all_directions)
        {
            bitmask |= (1 << cubelib_get_direction_index(direction));
        }
        
        for (int dim = 0; dim < 3; ++dim)
        for (int pos = -1; pos < 2; pos += 2)
        {
            int xyz[] = {0,0,0};
            xyz[dim] = pos;
            
            int x = xyz[0], y = xyz[1], z = xyz[2];
            
            cubelib_direction_t direction = cubelib_get_direction_by_int3(x,y,z);
            
            EXPECT_NE( (bitmask & ( 1 << cubelib_get_direction_index(direction))), uint32_t(0) );
            bitmask ^= ( 1 << cubelib_get_direction_index(direction));
            
            EXPECT_EQ(cubelib_get_direction_x(direction), x);
            EXPECT_EQ(cubelib_get_direction_y(direction), y);
            EXPECT_EQ(cubelib_get_direction_z(direction), z);
            
            for (int i = 0; i < 3; ++i)
                EXPECT_EQ(cubelib_get_direction_i(direction, i), xyz[i]);
            
        }
        
        EXPECT_EQ(bitmask, uint32_t(0));
    }
    
}


TEST_F(CubelibDirectionTest,get_opposite_direction)
{

    ///make sure every opposite direction is tested, use a bitmask to mark each direction
    uint32_t bitmask = 0;
    for (auto direction : cubelib_all_directions)
    {
        bitmask |= (1 << cubelib_get_direction_index(direction));
    }

    ///test cubelib_get_opposite_direction()
    {
        for (auto direction : cubelib_all_directions)
        {
            auto opposite_direction = cubelib_get_opposite_direction(direction);
            
            ///Make sure running cubelib_get_opposite_direction() again gives the original result
            EXPECT_TRUE(cubelib_is_direction_equal(direction, cubelib_get_opposite_direction(opposite_direction)));
            
            
            EXPECT_EQ(cubelib_get_direction_x(direction), -cubelib_get_direction_x(opposite_direction));
            EXPECT_EQ(cubelib_get_direction_y(direction), -cubelib_get_direction_y(opposite_direction));
            EXPECT_EQ(cubelib_get_direction_z(direction), -cubelib_get_direction_z(opposite_direction));
            
            
            
            
            EXPECT_NE( (bitmask & ( 1 << cubelib_get_direction_index(opposite_direction))), uint32_t(0) );
            bitmask ^= ( 1 << cubelib_get_direction_index(opposite_direction));

        }
      
    }
    EXPECT_EQ(bitmask, uint32_t(0));

}

TEST_F(CubelibDirectionTest,get_dir_adj_cnr)
{
    for (auto direction : cubelib_all_directions)
    {
        for (int i = 0; i < 4; ++i)
        {
          
        }
    }
  
}



TEST_F(CubelibDirectionTest,direction_formatters)
{

    
    std::vector< std::tuple<cubelib_direction_t, std::string> > tests;
    
    tests.push_back( std::make_tuple(cubelib_get_direction_by_int3(1,0,0), "(1,0,0)") );
    tests.push_back( std::make_tuple(cubelib_get_direction_by_int3(-1,0,0), "(-1,0,0)") );
    tests.push_back( std::make_tuple(cubelib_get_direction_by_int3(0,1,0), "(0,1,0)") );
    tests.push_back( std::make_tuple(cubelib_get_direction_by_int3(0,-1,0), "(0,-1,0)") );
    tests.push_back( std::make_tuple(cubelib_get_direction_by_int3(0,0,1), "(0,0,1)") );
    tests.push_back( std::make_tuple(cubelib_get_direction_by_int3(0,0,-1), "(0,0,-1)") );
    
    
    for (auto test : tests)
    {
        auto direction = std::get<0>(test);
        
        auto expected = std::get<1>(test);
        
        auto actual = tostr(direction);
        
        EXPECT_EQ(expected, actual);
    }
    
    
    
}







