#include "cubelib/cubelib.h"
#include "gtest/gtest.h"
#include "cubelib/formatters.hpp"

#include <vector>
#include <fstream>
#include <tuple>



int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}




struct CornerTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};





TEST_F(CornerTest,corner_indices)
{

    ///test corner indices
    {
        ///mark each corner's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (corner_t corner : all_corners)
        {
            bitmask |= (1 << get_corner_index(corner));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << 8) - 1);
        
        
        
        ///test get_corner_by_index()
        uint32_t index = 0;
        for (corner_t corner : all_corners)
        {
            EXPECT_EQ(index, get_corner_index(corner));
            
            EXPECT_TRUE(is_corner_equal(corner, get_corner_by_index(index)));
            EXPECT_FALSE(is_null_corner(corner));
            EXPECT_TRUE(is_valid_corner(corner));
            
            ++index;
        }
        
    }
    


}


TEST_F(CornerTest,corner_null)
{

    
    ///test null corner
    {
        EXPECT_TRUE(is_valid_corner(null_corner));
        EXPECT_TRUE(is_null_corner(null_corner));
    }
    
    for (corner_t corner : all_corners)
    {
        EXPECT_FALSE(is_null_corner(corner));
        EXPECT_TRUE(is_valid_corner(corner));
    }


}



TEST_F(CornerTest,get_corner_by_int3_unitx)
{

    ///test get_corner_by_int3() with unit x,y,z
    {
        ///make sure every corner is tested, use a bitmask to mark each corner
        uint32_t bitmask = 0;
        for (corner_t corner : all_corners)
        {
            bitmask |= (1 << get_corner_index(corner));
        }

        for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y)
        for (int z = 0; z < 2; ++z)
        {
            corner_t corner = get_corner_by_int3(x,y,z);

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


TEST_F(CornerTest,get_corner_by_int3)
{

    ///test get_corner_by_int3()
    {
        ///make sure every corner is tested, use a bitmask to mark each corner
        uint32_t bitmask = 0;
        for (corner_t corner : all_corners)
        {
            bitmask |= (1 << get_corner_index(corner));
        }

        for (int x = -1; x < 2; x += 2)
        for (int y = -1; y < 2; y += 2)
        for (int z = -1; z < 2; z += 2)
        {
            corner_t corner = get_corner_by_int3(x,y,z);

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


TEST_F(CornerTest,opposite_corner)
{
    ///test opposite_corner
    {
        for (corner_t corner0 : all_corners)
        {
            auto corner1 = opposite_corner(corner0);

            EXPECT_EQ(get_corner_x(corner1), -get_corner_x(corner0));
            EXPECT_EQ(get_corner_y(corner1), -get_corner_y(corner0));
            EXPECT_EQ(get_corner_z(corner1), -get_corner_z(corner0));

        }
    }


}


TEST_F(CornerTest,direction_indices)
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
            EXPECT_FALSE(is_null_direction(direction));
            EXPECT_TRUE(is_valid_direction(direction));

            ++index;
        }

    }
}

TEST_F(CornerTest,null_direction)
{

    
    ///test null direction
    {
        EXPECT_TRUE(is_valid_direction(null_direction));
        EXPECT_TRUE(is_null_direction(null_direction));
        
        for (auto direction : all_directions)
        {
            EXPECT_FALSE(is_null_direction(direction));
            EXPECT_TRUE(is_valid_direction(direction));
        }
    }

}

TEST_F(CornerTest,get_direction_by_int3)
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



TEST_F(CornerTest,calc_cnr_adj_cnr)
{

    
    
    ///test calc_cnr_adj_cnr()
    {
        for (corner_t corner0 : all_corners)
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


TEST_F(CornerTest,cnr_adj_cnrs)
{

    
    
    ///test calc_cnr_adj_cnr()
    {
        for (corner_t corner0 : all_corners)
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




TEST_F(CornerTest,move_corner)
{

    
    
    ///test calc_cnr_adj_cnr()
    {
        for (corner_t corner0 : all_corners)
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



TEST_F(CornerTest,corner_formatters)
{

    
    
    std::vector< std::tuple<corner_t, std::string> > tests;
    
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

TEST_F(CornerTest,direction_formatters)
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




