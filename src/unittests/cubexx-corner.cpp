



#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include "gtest/gtest.h"







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






