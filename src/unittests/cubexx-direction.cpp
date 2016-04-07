



#include "cubexx/cubexx.hpp"
#include "cubexx/formatters.hpp"

#include "gtest/gtest.h"







struct CUBEXXDirectionTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};


TEST_F(CUBEXXDirectionTest,SIZE)
{
    ASSERT_EQ(6U, cubexx::direction_t::SIZE());
}

TEST_F(CUBEXXDirectionTest,sanity)
{
    
    for (auto direction : cubexx::direction_t::all())
    {
        int x = direction.x(), y = direction.y(), z = direction.z();
        
        
        ASSERT_TRUE( x == 0 || x == -1 || x == 1)
            << "direction: " << direction;
        ASSERT_TRUE( y == 0 || y == -1 || y == 1)
            << "direction: " << direction;
        ASSERT_TRUE( z == 0 || z == -1 || z == 1)
            << "direction: " << direction;
        
        
        
        ASSERT_TRUE( x != 0 || y != 0 || z != 0 )
            << "direction: " << direction;
            
            
        ASSERT_TRUE( std::abs(x) + std::abs(y) + std::abs(z) == 1 )
            << "direction: " << direction;
    }
    
}


TEST_F(CUBEXXDirectionTest,index)
{
    ///test direction_t::index() and direction_t::index(index)
    uint32_t index = 0;
    for (auto direction : cubexx::direction_t::all())
    {
        EXPECT_EQ(index, direction.index());
        
        EXPECT_EQ(direction, cubexx::direction_t::get(index));
        
        
        ++index;
    }
}

TEST_F(CUBEXXDirectionTest,unique)
{

    ///test direction uniqueness
    {
        ///mark each direction's index, and test if all the indices get marked off
        uint32_t bitmask = 0;
        for (auto direction : cubexx::direction_t::all())
        {
            bitmask |= (1 << direction.index());
        }
        
        EXPECT_EQ(bitmask, uint32_t(1 << 6) - 1);
        
        
    }
    
    
    
    std::map< std::tuple<int,int,int>, cubexx::direction_t > xyz_values;
    std::map< std::size_t, cubexx::direction_t > index_values;
    for (auto direction : cubexx::direction_t::all())
    {
        int x = direction.x(), y = direction.y(), z = direction.z();
        

        if (xyz_values.count(std::make_tuple(x,y,z)) > 0)
        {
            auto direction0 = xyz_values[std::make_tuple(x,y,z)];
            ASSERT_TRUE( xyz_values.count(std::make_tuple(x,y,z)) == 0 )
                << "direction: " << direction
                << ", (x,y,z): " << "(" << x << "," << y << "," << z << ")"
                << ", direction0: " << direction0;
        }
        xyz_values[std::make_tuple(x,y,z)] = direction;
        
        if (index_values.count(direction.index()) > 0)
        {
            
            auto direction0 = index_values[direction.index()];
            ASSERT_TRUE( index_values.count(direction.index()) == 0 )
                << "direction: " << direction
                << ", (x,y,z): " << "(" << x << "," << y << "," << z << ")"
                << ", direction0: " << direction0;
            
        }
        index_values[direction.index()] = direction;
        
    }
}

TEST_F(CUBEXXDirectionTest,opposite)
{

    ///test direction_t::opposite()
    {
        
        
        for (auto direction : cubexx::direction_t::all())
        {
            
            auto opposite_direction = direction.opposite();
            
            
            EXPECT_NE(direction, opposite_direction);
            EXPECT_EQ(direction, opposite_direction.opposite());
            EXPECT_EQ(-opposite_direction.x(), direction.x());
            EXPECT_EQ(-opposite_direction.y(), direction.y());
            EXPECT_EQ(-opposite_direction.z(), direction.z());
            
        }
        
    }
}



TEST_F(CUBEXXDirectionTest,get)
{

    ///test direction get(int,int,int) and get(direction_t)
    {
        uint32_t mask = 0;
        
        for (int dim = 0; dim < 3; ++dim)
        for (int sign = -1; sign <= 1; sign += 2)
        {
            
            int xyz[] = {0,0,0};
            
            xyz[dim] = sign;
            
            int x = xyz[0], y = xyz[1], z = xyz[2];
            
            auto direction = cubexx::direction_t::get(x,y,z);
            
            EXPECT_EQ(direction.x(), x);
            EXPECT_EQ(direction.y(), y);
            EXPECT_EQ(direction.z(), z);
            
            EXPECT_EQ(direction.positive(), sign > 0);
            EXPECT_EQ(direction, direction.get(direction));
            
            mask |= (1 << direction.index());
        }
        
        EXPECT_EQ(mask, uint32_t(1 << cubexx::direction_t::SIZE()) - 1);
    }
}


TEST_F(CUBEXXDirectionTest,positive)
{

    ///test direction positive
    {
        uint32_t mask = 0;
        
        for (int dim = 0; dim < 3; ++dim)
        for (int sign = -1; sign <= 1; sign += 2)
        {
            
            int xyz[] = {0,0,0};
            
            xyz[dim] = sign;
            
            int x = xyz[0], y = xyz[1], z = xyz[2];
            
            auto direction = cubexx::direction_t::get(x,y,z);
            
            EXPECT_EQ(direction.positive(), sign > 0);
            
            mask |= (1 << direction.index());
        }
        
        EXPECT_EQ(mask, uint32_t(1 << cubexx::direction_t::SIZE()) - 1);
    }
}



TEST_F(CUBEXXDirectionTest,xyz)
{

    for (auto direction : cubexx::direction_t::all())
    {
        auto xyz = direction.xyz();
        ASSERT_EQ(direction.x(), xyz[0]);
        ASSERT_EQ(direction.y(), xyz[1]);
        ASSERT_EQ(direction.z(), xyz[2]);
    }
}






TEST_F(CUBEXXDirectionTest,axis)
{
    for (auto direction : cubexx::direction_t::all())
    {
        auto axis = direction.axis();
        auto xyz = direction.xyz();

        ASSERT_NE(0, direction.xyz()[axis]);

        ASSERT_EQ(0, direction.xyz()[(axis + 1 ) % 3]) << "direction: " << direction
                                                       << ", axis: " << (int)axis
                                                       << ", (axis + 1 ) % 3: " << (int)((axis + 1 ) % 3)
                                                       << ", direction.xyz()[(axis + 1 ) % 3]: " << (int)direction.xyz()[(axis + 1 ) % 3]
                                                       << ", xyz[(axis + 1 ) % 3]: " << (int)xyz[(axis + 1 ) % 3]
                                                        ;
        ASSERT_EQ(0, direction.xyz()[(axis + 2 ) % 3]) << "direction: " << direction << ", axis: " << (int)axis;
    }
}





TEST_F(CUBEXXDirectionTest,face)
{

    ASSERT_TRUE(false);
}





