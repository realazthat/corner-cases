


#include "cubelib/cubelib.h"
#include "cubelib/formatters.hpp"

#include "gtest/gtest.h"



#include <vector>
#include <tuple>





struct CubelibCornerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};





TEST_F(CubelibCornerTest,indices)
{

    ///test corner indices
    {
        ///mark each corner's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (cubelib_corner_t corner : all_corners)
        {
            bitmask |= (1 << get_corner_index(corner));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << 8) - 1);
        
        
        
        ///test get_corner_by_index()
        uint32_t index = 0;
        for (cubelib_corner_t corner : all_corners)
        {
            EXPECT_EQ(index, get_corner_index(corner));
            
            EXPECT_TRUE(is_corner_equal(corner, get_corner_by_index(index)));
            EXPECT_FALSE(is_corner_null(corner));
            EXPECT_TRUE(is_corner_valid(corner));
            
            ++index;
        }
        
    }
}


TEST_F(CubelibCornerTest,null)
{

    
    ///test null corner
    {
        EXPECT_TRUE(is_corner_valid(null_corner));
        EXPECT_TRUE(is_corner_null(null_corner));
        EXPECT_TRUE(is_corner_equal(null_corner, null_corner));
    }
    
    ///make sure all the regular corners are not null_corner
    for (cubelib_corner_t corner : all_corners)
    {
        EXPECT_FALSE(is_corner_null(corner));
        EXPECT_TRUE(is_corner_valid(corner));
        EXPECT_FALSE(is_corner_equal(corner, null_corner));
    }


}



TEST_F(CubelibCornerTest,get_corner_by_int3_unitxyz)
{

    ///test get_corner_by_int3() with unit x,y,z
    {
        ///make sure every corner is tested, use a bitmask to mark each corner
        uint32_t bitmask = 0;
        for (cubelib_corner_t corner : all_corners)
        {
            bitmask |= (1 << get_corner_index(corner));
        }

        for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y)
        for (int z = 0; z < 2; ++z)
        {
            cubelib_corner_t corner = get_corner_by_int3(x,y,z);

            auto index = get_corner_index(corner);
            EXPECT_NE( (bitmask & (1 << index)), uint32_t(0) );
            bitmask ^= ( 1 << get_corner_index(corner));

            EXPECT_EQ(get_corner_unitx(corner), x);
            EXPECT_EQ(get_corner_unity(corner), y);
            EXPECT_EQ(get_corner_unitz(corner), z);

            int xyz[] = {x,y,z};

            for (int i = 0; i < 3; ++i)
                EXPECT_EQ(get_corner_uniti(corner,i), xyz[i]);
            
        }

        EXPECT_EQ(bitmask, uint32_t(0));
    }

}


TEST_F(CubelibCornerTest,get_corner_by_int3)
{

    ///test get_corner_by_int3()
    {
        ///make sure every corner is tested, use a bitmask to mark each corner
        uint32_t bitmask = 0;
        for (cubelib_corner_t corner : all_corners)
        {
            bitmask |= (1 << get_corner_index(corner));
        }

        for (int x = -1; x < 2; x += 2)
        for (int y = -1; y < 2; y += 2)
        for (int z = -1; z < 2; z += 2)
        {
            cubelib_corner_t corner = get_corner_by_int3(x,y,z);

            EXPECT_NE( (bitmask & ( 1 << get_corner_index(corner))), uint32_t(0) );
            bitmask ^= ( 1 << get_corner_index(corner));

            EXPECT_EQ(get_corner_x(corner), x);
            EXPECT_EQ(get_corner_y(corner), y);
            EXPECT_EQ(get_corner_z(corner), z);
            
            int xyz[] = {x,y,z};

            for (int i = 0; i < 3; ++i)
                EXPECT_EQ(get_corner_i(corner,i), xyz[i]);
        }

        EXPECT_EQ(bitmask, uint32_t(0));
    }

}


TEST_F(CubelibCornerTest,get_opposite_corner)
{
    ///test get_opposite_corner
    {
        for (cubelib_corner_t corner0 : all_corners)
        {
            auto corner1 = get_opposite_corner(corner0);

            EXPECT_EQ(get_corner_x(corner1), -get_corner_x(corner0));
            EXPECT_EQ(get_corner_y(corner1), -get_corner_y(corner0));
            EXPECT_EQ(get_corner_z(corner1), -get_corner_z(corner0));

        }
    }


}



TEST_F(CubelibCornerTest,corner_move)
{

    for (cubelib_corner_t corner0 : all_corners)
    {
        for (direction_t direction : all_directions)
        {
            cubelib_corner_t corner1 = corner_move(corner0, direction);
            
            ASSERT_FALSE(is_corner_equal(corner0, corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            int xyz0[] = {get_corner_x(corner0), get_corner_y(corner0), get_corner_z(corner0)};
            int dir[] = {get_direction_x(direction), get_direction_y(direction), get_direction_z(direction)};
            int xyz1_expected[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz1_expected[i] = xyz0[i] + dir[i]*2;
            
            bool expected_out_of_bounds = false;
            
            for (int i = 0; i < 3; ++i)
                if (std::abs(xyz1_expected[i]) != 1)
                    expected_out_of_bounds = true;
            
            
            ///corner1 is null <=> it was out of bound
            ASSERT_EQ(is_corner_null(corner1), expected_out_of_bounds)
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            
            if (is_corner_null(corner1))
                continue;
            
            int xyz1_actual[] = {get_corner_x(corner1), get_corner_y(corner1), get_corner_z(corner1)};
            
            for (int i = 0; i < 3; ++i)
                ASSERT_EQ(xyz1_actual[i] - xyz0[i], 2*dir[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            for (int i = 0; i < 3; ++i)
                ASSERT_EQ(xyz1_actual[i], xyz1_expected[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
        }
    }
    
    
    
    
    for (cubelib_corner_t corner0 : all_corners)
    {
        for (direction_t direction : all_directions)
        {
            auto corner1 = corner_move(corner0,direction);
            ASSERT_FALSE(is_corner_equal(corner0, corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;

            auto x1 = get_corner_x(corner0) + 2*get_direction_x(direction);
            auto y1 = get_corner_y(corner0) + 2*get_direction_y(direction);
            auto z1 = get_corner_z(corner0) + 2*get_direction_z(direction);

            if (x1 > 1 || x1 < -1 || y1 > 1 || y1 < -1 || z1 > 1 || z1 < -1)
            {
                ASSERT_TRUE(is_corner_null(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            } else {
                ASSERT_EQ(x1, get_corner_x(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(y1, get_corner_y(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(z1, get_corner_z(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            }

        }
    }
}



TEST_F(CubelibCornerTest,corner_push)
{

    for (cubelib_corner_t corner0 : all_corners)
    {
        for (direction_t direction : all_directions)
        {
            cubelib_corner_t corner1 = corner_push(corner0, direction);
            
            ASSERT_FALSE(is_corner_null(corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            int xyz0[] = {get_corner_x(corner0), get_corner_y(corner0), get_corner_z(corner0)};
            int dir[] = {get_direction_x(direction), get_direction_y(direction), get_direction_z(direction)};
            int xyz1_expected[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz1_expected[i] = xyz0[i] + dir[i]*2;
            
            bool expected_out_of_bounds = false;
            
            for (int i = 0; i < 3; ++i)
                if (std::abs(xyz1_expected[i]) != 1)
                    expected_out_of_bounds = true;
            
            
            ///(corner1 == corner0) <=> it was out of bound
            ASSERT_EQ(is_corner_equal(corner0,corner1), expected_out_of_bounds)
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            
            if (expected_out_of_bounds)
                continue;
            
            int xyz1_actual[] = {get_corner_x(corner1), get_corner_y(corner1), get_corner_z(corner1)};
            
            ///check if the new corner is what we expect
            {
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(xyz1_actual[i] - xyz0[i], 2*dir[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                for (int i = 0; i < 3; ++i)
                    ASSERT_EQ(xyz1_actual[i], xyz1_expected[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            }
        }
    }
    
    
    
    ///same thing, written in a different way
    for (cubelib_corner_t corner0 : all_corners)
    {
        for (direction_t direction : all_directions)
        {
            auto corner1 = corner_push(corner0,direction);
            ASSERT_FALSE(is_corner_null(corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;

            auto x1 = get_corner_x(corner0) + 2*get_direction_x(direction);
            auto y1 = get_corner_y(corner0) + 2*get_direction_y(direction);
            auto z1 = get_corner_z(corner0) + 2*get_direction_z(direction);

            if (x1 > 1 || x1 < -1 || y1 > 1 || y1 < -1 || z1 > 1 || z1 < -1)
            {
                ASSERT_TRUE(!is_corner_null(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_TRUE(is_corner_equal(corner0,corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            } else {
                ASSERT_EQ(x1, get_corner_x(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(y1, get_corner_y(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(z1, get_corner_z(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            }

        }
    }
}

TEST_F(CubelibCornerTest,is_corner_adjacent_corner)
{

    for (cubelib_corner_t corner0 : all_corners)
    {
        for (direction_t direction : all_directions)
        {
            cubelib_corner_t corner1 = get_adjacent_corner(corner0, direction);
            
            ASSERT_TRUE(!is_corner_null(corner1));
            ASSERT_TRUE(is_corner_adjacent_corner(corner0, corner1));
            
            
            
        }
    }
    
    ///@TODO: test for false positives
}




TEST_F(CubelibCornerTest,calc_cnr_adj_cnr)
{

    
    
    ///test calc_cnr_adj_cnr()
    {
        for (cubelib_corner_t corner0 : all_corners)
        {
            int xyz0[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz0[i] = get_corner_i(corner0, i);
            EXPECT_EQ(get_corner_x(corner0), xyz0[0]);
            EXPECT_EQ(get_corner_y(corner0), xyz0[1]);
            EXPECT_EQ(get_corner_z(corner0), xyz0[2]);
            
            for (int dim = 0; dim < 3; ++dim)
            {
                auto corner1 = calc_cnr_adj_cnr(corner0,dim);
                
                //std::cout << "corner0: " << corner0 << ", corner1: " << corner1
                //<< ", dim: " << dim
                //<< std::endl;
                int xyz1[] = {0,0,0};
                for (int i = 0; i < 3; ++i)
                    xyz1[i] = get_corner_i(corner1, i);
                EXPECT_EQ(get_corner_x(corner1), xyz1[0]);
                EXPECT_EQ(get_corner_y(corner1), xyz1[1]);
                EXPECT_EQ(get_corner_z(corner1), xyz1[2]);
                
                
                for (int i = 0; i < 3; ++i)
                {
                    /*
                        std::cout << "corner0: " << corner0 << ", corner1: " << corner1
                        << ", dim: " << dim << ", i: " << i
                        << ", xyz0[i]: " << xyz0[i]
                        << ", xyz1[i]: " << xyz1[i]
                        << ", b(corner0): " << std::bitset<8>(corner0.value)
                        << ", b(corner1): " << std::bitset<8>(corner1.value)
                        << std::endl;
                     */
                    if (dim == i)
                        EXPECT_NE(xyz1[i], xyz0[i]);
                    else
                        EXPECT_EQ(xyz1[i], xyz0[i]);
                }
            }
        }
    }
}


TEST_F(CubelibCornerTest,cnr_adj_cnrs)
{

    
    
    ///test calc_cnr_adj_cnr()
    {
        for (cubelib_corner_t corner0 : all_corners)
        {
            int xyz0[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz0[i] = get_corner_i(corner0, i);
            EXPECT_EQ(get_corner_x(corner0), xyz0[0]);
            EXPECT_EQ(get_corner_y(corner0), xyz0[1]);
            EXPECT_EQ(get_corner_z(corner0), xyz0[2]);
            
            for (int dim = 0; dim < 3; ++dim)
            {
                auto corner1 = calc_cnr_adj_cnr(corner0,dim);
                
                //std::cout << "corner0: " << corner0 << ", corner1: " << corner1
                //<< ", dim: " << dim
                //<< std::endl;
                int xyz1[] = {0,0,0};
                for (int i = 0; i < 3; ++i)
                    xyz1[i] = get_corner_i(corner1, i);
                EXPECT_EQ(get_corner_x(corner1), xyz1[0]);
                EXPECT_EQ(get_corner_y(corner1), xyz1[1]);
                EXPECT_EQ(get_corner_z(corner1), xyz1[2]);
                
                
                for (int i = 0; i < 3; ++i)
                {
                    /*
                        std::cout << "corner0: " << corner0 << ", corner1: " << corner1
                        << ", dim: " << dim << ", i: " << i
                        << ", xyz0[i]: " << xyz0[i]
                        << ", xyz1[i]: " << xyz1[i]
                        << ", b(corner0): " << std::bitset<8>(corner0.value)
                        << ", b(corner1): " << std::bitset<8>(corner1.value)
                        << std::endl;
                     */
                    if (dim == i)
                        EXPECT_NE(xyz1[i], xyz0[i]);
                    else
                        EXPECT_EQ(xyz1[i], xyz0[i]);
                }
            }
        }
    }
}



TEST_F(CubelibCornerTest,corner_formatters)
{

    
    
    std::vector< std::tuple<cubelib_corner_t, std::string> > tests;
    
    tests.push_back( std::make_tuple(get_corner_by_int3(0,0,1), "(-1,-1,1)") );
    tests.push_back( std::make_tuple(get_corner_by_int3(0,0,-1), "(-1,-1,-1)") );
    tests.push_back( std::make_tuple(get_corner_by_int3(0,1,-1), "(-1,1,-1)") );
    
    
    for (auto test : tests)
    {
        auto corner = std::get<0>(test);
        
        auto expected = std::get<1>(test);
        
        auto actual = tostr(corner);
        
        EXPECT_EQ(expected, actual);
    }
    
}