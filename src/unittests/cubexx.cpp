



#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include "gtest/gtest.h"






int main(int argc, char **argv){
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}





struct CUBEXXTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};




TEST_F(CUBEXXTest,corner_unique)
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
        
        
        
        ///test corner_t::index()
        uint32_t index = 0;
        for (auto corner : cubexx::corner_t::all())
        {
            EXPECT_EQ(index, corner.index());
            
            EXPECT_EQ(corner, cubexx::corner_t::index(index));
            
            auto opposite_corner = corner.opposite();
            
            
            EXPECT_NE(corner, opposite_corner);
            EXPECT_EQ(corner, opposite_corner.opposite());
            EXPECT_EQ(-opposite_corner.x(), corner.x());
            EXPECT_EQ(-opposite_corner.y(), corner.y());
            EXPECT_EQ(-opposite_corner.z(), corner.z());
            
            ++index;
        }
        
    }
}

TEST_F(CUBEXXTest,corner_get)
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

TEST_F(CUBEXXTest,corner_adjacent_direction)
{

    ///test corner adjacent(direction_t)
    {
        
        for (auto corner0 : cubexx::corner_t::all())
        {
            for (auto direction : cubexx::direction_t::all())
            {
                auto corner1 = corner0.adjacent(direction);
                
                EXPECT_NE(corner0, corner1);
                
                
                EXPECT_TRUE(corner0.is_adjacent(corner1));
                
                
                int xyz0[] = {corner0.x(), corner0.y(), corner0.z()};
                int dir[] = {direction.x(), direction.y(), direction.z()};
                int xyz1_expected[] = {0,0,0};
                for (int i = 0; i < 3; ++i)
                    xyz1_expected[i] = xyz0[i] + dir[i]*2;
                
                bool expected_out_of_bounds = false;
                
                for (int i = 0; i < 3; ++i)
                    if (std::abs(xyz1_expected[i]) != 1)
                        expected_out_of_bounds = true;
                
                
                //ASSERT_TRUE(!is_null_corner(corner1) || expected_out_of_bounds) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                
                
                //if (is_null_corner(corner1))
                //    continue;
                    
                int xyz1_actual[] = {corner1.x(), corner1.y(), corner1.z()};
                
                for (int i = 0; i < 3; ++i)
                    EXPECT_EQ(xyz1_actual[i] - xyz0[i], 2*dir[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                for (int i = 0; i < 3; ++i)
                    EXPECT_EQ(xyz1_actual[i], xyz1_expected[i]) << "corner0: " << corner0 << ", direction: " << direction << ", corner1: " << corner1;
                
            }
        }
        
        

    }
}




