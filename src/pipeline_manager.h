#pragma once
#include "pipeline_builder.h"
#include <unordered_map>
#include <mutex>

struct Pipeline {
    GstElement* pipeline;
};

class PipelineManager {
public:
    void add(const PipelineConfig& cfg);

    bool start(const std::string& id);
    bool stop(const std::string& id);

private:
    std::unordered_map<std::string, Pipeline> pipelines_;
    std::mutex mutex_;
};