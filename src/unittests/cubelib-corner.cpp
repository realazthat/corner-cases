


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
        for (cubelib_corner_t corner : cubelib_all_corners)
        {
            bitmask |= (1 << cubelib_get_corner_index(corner));
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << 8) - 1);
        
        
        
        ///test cubelib_get_corner_by_index()
        uint32_t index = 0;
        for (cubelib_corner_t corner : cubelib_all_corners)
        {
            EXPECT_EQ(index, cubelib_get_corner_index(corner));
            
            EXPECT_TRUE(cubelib_is_corner_equal(corner, cubelib_get_corner_by_index(index)));
            EXPECT_FALSE(cubelib_is_corner_null(corner));
            EXPECT_TRUE(cubelib_is_corner_valid(corner));
            
            ++index;
        }
        
    }
}


TEST_F(CubelibCornerTest,null)
{

    
    ///test null corner
    {
        EXPECT_TRUE(cubelib_is_corner_valid(cubelib_null_corner));
        EXPECT_TRUE(cubelib_is_corner_null(cubelib_null_corner));
        EXPECT_TRUE(cubelib_is_corner_equal(cubelib_null_corner, cubelib_null_corner));
    }
    
    ///make sure all the regular corners are not cubelib_null_corner
    for (cubelib_corner_t corner : cubelib_all_corners)
    {
        EXPECT_FALSE(cubelib_is_corner_null(corner));
        EXPECT_TRUE(cubelib_is_corner_valid(corner));
        EXPECT_FALSE(cubelib_is_corner_equal(corner, cubelib_null_corner));
    }


}



TEST_F(CubelibCornerTest,get_corner_by_int3_unitxyz)
{

    ///test cubelib_get_corner_by_int3() with unit x,y,z
    {
        ///make sure every corner is tested, use a bitmask to mark each corner
        uint32_t bitmask = 0;
        for (cubelib_corner_t corner : cubelib_all_corners)
        {
            bitmask |= (1 << cubelib_get_corner_index(corner));
        }

        for (int x = 0; x < 2; ++x)
        for (int y = 0; y < 2; ++y)
        for (int z = 0; z < 2; ++z)
        {
            cubelib_corner_t corner = cubelib_get_corner_by_int3(x,y,z);

            auto index = cubelib_get_corner_index(corner);
            EXPECT_NE( (bitmask & (1 << index)), uint32_t(0) );
            bitmask ^= ( 1 << cubelib_get_corner_index(corner));

            EXPECT_EQ(cubelib_get_corner_ux(corner), x);
            EXPECT_EQ(cubelib_get_corner_uy(corner), y);
            EXPECT_EQ(cubelib_get_corner_uz(corner), z);

            int xyz[] = {x,y,z};

            for (int i = 0; i < 3; ++i)
                EXPECT_EQ(cubelib_get_corner_ui(corner,i), xyz[i]);
            
        }

        EXPECT_EQ(bitmask, uint32_t(0));
    }

}


TEST_F(CubelibCornerTest,get_corner_by_int3)
{

    ///test cubelib_get_corner_by_int3()
    {
        ///make sure every corner is tested, use a bitmask to mark each corner
        uint32_t bitmask = 0;
        for (cubelib_corner_t corner : cubelib_all_corners)
        {
            bitmask |= (1 << cubelib_get_corner_index(corner));
        }

        for (int x = -1; x < 2; x += 2)
        for (int y = -1; y < 2; y += 2)
        for (int z = -1; z < 2; z += 2)
        {
            cubelib_corner_t corner = cubelib_get_corner_by_int3(x,y,z);

            EXPECT_NE( (bitmask & ( 1 << cubelib_get_corner_index(corner))), uint32_t(0) );
            bitmask ^= ( 1 << cubelib_get_corner_index(corner));

            EXPECT_EQ(cubelib_get_corner_x(corner), x);
            EXPECT_EQ(cubelib_get_corner_y(corner), y);
            EXPECT_EQ(cubelib_get_corner_z(corner), z);
            
            int xyz[] = {x,y,z};

            for (int i = 0; i < 3; ++i)
                EXPECT_EQ(cubelib_get_corner_i(corner,i), xyz[i]);
        }

        EXPECT_EQ(bitmask, uint32_t(0));
    }

}


TEST_F(CubelibCornerTest,cubelib_get_opposite_corner)
{
    ///test cubelib_get_opposite_corner
    {
        for (cubelib_corner_t corner0 : cubelib_all_corners)
        {
            auto corner1 = cubelib_get_opposite_corner(corner0);

            EXPECT_EQ(cubelib_get_corner_x(corner1), -cubelib_get_corner_x(corner0));
            EXPECT_EQ(cubelib_get_corner_y(corner1), -cubelib_get_corner_y(corner0));
            EXPECT_EQ(cubelib_get_corner_z(corner1), -cubelib_get_corner_z(corner0));

        }
    }


}



TEST_F(CubelibCornerTest,corner_move)
{

    for (cubelib_corner_t corner0 : cubelib_all_corners)
    {
        for (cubelib_direction_t direction : cubelib_all_directions)
        {
            cubelib_corner_t corner1 = cubelib_corner_move(corner0, direction);
            
            ASSERT_FALSE(cubelib_is_corner_equal(corner0, corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            int xyz0[] = {cubelib_get_corner_x(corner0), cubelib_get_corner_y(corner0), cubelib_get_corner_z(corner0)};
            int dir[] = {cubelib_get_direction_x(direction), cubelib_get_direction_y(direction), cubelib_get_direction_z(direction)};
            int xyz1_expected[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz1_expected[i] = xyz0[i] + dir[i]*2;
            
            bool expected_out_of_bounds = false;
            
            for (int i = 0; i < 3; ++i)
                if (std::abs(xyz1_expected[i]) != 1)
                    expected_out_of_bounds = true;
            
            
            ///corner1 is null <=> it was out of bound
            ASSERT_EQ(cubelib_is_corner_null(corner1), expected_out_of_bounds)
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            
            if (cubelib_is_corner_null(corner1))
                continue;
            
            int xyz1_actual[] = {cubelib_get_corner_x(corner1), cubelib_get_corner_y(corner1), cubelib_get_corner_z(corner1)};
            
            for (int i = 0; i < 3; ++i)
                ASSERT_EQ(xyz1_actual[i] - xyz0[i], 2*dir[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            for (int i = 0; i < 3; ++i)
                ASSERT_EQ(xyz1_actual[i], xyz1_expected[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
        }
    }
    
    
    
    
    for (cubelib_corner_t corner0 : cubelib_all_corners)
    {
        for (cubelib_direction_t direction : cubelib_all_directions)
        {
            auto corner1 = cubelib_corner_move(corner0,direction);
            ASSERT_FALSE(cubelib_is_corner_equal(corner0, corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;

            auto x1 = cubelib_get_corner_x(corner0) + 2*cubelib_get_direction_x(direction);
            auto y1 = cubelib_get_corner_y(corner0) + 2*cubelib_get_direction_y(direction);
            auto z1 = cubelib_get_corner_z(corner0) + 2*cubelib_get_direction_z(direction);

            if (x1 > 1 || x1 < -1 || y1 > 1 || y1 < -1 || z1 > 1 || z1 < -1)
            {
                ASSERT_TRUE(cubelib_is_corner_null(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            } else {
                ASSERT_EQ(x1, cubelib_get_corner_x(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(y1, cubelib_get_corner_y(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(z1, cubelib_get_corner_z(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            }

        }
    }
}



TEST_F(CubelibCornerTest,corner_push)
{

    for (cubelib_corner_t corner0 : cubelib_all_corners)
    {
        for (cubelib_direction_t direction : cubelib_all_directions)
        {
            cubelib_corner_t corner1 = cubelib_corner_push(corner0, direction);
            
            ASSERT_FALSE(cubelib_is_corner_null(corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            int xyz0[] = {cubelib_get_corner_x(corner0), cubelib_get_corner_y(corner0), cubelib_get_corner_z(corner0)};
            int dir[] = {cubelib_get_direction_x(direction), cubelib_get_direction_y(direction), cubelib_get_direction_z(direction)};
            int xyz1_expected[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz1_expected[i] = xyz0[i] + dir[i]*2;
            
            bool expected_out_of_bounds = false;
            
            for (int i = 0; i < 3; ++i)
                if (std::abs(xyz1_expected[i]) != 1)
                    expected_out_of_bounds = true;
            
            
            ///(corner1 == corner0) <=> it was out of bound
            ASSERT_EQ(cubelib_is_corner_equal(corner0,corner1), expected_out_of_bounds)
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            
            
            if (expected_out_of_bounds)
                continue;
            
            int xyz1_actual[] = {cubelib_get_corner_x(corner1), cubelib_get_corner_y(corner1), cubelib_get_corner_z(corner1)};
            
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
    for (cubelib_corner_t corner0 : cubelib_all_corners)
    {
        for (cubelib_direction_t direction : cubelib_all_directions)
        {
            auto corner1 = cubelib_corner_push(corner0,direction);
            ASSERT_FALSE(cubelib_is_corner_null(corner1))
                << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;

            auto x1 = cubelib_get_corner_x(corner0) + 2*cubelib_get_direction_x(direction);
            auto y1 = cubelib_get_corner_y(corner0) + 2*cubelib_get_direction_y(direction);
            auto z1 = cubelib_get_corner_z(corner0) + 2*cubelib_get_direction_z(direction);

            if (x1 > 1 || x1 < -1 || y1 > 1 || y1 < -1 || z1 > 1 || z1 < -1)
            {
                ASSERT_TRUE(!cubelib_is_corner_null(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_TRUE(cubelib_is_corner_equal(corner0,corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            } else {
                ASSERT_EQ(x1, cubelib_get_corner_x(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(y1, cubelib_get_corner_y(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                ASSERT_EQ(z1, cubelib_get_corner_z(corner1)) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
            }

        }
    }
}

TEST_F(CubelibCornerTest,is_corner_adjacent_corner)
{

    for (cubelib_corner_t corner0 : cubelib_all_corners)
    {
        for (cubelib_direction_t direction : cubelib_all_directions)
        {
            cubelib_corner_t corner1 = cubelib_get_adjacent_corner(corner0, direction);
            
            ASSERT_TRUE(!cubelib_is_corner_null(corner1));
            ASSERT_TRUE(cubelib_is_corner_adjacent_corner(corner0, corner1));
            
            
            
        }
    }
    
    ///@TODO: test for false positives
}




TEST_F(CubelibCornerTest,cubelib_calc_cnr_adj_cnr)
{

    
    
    ///test cubelib_calc_cnr_adj_cnr()
    {
        for (cubelib_corner_t corner0 : cubelib_all_corners)
        {
            int xyz0[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz0[i] = cubelib_get_corner_i(corner0, i);
            EXPECT_EQ(cubelib_get_corner_x(corner0), xyz0[0]);
            EXPECT_EQ(cubelib_get_corner_y(corner0), xyz0[1]);
            EXPECT_EQ(cubelib_get_corner_z(corner0), xyz0[2]);
            
            for (int dim = 0; dim < 3; ++dim)
            {
                auto corner1 = cubelib_calc_cnr_adj_cnr(corner0,dim);
                
                //std::cout << "corner0: " << corner0 << ", corner1: " << corner1
                //<< ", dim: " << dim
                //<< std::endl;
                int xyz1[] = {0,0,0};
                for (int i = 0; i < 3; ++i)
                    xyz1[i] = cubelib_get_corner_i(corner1, i);
                EXPECT_EQ(cubelib_get_corner_x(corner1), xyz1[0]);
                EXPECT_EQ(cubelib_get_corner_y(corner1), xyz1[1]);
                EXPECT_EQ(cubelib_get_corner_z(corner1), xyz1[2]);
                
                
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

    
    
    ///test cubelib_calc_cnr_adj_cnr()
    {
        for (cubelib_corner_t corner0 : cubelib_all_corners)
        {
            int xyz0[] = {0,0,0};
            for (int i = 0; i < 3; ++i)
                xyz0[i] = cubelib_get_corner_i(corner0, i);
            EXPECT_EQ(cubelib_get_corner_x(corner0), xyz0[0]);
            EXPECT_EQ(cubelib_get_corner_y(corner0), xyz0[1]);
            EXPECT_EQ(cubelib_get_corner_z(corner0), xyz0[2]);
            
            for (int dim = 0; dim < 3; ++dim)
            {
                auto corner1 = cubelib_calc_cnr_adj_cnr(corner0,dim);
                
                //std::cout << "corner0: " << corner0 << ", corner1: " << corner1
                //<< ", dim: " << dim
                //<< std::endl;
                int xyz1[] = {0,0,0};
                for (int i = 0; i < 3; ++i)
                    xyz1[i] = cubelib_get_corner_i(corner1, i);
                EXPECT_EQ(cubelib_get_corner_x(corner1), xyz1[0]);
                EXPECT_EQ(cubelib_get_corner_y(corner1), xyz1[1]);
                EXPECT_EQ(cubelib_get_corner_z(corner1), xyz1[2]);
                
                
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



TEST_F(CubelibCornerTest,is_corner_on_face)
{
    for (auto face : cubelib_all_faces)
    {
        
        uint32_t edge_corners_per_face[CUBELIB_CORNERS_SIZE] = {0};
        
        ///for each edge in the face, count up the corner occurrences
        for (auto edge : cubelib_edges_on_face[cubelib_get_face_index(face)])
        {
            auto edge_corner0 = cubelib_get_edge_corner0(edge);
            auto edge_corner1 = cubelib_get_edge_corner1(edge);
            edge_corners_per_face[ cubelib_get_corner_index(edge_corner0) ]++;
            edge_corners_per_face[ cubelib_get_corner_index(edge_corner1) ]++;
        }
        
        ///Check the corners have the right counts
        for (auto corner : cubelib_all_corners)
        {
            int edge_corner_count = edge_corners_per_face[ cubelib_get_corner_index(corner) ];
            
            ///cubelib_is_corner_on_face(corner,face) => edge_corner_count == 2
            ASSERT_TRUE( !cubelib_is_corner_on_face(corner,face) || edge_corner_count == 2 );
            
            ///!cubelib_is_corner_on_face(corner,face) => edge_corner_count == 0
            ASSERT_TRUE( cubelib_is_corner_on_face(corner,face) || edge_corner_count == 0 );
        }
    
    
        auto direction = cubelib_get_face_direction(face);
        
        auto significant_dimension = cubelib_get_direction_sigdim(direction);
        ASSERT_LT(significant_dimension, 3);
        
        for (auto corner : cubelib_all_corners)
        {
            
            ///test that the non-zero component of the direction vector has the same value of the same component
            /// in the corner vector representation. For example, face/direction (0,0,+1) would have all (u,v,+1) corners on its
            /// face.
            if (cubelib_is_corner_on_face(corner,face))
            {
                ASSERT_EQ(cubelib_get_direction_i(direction,significant_dimension), cubelib_get_corner_i(corner,significant_dimension));
            } else {
                ASSERT_NE(cubelib_get_direction_i(direction,significant_dimension), cubelib_get_corner_i(corner,significant_dimension));
            }
          
          
        }
        
        
        ///logically test all corners that are on and off the face.
        ///test that the non-zero component of the direction vector has the same value of the same component
        /// in the corner vector representation. For example, face/direction (0,0,+1) would have all (u,v,+1) corners on its
        /// face.
        for (int u : {-1,+1})
        for (int v : {-1,+1})
        {
            ///manually make an int3 corner
            int xyz[] = {0,0,0};
            ///set the significant dimension to match the direction's
            xyz[significant_dimension] = cubelib_get_direction_i(direction,significant_dimension);
            
            ///set the other components to u,v
            xyz[(significant_dimension + 1) % 3] = u;
            xyz[(significant_dimension + 2) % 3] = v;
            
            {
              ///construct the corner
              auto corner = cubelib_get_corner_by_int3(xyz[0],xyz[1],xyz[2]);
              
              ASSERT_TRUE(cubelib_is_corner_on_face(corner,face));
            }
            
            
            ///set the significant dimension to NOT match the direction's (note the negative)
            xyz[significant_dimension] = -cubelib_get_direction_i(direction,significant_dimension);
            
            {
                ///construct the corner
                auto corner = cubelib_get_corner_by_int3(xyz[0],xyz[1],xyz[2]);
                ASSERT_FALSE(cubelib_is_corner_on_face(corner,face));
            }
        }
    }
    
    
}



TEST_F(CubelibCornerTest,corner_formatters)
{

    
    
    std::vector< std::tuple<cubelib_corner_t, std::string> > tests;
    
    tests.push_back( std::make_tuple(cubelib_get_corner_by_int3(0,0,1), "(-1,-1,1)") );
    tests.push_back( std::make_tuple(cubelib_get_corner_by_int3(0,0,-1), "(-1,-1,-1)") );
    tests.push_back( std::make_tuple(cubelib_get_corner_by_int3(0,1,-1), "(-1,1,-1)") );
    
    
    for (auto test : tests)
    {
        auto corner = std::get<0>(test);
        
        auto expected = std::get<1>(test);
        
        auto actual = tostr(corner);
        
        EXPECT_EQ(expected, actual);
    }
    
}