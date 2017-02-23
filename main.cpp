#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>

#include "knapsack.hpp"

#define DEBUG

struct EndPoint {
    EndPoint(int latency) : latency(latency) {}
    int latency;
    std::unordered_map<int,int> caches;
};

struct Request {
    int id, endpoint, num_req;
};

struct Result {
    int cache_id;
    std::vector<int> video_ids;
};

int V, E, R, C, X;
std::vector<int> video_sizes;
std::vector<EndPoint> end_points;
std::vector<Request> requests;

std::vector<std::vector<int>> knapsack_solver() {
    // video ID to total saved time
    std::vector<std::unordered_map<int, int>> cache_videos(C);

    for (auto &r : requests) {
        for (auto &c : end_points[r.endpoint].caches) {
            int cache_id = c.first;
            int cache_latency = c.second;
            int saved_time = end_points[r.endpoint].latency - cache_latency;

            cache_videos[cache_id][r.id] += r.num_req*saved_time;
        }
    }

    std::vector<std::vector<int>> res;
    int count = 0;
    for (auto &cache : cache_videos) {
        std::cerr << count << '\n';
        // knapsack items: weight MB, value saved time
        std::vector<Item> items;
        // video ids of the cache videos
        std::vector<int> ids;
        for (auto &video : cache) {
            int video_id = video.first;
            int saved_time = video.second;
            items.emplace_back(saved_time, video_sizes[video_id]);
            ids.emplace_back(video_id);
        }
        std::vector<int> added_videos;
        knapsack(X, items.begin(), items.end(), back_inserter(added_videos));
        std::vector<int> added_ids;
        // get the added video ids
        for (auto &item_index : added_videos) {
            added_ids.emplace_back(ids[item_index]);
        }
        res.push_back(std::move(added_ids));
        count++;
    }

    return res;
}

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
    }
}

int main(int argc, char const *argv[]) {
    read_input();
    auto result = knapsack_solver();
    std::cout << result.size() << '\n';
    for (int i = 0; i < result.size(); i++) {
        std::cout << i << " ";
        for (auto id : result[i]) {
            std::cout << id << " ";
        }
        std::cout << '\n';
    }
    return 0;
}
