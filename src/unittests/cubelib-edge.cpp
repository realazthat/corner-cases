


#include "cubelib/cubelib.h"
#include "cubelib/formatters.hpp"

#include "gtest/gtest.h"



#include <vector>
#include <tuple>


struct CubelibEdgeTest : public ::testing::Test {
protected:
    virtual void SetUp() {
        
    }

    virtual void TearDown() {
    // Code here will be called immediately after each test
    // (right before the destructor).
    
    }
};



TEST_F(CubelibEdgeTest,null)
{

    
    ///test null edge
    {
        EXPECT_TRUE(is_edge_valid(null_edge));
        EXPECT_TRUE(is_edge_null(null_edge));
        EXPECT_TRUE(is_edge_equal(null_edge, null_edge));
    }
    
    ///make sure all the regular edges are not null_edge
    for (auto edge : all_edges)
    {
        EXPECT_FALSE(is_edge_null(edge));
        EXPECT_TRUE(is_edge_valid(edge));
        EXPECT_FALSE(is_edge_equal(edge, null_edge));
    }

}


