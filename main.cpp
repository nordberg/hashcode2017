#include <iostream>
#include <vector>
#include <string>

struct Cache {
    int id, latency;
};

struct EndPoint {
    int center_latency;
    std::vector<Cache> caches;
};

struct Request {
    int id, endpoint, num_req;
};

int main(int argc, char const *argv[]) {
    int V, E, R, C, X;
    std::cin >> V >> E >> R >> C >> X;

    std::vector<int> video_sizes;
    for (size_t i = 0; i < V; i++) {
        int s_i;
        std::cin >> s_i;
        video_sizes.emplace_back(s_i);
    }

    std::vector<EndPoint> end_points;
    for (size_t i = 0; i < E; i++) {
        int latency;
        std::cin >> latency;
        int caches;
        std::cin >> caches;
        std::vector<Cache> cache_v;
        for (size_t j = 0; j < caches; j++) {
            int id, cache_latency;
            std::cin >> id >> cache_latency;
            cache_v.emplace_back(Cache{id, cache_latency});
        }
        end_points.emplace_back(EndPoint{latency, cache_v});
    }

    std::vector<Request> requests;
    for (size_t i = 0; i < R; i++) {
        int id, endpoint, num_req;
        std::cin >> id >> endpoint >> num_req;
        requests.emplace_back(Request{id, endpoint, num_req});
    }

    return 0;
}
