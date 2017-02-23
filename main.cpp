#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <tuple>
#include <algorithm>

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

std::vector<std::unordered_set<int>> points_in_cache;
std::vector<int> median_dist;

// endpoints to video
std::vector<std::unordered_set<int>> video_by_endpoint;
// videos needed by endpoint
std::vector<std::unordered_set<int>> endpoint_by_video;

typedef std::pair<int,int> pii;

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

    std::vector<pii> cache_with_sizes;
    for (int i = 0; i < C; i++) {
        cache_with_sizes.emplace_back(i, median_dist[i]);
    }
    std::sort(cache_with_sizes.begin(), cache_with_sizes.end(), [](const pii &a, const pii &b){
        return a.second < b.second;
    });

    for (auto &p : cache_with_sizes) {
        int i = p.first;
        auto &cache = cache_videos[i];

        std::cerr << count << '\n';
        // knapsack items: weight MB, value saved time
        std::vector<Item> items;
        // video ids of the cache videos
        std::vector<int> ids;
        for (auto &video : cache) {
            int video_id = video.first;
            int saved_time = video.second;

            // dont add covered videos
            bool needed = false;
            for (auto &endp : video_by_endpoint[video_id]) {
                if (points_in_cache[i].count(endp) > 0) {
                    needed = true;
                    break;
                }
            }
            if (!needed) continue;

            items.emplace_back(saved_time, video_sizes[video_id]);
            ids.emplace_back(video_id);
        }
        std::vector<int> added_videos;
        knapsack(X, items.begin(), items.end(), back_inserter(added_videos));
        std::vector<int> added_ids;

        // get the added video ids
        for (auto &item_index : added_videos) {
            added_ids.emplace_back(ids[item_index]);
            for (int endpoint : points_in_cache[i]) {
                endpoint_by_video[endpoint].erase(added_ids.back());
                video_by_endpoint[added_ids.back()].erase(endpoint);
            }
        }
        res.push_back(std::move(added_ids));
        count++;
    }

    return res;
}

void read_input() {
    std::cin >> V >> E >> R >> C >> X;
    median_dist = std::vector<int>(C);
    points_in_cache = std::vector<std::unordered_set<int>>(C);
    video_by_endpoint = std::vector<std::unordered_set<int>>(V);
    endpoint_by_video = std::vector<std::unordered_set<int>>(E);

    std::vector<std::vector<int>> latencies(C);

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
            points_in_cache[id].insert(i);
            latencies[id].emplace_back(cache_latency);
        }
    }

    for (size_t i = 0; i < C; i++) {
        std::sort(latencies[i].begin(), latencies[i].end());
        median_dist[i] = latencies[i][latencies[i].size()/2];
    }

    for (size_t i = 0; i < R; i++) {
        int id, endpoint, num_req;
        std::cin >> id >> endpoint >> num_req;
        requests.emplace_back(Request{id, endpoint, num_req});
        endpoint_by_video[endpoint].insert(id);
        video_by_endpoint[id].insert(endpoint);
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
