#include "tcpclient.h"
#include "gtest/gtest.h"

class TestClient : public ::testing::Test
{
    public:
    void SetUp() {}
    void TearDown() {}
};

TEST_F(TestClient, test_getfd)
{
    tcpclient client;
    EXPECT_EQ(client.get_client_fd(), -1);
}