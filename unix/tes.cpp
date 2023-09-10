#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

struct A
{
    uint32_t a[6];
};

typedef struct _host_info_s {
    uint64_t mac64;
    std::string macstr;
    uint32_t ip32;
    std::string ipv4str;
    uint8_t ip128[16];
    std::string ipv6str;
} host_info_s;

int main()
{
    //unordered_map<string, host_info_s> m;
    uint8_t addr[6];
    // cout << m["1"].macstr << endl;
    // cout << m["1"].ip32 << endl;
    // cout << m["1"].ipv4str << endl;
    // m["2"].ipv4str = "";
    // cout << m["2"].ipv4str << endl;
    // const char * str = m["2"].ipv4str.c_str();
    // cout << str << endl;
    cout << addr[0] << endl;
    cout << hex << addr[1] << endl;
    cout << hex << addr[2] << endl;
    uint8_t a = 10 ;
    cout << a << endl;
    char str[19];
    snprintf(str, sizeof str, "%02x:%02x:%02x:%02x:%02x:%02x", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    cout <<str << endl;
    A a_obj;
    cout << a_obj.a[0] << endl;
    cout << a_obj.a[1] << endl;
    cout << a_obj.a[2] << endl;
    cout << a_obj.a[3] << endl;
}