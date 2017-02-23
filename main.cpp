#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#define DEBUG

struct EndPoint {
    EndPoint(int latency) : latency(latency) {}
    int latency;
    std::unordered_map<int,int> caches;
};

struct Request {
    int id, endpoint, num_req;
};

int V, E, R, C, X;
std::vector<int> video_sizes;
std::vector<EndPoint> end_points;
std::vector<Request> requests;

void read_input() {
    std::cin >> V >> E >> R >> C >> X;

    for (size_t i = 0; i < V; i++) {
        int s_i;
        std::cin >> s_i;
        video_sizes.emplace_back(s_i);
    }

    for (size_t i = 0; i < E; i++) {
        int latency;
        std::cin >> latency;
        int caches;
        std::cin >> caches;
        end_points.emplace_back(latency);
        for (size_t j = 0; j < caches; j++) {
            int id, cache_latency;
            std::cin >> id >> cache_latency;
            end_points.back().caches[id] = cache_latency;
        }
    }

    for (size_t i = 0; i < R; i++) {
        int id, endpoint, num_req;
        std::cin >> id >> endpoint >> num_req;
        requests.emplace_back(Request{id, endpoint, num_req});
    }}

int main(int argc, char const *argv[]) {
    read_input();
    return 0;
}
